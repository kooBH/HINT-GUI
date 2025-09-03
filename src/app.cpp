#include "app.h"

app::app(){
  font.setFamily(QStringLiteral(u"Malgun Gothic")); 
  font.setPointSize(16);
  this->setFont(font);

  /* main widget */{
    widget_main.addTab(&widget_control, "Control");
    widget_main.addTab(&widget_config, "Device");

    widget_config.Add("Input/Output", "../config/io.json");
    layout_main.addWidget(&widget_main);
  }

  label_status.setText("Initialized");
  label_status.setAlignment(Qt::AlignRight);
  layout_main.addWidget(&label_status);

  setMinimumSize(1280,640);
  setLayout(&layout_main);

  player = new Player();
  widget_control.ConnectPlayer(player);

  RefreshPlayer();

  QObject::connect(widget_config.GetButtonApply(), &QPushButton::clicked, this, &app::RefreshPlayer);
};

app::~app() {
  delete player;
};

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
   player->device_1= static_cast<int>(get("Input/Output", "clean_speaker"));

 }

 void app::enable_control(bool flag) {
   widget_control.EnableControl(flag);
 }

 void app::RefreshPlayer() {
   player->Off();
   setProcParam();
   widget_control.ApplyPath();

   if (player->On()) {
     QMessageBox::critical(this, "Error", QString("Failed to open stream. Please check the console log."), QMessageBox::Ok);
     is_player_on = false;
     label_status.setText("Failed to open stream");
   }
   else {
     is_player_on = true;
     label_status.setText("Stream opened.");
   }
     enable_control(is_player_on);

 }

