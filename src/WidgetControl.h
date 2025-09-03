#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDir>
#include <QStringList>
#include <QLineEdit>
#include <QGroupBox>

#include <random>
#include <vector>
using std::vector;

#include <utility>
using std::pair;

#include <fstream>
#include <iostream>

#include "Player.h"

class WidgetControl : public QWidget{
  Q_OBJECT

private:
  Player* player = nullptr;
  QVBoxLayout layout_main;
  
  //--- Toolbar & file selection ---//
  QHBoxLayout layout_toolbar;
  QLabel label_L;
  QComboBox combo_L;
  QLabel label_clean_pre;
  QLabel label_clean;
  QLabel label_clean_cnt;
  QLabel label_noise;
  QComboBox combo_noise;
  QPushButton btn_play;
  QPushButton btn_tmp_clean;


  // functionals for managing files 
  // L<S<filename,path>>
  const static int n_L = 14;
  const static int n_S = 20;
  int cur_l=0;
  int cur_s=0;
  int idx_s=0;
  vector<vector<pair<QString,QString>>> vec_clean;
  vector<pair<QString, QString>> vec_noise;

  int listing_clean();
  void select_clean_L(int idx);
  void select_S(int idx_S);
  int listing_noise();
  void init_toolbar();

  QString path_clean;
  QString path_noise;
  QString script[n_L][n_S];
  void load_script();
  
  //--- Answers & Score ---//
  QHBoxLayout layout_text;
  QLabel label_text;
  QString text;
  QPushButton btn_select_all, btn_next;
  void init_text();
  void update_answer();
  
  QHBoxLayout layout_answer;
  vector<std::unique_ptr<QPushButton>> vec_btn_answer;

  // dual column layout here
  QHBoxLayout layout_two_col;

  QVBoxLayout layout_left;
  QFrame frame_line;
  
  QGroupBox group_score;
  QVBoxLayout layout_score;
  QLabel label_score;
  void init_left();

  // functionals for Answers & Score
  int sum_answer = 0;
  int sum_correct = 0;
  void update_score();

  //--- Player related ---//
  QVBoxLayout layout_right;
  QGroupBox group_dB;
  QVBoxLayout layout_dB;
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
  void init_dB();

  //
  QGroupBox group_device;
  QVBoxLayout layout_device;
  QLabel label_device_1;
  QLabel label_device_2;
  QHBoxLayout layout_device_clean;
  QLabel label_device_clean;
  QButtonGroup btng_clean;
  QRadioButton radio_clean_device1_ch1, radio_clean_device1_ch2;

  QHBoxLayout layout_device_noise;
  QLabel label_device_noise;
  QButtonGroup btng_noise;
  QRadioButton radio_noise_device1_ch2, radio_noise_device1_ch1, radio_noise_device2_ch12;

  QHBoxLayout layout_device_tune;
  QLabel label_device_tune;
  QButtonGroup btng_tune;
  QRadioButton radio_tune_device1_ch1, radio_tune_device1_ch2;

  void init_device();
  void slot_clean_radio_clicked(QAbstractButton *btn);
  void slot_noise_radio_clicked(QAbstractButton *btn);




public:
  WidgetControl();
  ~WidgetControl();

  void ConnectPlayer(Player* p);
  void EnableControl(bool flag);
  void ApplyPath();

signals :
  void Play(QString path);


};

