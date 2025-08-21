#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDir>
#include <QStringList>
#include <QLineEdit>
#include <QGroupBox>

#include <vector>
using std::vector;

#include <utility>
using std::pair;

#include "Player.h"

class WidgetControl : public QWidget{
  Q_OBJECT

private:

  Player* player = nullptr;

  QVBoxLayout layout_main;

  
  QHBoxLayout layout_toolbar;
  QLabel label_L;
  QComboBox combo_L;
  QLabel label_clean_pre;
  QLabel label_clean;
  QLabel label_noise;
  QComboBox combo_noise;
  QPushButton btn_play;
  QPushButton btn_tmp_clean;
  vector<pair<QString, QString>> vec_noise;
  int listing_noise();
  void init_toolbar();


  QHBoxLayout layout_two_col;
  
  QVBoxLayout layout_left;
  QHBoxLayout layout_text;
  QLabel label_text;
  QString text;
  QPushButton btn_select_all, btn_next;
  void init_text();

  
  QHBoxLayout layout_answer;
  vector<std::unique_ptr<QPushButton>> vec_btn_answer;
  void update_answer();

  QFrame frame_line;
  
  QGroupBox group_score;
  QVBoxLayout layout_score;
  QLabel label_score;
  QLabel label_remain;
  void init_left();

  QGroupBox group_dB;
  QVBoxLayout layout_right;
  QHBoxLayout layout_SNR; 
  QLabel label_SNR;
  QLabel label_dB;
  bool flag_play_tune = false;

  QHBoxLayout layout_dB_clean;
  QLabel label_dB_clean;
  QPushButton btn_dB_clean_up, btn_dB_clean_down;
  QLabel label_dB_noise1;
  QHBoxLayout layout_dB_noise1;
  QPushButton btn_dB_noise1_up, btn_dB_noise1_down;
  double dB_clean = 0.0;
  double dB_noise1 = 0.0;

  QHBoxLayout layout_tune;
  QPushButton btn_tune;
  void init_right();

public:
  WidgetControl();
  ~WidgetControl();

  void ConnectPlayer(Player* p);
  void EnableControl(bool flag);

signals :
  void Play(QString path);


};

