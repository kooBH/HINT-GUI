#include "WidgetControl.h"


WidgetControl::WidgetControl(){
  layout_main.setAlignment(Qt::AlignTop);

  init_toolbar();
  layout_main.addLayout(&layout_toolbar);

  init_text();
  layout_main.addLayout(&layout_text);
  update_answer();
  layout_main.addLayout(&layout_answer);

  init_left();
  layout_two_col.addWidget(&group_score,1);
  init_right();
  layout_two_col.addWidget(&group_dB,1);
  layout_main.addLayout(&layout_two_col);
 
  setLayout(&layout_main);
}

WidgetControl::~WidgetControl(){
}

void WidgetControl::init_toolbar() {

  layout_toolbar.setAlignment(Qt::AlignLeft);

  label_L.setText("L:");
  layout_toolbar.addWidget(&label_L);

  for (int i = 1; i < 15; i++) {
    combo_L.addItem(QString::number(i));
  }
  layout_toolbar.addWidget(&combo_L);
  
  label_clean_pre.setText("Clean: ");
  layout_toolbar.addWidget(&label_clean_pre);

  label_clean.setText("Default");
  layout_toolbar.addWidget(&label_clean);

  label_noise.setText("Noise:");
  layout_toolbar.addWidget(&label_noise);

  listing_noise();
  layout_toolbar.addWidget(&combo_noise);

  btn_tmp_clean.setText("[TMP]Play Clean");
  layout_toolbar.addWidget(&btn_tmp_clean);

  btn_play.setText("[TMP]Play Noise");
  layout_toolbar.addWidget(&btn_play);



  QObject::connect(&btn_tmp_clean, &QPushButton::clicked, this, [&] {player->PlayClean(); });
  QObject::connect(&btn_play, &QPushButton::clicked, this, [&] {player->PlayNoise1(); });
  QObject::connect(&combo_noise, &QComboBox::currentIndexChanged, this, [this](int index) {player->SetNoise1Path(vec_noise[index].second); });
}

void WidgetControl::init_text() {
  layout_text.setAlignment(Qt::AlignRight);

  label_text.setText(QString::fromLocal8Bit("정답은 띄어쓰기 기반으로 나눠집니다."));
  label_text.setStyleSheet("background-color: white; font-weight: bold; font-size: 32pt;");
  layout_text.addWidget(&label_text);

  btn_select_all.setText("Select All");
  layout_text.addWidget(&btn_select_all);

  btn_next.setText("Next");
  layout_text.addWidget(&btn_next);
}

void WidgetControl::update_answer() {

  vec_btn_answer.clear();

  QStringList tokens = label_text.text().split(' ', Qt::SkipEmptyParts);

  for (const QString& token : tokens) {
    auto btn = std::make_unique<QPushButton>();
    btn.get()->setText(token);
    layout_answer.addWidget(btn.get());  
    vec_btn_answer.push_back(std::move(btn));
  }
}

void WidgetControl::init_left() {

    group_score.setTitle("Score");
    layout_left.setAlignment(Qt::AlignTop);

    label_score.setText("Score here");
    layout_left.addWidget(&label_score);

    label_remain.setText("Remain here");
    layout_left.addWidget(&label_remain);

    group_score.setLayout(&layout_left);

}

void WidgetControl::init_right() {

    group_dB.setTitle("dB");
    layout_right.setAlignment(Qt::AlignTop);

    layout_SNR.setAlignment(Qt::AlignRight);
    label_SNR.setText("SNR:");
    layout_SNR.addWidget(&label_SNR);

    edit_SNR.setText("0.0");
    layout_SNR.addWidget(&edit_SNR);
      
    label_dB.setText("dB");
    layout_SNR.addWidget(&label_dB);

    layout_right.addLayout(&layout_SNR);

    btn_tune.setText("Play Tune");
    layout_tune.addWidget(&btn_tune);
    layout_right.addLayout(&layout_tune);
  
    group_dB.setLayout(&layout_right);

    QObject::connect(&btn_tune, &QPushButton::clicked, this, [&] {
      player->PlayTune();
      if (flag_play_tune) {
        flag_play_tune = false;
        btn_tune.setText("Play Tune");
      }
      else {
        flag_play_tune = true;
        btn_tune.setText("Stop Tune");
      
      }
      
      });
}

void WidgetControl::ConnectPlayer(Player* p) {
  player = p;

  player->SetCleanPath(QString("../data/SNR30_int16/IBMSignal_SNR30_L1_S1.wav"));
  player->SetNoise1Path(vec_noise[combo_noise.currentIndex()].second);
}

int WidgetControl::listing_noise() {

  QDir dir("../data/stationary-noise");
  QStringList filters;
  vec_noise.clear();

  filters << "*.wav" << "*.WAV";

  // QDir::Files : 파일만, QDir::NoDotAndDotDot : . .. 제외
  QStringList wavFiles = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

  for (int i = 0; i < wavFiles.size(); ++i) {
    vec_noise.push_back(std::make_pair(wavFiles[i], dir.absoluteFilePath(wavFiles[i])));
    combo_noise.addItem(wavFiles[i]);
  }

  return wavFiles.size();
  
}


