#include "app.h"

app::app():
    widget_disp(640,256,512,400)
  {
  setStyleSheet("\
			QWidget{background:rgb(235, 250, 235);}\
      \
      ");

    /* top widget */{
    layout_top.addWidget(&btn_play);
    btn_play.setText("Play");
    layout_top.addWidget(&btn_load);
    btn_load.setText("Load");

    // Param Edits
    /*
    https://doc.qt.io/qt-6/qdoublevalidator.html
    QDoubleValidator(double bottom, double top, int decimals, QObject *parent = nullptr)
    */
   
    layout_main.addLayout(&layout_top);
    layout_top.setAlignment(Qt::AlignLeft);

    QObject::connect(&btn_play, &QPushButton::pressed, this, &app::slot_btn_play);
    QObject::connect(&proc, &processor::signal_update, &widget_disp, &KPlotStreamer::slot_update);
    /* Processor */
    QObject::connect(&btn_play, &QPushButton::pressed, &proc, &processor::slot_toggle);

    QObject::connect(&btn_load, &QPushButton::clicked, [&]() {
      QString fileName;
      QFileDialog dialog;

      /* signal - slot for import button */
      fileName = dialog.getOpenFileName(this,
        tr("Open Wav File"), ".", tr("something (*.wav)"));
      if (!fileName.isEmpty()){
        emit(signal_load(fileName));
        setProcParam();
       }
      }
    );
    QObject::connect(this, &app::signal_load, &proc, &processor::slot_load);
  }

  /* main widget */{
    widget_main.setStyleSheet("\
			QWidget{background:rgb(220, 250, 220);}\
      \
      ");

    widget_main.addTab(&widget_disp, "display");


    widget_main.addTab(&widget_config, "parameters");
    widget_config.setStyleSheet("\
			QWidget{background:rgb(255,255,255);}\
			QLineEdit{background:rgb(238, 238, 238);}\
			QComboBox{background:rgb(238, 238, 238);}\
      \
      ");
    

    widget_config.Add("Input/Output", "../config/io.json");
    widget_config.Add("colormap", "../config/display.json");
    layout_main.addWidget(&widget_main);
  }

  setMinimumSize(640,480);
  setLayout(&layout_main);

  // Apply param
};

app::~app() {

};

void app::slot_btn_play() {
  // Stop
  if (is_playing) {
    btn_play.setText("Play");
  }
  // Start
  else {
    setProcParam();
    btn_play.setText("Stop");
  }
  is_playing = !is_playing;
  std::cout << is_playing << std::endl;
}


 double app::get(QString a, string b) {
  double val;
   json j = widget_config[a.toStdString()][b]["value"];
   switch (j.type()) {
    //case json::value_t::object :
      //break;
    case json::value_t::boolean :
      return static_cast<double>(j.get<bool>());
    case json::value_t::number_float :
      return j.get<double>();
    case json::value_t::number_unsigned :
      return static_cast<double>(j.get<int>());
    case json::value_t::number_integer :
      return static_cast<double>(j.get<int>());
    default:
      printf("ERROR::unsupported json type:%d\n",j.type());
    }
 }

 void app::setProcParam() {
   proc.device_in = static_cast<int>(get("Input/Output", "input_device"));
   proc.multiple = static_cast<double>(get("Input/Output", "multiple"));
   widget_disp.SetColormapRange(get("colormap", "color_max"), get("colormap", "color_min"));

 }

