#include "WidgetControl.h"


WidgetControl::WidgetControl(){
  load_script();

  //
  layout_main.setAlignment(Qt::AlignTop);
  layout_main.setSpacing(20);

  init_toolbar();
  layout_main.addLayout(&layout_toolbar);

  init_text();
  layout_main.addLayout(&layout_text);
  layout_main.addLayout(&layout_answer);

  frame_line.setFrameShape(QFrame::HLine);
  frame_line.setFrameShadow(QFrame::Plain);
  layout_main.addWidget(&frame_line);

  init_left();
  layout_two_col.addWidget(&group_score,1);
  init_dB();
  layout_right.addWidget(&group_dB);

  init_device();
  layout_right.addWidget(&group_device);

  layout_two_col.addLayout(&layout_right,1);
  layout_main.addLayout(&layout_two_col);

  update_answer();
  setLayout(&layout_main);

}

WidgetControl::~WidgetControl(){

}

void WidgetControl::init_toolbar() {
  layout_toolbar.setAlignment(Qt::AlignLeft);

  label_L.setText("L:");
  layout_toolbar.addWidget(&label_L);

  for (int i = 1; i <= 14; i++) {
    combo_L.addItem(QString::number(i));
  }

  combo_L.setStyleSheet(R"(
        QComboBox:disabled {
            background-color: #d3d3d3; /* 배경색을 연한 회색으로 */
            color: #808080; /* 글자색을 회색으로 */
            border: 1px solid #a0a0a0; /* 테두리를 좀 더 어둡게 */
        })");

  layout_toolbar.addWidget(&combo_L);
  
  label_clean_pre.setText("Clean: ");
  layout_toolbar.addWidget(&label_clean_pre);

  listing_clean();

  label_clean.setText("Default");
  layout_toolbar.addWidget(&label_clean);

  label_clean_cnt.setText("1/20");
  layout_toolbar.addWidget(&label_clean_cnt);

  label_noise.setText("Noise:");
  layout_toolbar.addWidget(&label_noise);

  listing_noise();
  layout_toolbar.addWidget(&combo_noise);
  
  //btn_tmp_clean.setText("[TEST]Play Clean");
  //layout_toolbar.addWidget(&btn_tmp_clean);

  btn_play.setText("Play");
  layout_toolbar.addWidget(&btn_play);

  QObject::connect(&btn_tmp_clean, &QPushButton::clicked, this, [&] {
    player->Play();
    player->InsertClean(); });
  QObject::connect(&btn_play, &QPushButton::clicked, this, [&] {
    if (is_playing) {
      player->Stop();
      btn_play.setText("Play");
      is_playing = false;
      EnableControl(false);

    }
    else {
      player->Play();
      player->LoopNoise();
      player->InsertClean();
      btn_play.setText("Stop");
      is_playing = true;
      EnableControl(true);
      update_score();
    }
    
    });
  QObject::connect(&combo_L, &QComboBox::currentIndexChanged, this, [this](int index) {
    select_clean_L(index);
    this->update_answer();
    this->update_score();
    });
  QObject::connect(&combo_noise, &QComboBox::currentIndexChanged, this, [this](int index) {
    path_noise = vec_noise[index].second;
    ApplyPath();
    if (player->IsNoisePlaying())
      player->LoopNoise();

    });

  select_clean_L(0);
  update_score();
}

void WidgetControl::init_text() {
  //layout_text.setAlignment(Qt::AlignRight);

  label_text.setStyleSheet("background-color: white; font-weight: bold; font-size: 32pt;");
  layout_text.addWidget(&label_text);
  layout_text.addStretch(1);

  btn_select_all.setText("Select All");
  layout_text.addWidget(&btn_select_all);

  btn_next.setText("Next");
  layout_text.addWidget(&btn_next);

  QObject::connect(&btn_next, &QPushButton::clicked, this, [&] {
    idx_s++;
    select_S(idx_s);
    update_answer();
    update_score();
    player->InsertClean();

    if (idx_s == n_S-1) {
      btn_next.setText("End");
      btn_next.setEnabled(false);
    }

  });


  QObject::connect(&btn_select_all, &QPushButton::clicked, this, [&] {
    for (const auto& btn_ptr : vec_btn_answer) {
      if (btn_ptr) {
        btn_ptr->click();
      }
    }
    });
}

void WidgetControl::init_left() {

    group_score.setTitle("Score");
    layout_left.setAlignment(Qt::AlignTop);

    label_score.setText("Score here");
    layout_left.addWidget(&label_score);

    group_score.setLayout(&layout_left);

}

void WidgetControl::init_dB() {

    group_dB.setTitle("dB");
    layout_dB.setAlignment(Qt::AlignTop);

    layout_SNR.setAlignment(Qt::AlignRight);
    label_SNR.setText("SNR:");
    layout_SNR.addWidget(&label_SNR);

    label_dB.setText("0.0 dB");
    layout_SNR.addWidget(&label_dB);

    layout_dB.addLayout(&layout_SNR);

    label_dB_clean.setText("0.0 dB");
    btn_dB_clean_up.setText("Clean 1dB Up");
    btn_dB_clean_down.setText("Clean 1dB Down");
    layout_dB_clean.addWidget(&label_dB_clean);
    layout_dB_clean.addWidget(&btn_dB_clean_up);
    layout_dB_clean.addWidget(&btn_dB_clean_down);
    layout_dB.addLayout(&layout_dB_clean);
    QObject::connect(&btn_dB_clean_up, &QPushButton::clicked, this, [&] {
      dB_clean += 1.0;
      label_dB_clean.setText(QString::number(dB_clean, 'f', 1) + "dB");
      player->SetdBClean(dB_clean);
      label_dB.setText(QString::number(dB_noise1 - dB_clean, 'f', 1) + "dB");
      });
    QObject::connect(&btn_dB_clean_down, &QPushButton::clicked, this, [&] {
      dB_clean -= 1.0;
      label_dB_clean.setText(QString::number(dB_clean, 'f', 1) + "dB");
      player->SetdBClean(dB_clean);
      label_dB.setText(QString::number(dB_noise1 - dB_clean, 'f', 1) + "dB");
      });

    label_dB_noise1.setText("0.0 dB");
    btn_dB_noise1_up.setText("Noise 1dB Up");
    btn_dB_noise1_down.setText("Noise 1dB Down");
    layout_dB_noise1.addWidget(&label_dB_noise1);
    layout_dB_noise1.addWidget(&btn_dB_noise1_up);
    layout_dB_noise1.addWidget(&btn_dB_noise1_down);
    layout_dB.addLayout(&layout_dB_noise1);
    QObject::connect(&btn_dB_noise1_up, &QPushButton::clicked, this, [&] {
      dB_noise1 += 1.0;
      label_dB_noise1.setText(QString::number(dB_noise1, 'f', 1) + "dB");
      player->SetdBNoise1(dB_noise1);
      label_dB.setText(QString::number(dB_noise1 - dB_clean, 'f', 1) + "dB");
      });
    QObject::connect(&btn_dB_noise1_down, &QPushButton::clicked, this, [&] {
      dB_noise1 -= 1.0;
      label_dB_noise1.setText(QString::number(dB_noise1, 'f', 1) + "dB");
      player->SetdBNoise1(dB_noise1);
      label_dB.setText(QString::number(dB_noise1 - dB_clean, 'f', 1) + "dB");
      });


    btn_tune.setText("Play Tune");
    layout_tune.addWidget(&btn_tune);
    layout_dB.addLayout(&layout_tune);
  
    group_dB.setLayout(&layout_dB);

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

void WidgetControl::init_device() {

    group_device.setTitle("Device");
    layout_device.setAlignment(Qt::AlignTop);

    // Device Info
    label_device_1.setText("Device1 : ");
    label_device_2.setText("Device2 : ");
    layout_device.addWidget(&label_device_1);
    layout_device.addWidget(&label_device_2);


    // Clean
    label_device_clean.setText("Clean  ");
    layout_device_clean.setAlignment(Qt::AlignLeft);
    layout_device_clean.addWidget(&label_device_clean);
    radio_clean_device1_ch1.setText("Device1 Ch1");
    radio_clean_device1_ch2.setText("Device1 Ch2");
    btng_clean.addButton(&radio_clean_device1_ch1);
    btng_clean.addButton(&radio_clean_device1_ch2);
    layout_device_clean.addWidget(&radio_clean_device1_ch1);
    layout_device_clean.addWidget(&radio_clean_device1_ch2);
    radio_clean_device1_ch1.setChecked(true);
    layout_device.addLayout(&layout_device_clean);

    QObject::connect(&btng_clean, &QButtonGroup::buttonClicked, this, &WidgetControl::slot_clean_radio_clicked);

    // Noise
    label_device_noise.setText("Noise  ");
    layout_device_noise.setAlignment(Qt::AlignLeft);
    layout_device_noise.addWidget(&label_device_noise);
    radio_noise_device1_ch2.setText("Device1 Ch2");
    radio_noise_device1_ch1.setText("Device1 Ch1");
    //radio_noise_device2_ch12.setText("Device2 Ch1+2");
    btng_noise.addButton(&radio_noise_device1_ch2);
    btng_noise.addButton(&radio_noise_device1_ch1);
    //btng_noise.addButton(&radio_noise_device2_ch12);
    layout_device_noise.addWidget(&radio_noise_device1_ch2);
    layout_device_noise.addWidget(&radio_noise_device1_ch1);
    //layout_device_noise.addWidget(&radio_noise_device2_ch12);
    radio_noise_device1_ch2.setChecked(true);
    layout_device.addLayout(&layout_device_noise);

    QObject::connect(&btng_noise, &QButtonGroup::buttonClicked, this, &WidgetControl::slot_noise_radio_clicked);

    // Tune
    label_device_tune.setText("Tune    ");
    layout_device_tune.setAlignment(Qt::AlignLeft);
    layout_device_tune.addWidget(&label_device_tune);
    radio_tune_device1_ch1.setText("Device1 Ch1");
    radio_tune_device1_ch2.setText("Device1 Ch2");
    btng_tune.addButton(&radio_tune_device1_ch1);
    btng_tune.addButton(&radio_tune_device1_ch2);
    layout_device_tune.addWidget(&radio_tune_device1_ch1);
    layout_device_tune.addWidget(&radio_tune_device1_ch2);
    radio_tune_device1_ch1.setChecked(true);
    layout_device.addLayout(&layout_device_tune);

    group_device.setLayout(&layout_device);
}

// Functional Part

void WidgetControl::ConnectPlayer(Player* p) {
  player = p;

  path_clean = vec_clean[cur_l][idx_s].second;
  path_noise = vec_noise[combo_noise.currentIndex()].second;

  player->SetCleanPath(path_clean);
  player->SetNoisePath(path_noise);

  QObject::connect(player, &Player::SignalUpdateDevice1Name, this, &WidgetControl::SLotUpdateDevice1Name);
}

int WidgetControl::listing_clean() {
  QDir dir("../data/SNR30_int16");
  QStringList filters;
  vec_clean.clear();
  vec_clean.resize(n_L);

  filters << "*.wav" << "*.WAV";

  QStringList wavFiles = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

  for (int i = 0; i < wavFiles.size(); ++i) {
    // e.g. IBMSignal_SNR30_L1_S1.wav
    QString fileName = wavFiles[i];
    QStringList tokens = fileName.split("_");
    QString token = tokens[2];
    int L = token.mid(1).toInt(); // L1 -> 1

    vec_clean[L - 1].push_back(std::make_pair(fileName, dir.absoluteFilePath(fileName)));
  }

  //Verification
  for (int i = 0; i < n_L; ++i) {
    if (vec_clean[i].size() != n_S) {
      qDebug() << "Error: L" << (i + 1) << "has" << vec_clean[i].size() << "files.";
    }
  }

  return vec_clean.size();
  
}

int WidgetControl::listing_noise() {
  QDir dir("../data/stationary-noise");
  QStringList filters;
  vec_noise.clear();

  filters << "*.wav" << "*.WAV";

  // QDir::Files : Files only, QDir::NoDotAndDotDot : . .. exept
  QStringList wavFiles = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

  for (int i = 0; i < wavFiles.size(); ++i) {
    vec_noise.push_back(std::make_pair(wavFiles[i], dir.absoluteFilePath(wavFiles[i])));
    combo_noise.addItem(wavFiles[i]);
  }

  return wavFiles.size();
}

void WidgetControl::select_clean_L(int idx) {
  label_clean_cnt.setText("1/" + QString::number(n_S));
  btn_next.setText("Next");
  cur_l = idx;

  //shuffle vector
  std::random_device rd;
  std::mt19937 generator(rd());
  std::shuffle(vec_clean[idx].begin(), vec_clean[idx].end(), generator);


  // init answer
  sum_answer = 0;
  sum_correct = 0;

  select_S(0);
}

void WidgetControl::select_S(int _idx_s) {
  idx_s = _idx_s;

  // Get actual S number from filename
  cur_s = -1;
  QString cur_path = vec_clean[cur_l][idx_s].first;
  QRegularExpression re("_S(\\d+)");
  QRegularExpressionMatch match = re.match(cur_path);

  if (match.hasMatch()) {
    QString numStr = match.captured(1);
    cur_s = numStr.toInt();
  }

  label_clean.setText(cur_path);
  path_clean = vec_clean[cur_l][idx_s].second;
  label_text.setText(script[cur_l][cur_s-1]);
  printf("WidgetControl::setlect_s::select[%d][%d]\n", cur_l,cur_s-1);
  label_clean_cnt.setText(QString::number(idx_s+1) + "/" + QString::number(n_S));
  this->ApplyPath();
}

void WidgetControl::update_answer() {

  vec_btn_answer.clear();

  QStringList tokens = label_text.text().split(' ', Qt::SkipEmptyParts);

  for (const QString& token : tokens) {
    auto btn = std::make_unique<QPushButton>();
    btn.get()->setText(token);
    btn.get()->setCheckable(true);
    layout_answer.addWidget(btn.get());

    QPushButton* btn_ptr = btn.get();

    btn_ptr->setStyleSheet(R"(
      QPushButton {
          background-color: #d0d7de;   
          color: #2c3e50;              
          border: 1px solid #a0a6ab;   
          border-radius: 8px;          
          padding: 10px 18px;          
          font-size: 24pt;             
          font-weight: 500;            
      }
    )");

    QObject::connect(btn_ptr, &QPushButton::toggled, this, [&,btn_ptr](bool checked) {
      if (checked) {
        btn_ptr->setStyleSheet(R"(
          QPushButton {
            background-color: #78e975;   
            color: #2c3e50;              
            border: 1px solid #a0a6ab;   
            border-radius: 8px;          
            padding: 10px 18px;          
            font-size: 24pt;             
            font-weight: 50
          })");
        this->sum_correct++;
        this->update_score();
      }
      else {
        btn_ptr->setStyleSheet(R"(
          QPushButton {
            background-color: #d0d7de;   
            color: #2c3e50;              
            border: 1px solid #a0a6ab;   
            border-radius: 8px;          
            padding: 10px 18px;          
            font-size: 24pt;             
            font-weight: 500;            
        }
    )");
        this->sum_correct--;
        this->update_score();
      }
      });
    vec_btn_answer.push_back(std::move(btn));
  }

  sum_answer += tokens.size();
}






void WidgetControl::EnableControl(bool flag) {
  for (auto& btn : vec_btn_answer) {
    btn->setEnabled(flag);
  }
  btn_select_all.setEnabled(flag);
  btn_next.setEnabled(flag);
  combo_L.setEnabled(!flag);
  //combo_noise.setEnabled(flag);
  //btn_tmp_clean.setEnabled(flag);
  //btn_play.setEnabled(flag);
  btn_tune.setEnabled(!flag);
  //btn_dB_clean_down.setEnabled(flag);
  //btn_dB_clean_up.setEnabled(flag);
  //btn_dB_noise1_down.setEnabled(flag);
  //btn_dB_noise1_up.setEnabled(flag);
}


void WidgetControl::ApplyPath() {
  printf("WidgetControl::ApplyPath()\n");
  if (player) {
    printf("WidgetControl::ApplyPath()::%s %s\n",path_clean.toStdString().c_str(), path_noise.toStdString().c_str());
    player->SetCleanPath(path_clean);
    player->SetNoisePath(path_noise);
  }
  else {
  printf("WidgetControl::ApplyPath()::player is not initialized\n");
  }


}

void WidgetControl::load_script() {
  std::ifstream file("../data/K-HINT_script.csv");

  std::string line;
  int L = 0;
  int S = 0;
  std::getline(file, line); // skip header
  while (std::getline(file, line)) {
    std::vector<std::string> row;
    std::stringstream ss(line);
    std::string field;

    while (std::getline(ss, field, ',')) {
      // Skip index
      if (S == 0) {
        S++;
        continue;
      }
      script[L][S-1] = QString::fromStdString(field);
      S++;
    }
    S = 0;
    L++;
  }

  for (int l = 0; l < n_L; l++) {
    for (int s = 0; s < n_S; s++) {
      std::cout << script[l][s].toStdString() << ",";
    }
    std::cout << std::endl;
  }

  file.close();

}

void WidgetControl::update_score() {
  label_score.setText("Score: " + QString::number(sum_correct) + "/" + QString::number(sum_answer) + " [" + QString::number(int(sum_correct /(float)sum_answer * 100)) + "%]");
}

void WidgetControl::slot_clean_radio_clicked(QAbstractButton* btn) {
  if (player) {
    if (btn == &radio_clean_device1_ch1) {
      player->SetCleanIndex(0);
    }
    else if (btn == &radio_clean_device1_ch2) {
      player->SetCleanIndex(1);
    }
  }
}

void WidgetControl::slot_noise_radio_clicked(QAbstractButton* btn) {
  if (player) {
    if (btn == &radio_noise_device1_ch1) {
      player->SetNoiseIndex(0);
    }
    else if (btn == &radio_noise_device1_ch2) {
      player->SetNoiseIndex(1);
    }
  }
}

void WidgetControl::SLotUpdateDevice1Name(QString name) {
    label_device_1.setText("Device1 : " + name);
}


