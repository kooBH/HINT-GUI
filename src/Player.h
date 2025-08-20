#pragma once

#include <QObject>

#include "WAV.h"
#include "RtOutput.h"

#include <vector>
#include <atomic>
#include <thread>
#include <time.h>


using std::vector;

class Player : public QObject {
  Q_OBJECT
private:

  int in_channels = 1;
  int out_channels = 1;

  int sr_in = 16000;
  int sr_out = 16000;
  int n_buf = 2048;
  double multiple = 5.0;
  int cnt = 0;
  double scale_factor = 10; //UMA-8

  bool isRunning = false;

  RtOutput* stream_clean = nullptr;
  RtOutput* stream_noise1 = nullptr;
  RtOutput* stream_noise2 = nullptr;
  WAV* wav_input = nullptr;
  WAV* wav_noise1 = nullptr;

  short* buf_in = nullptr;
  std::atomic<bool> flag_noise1_play;
  short* buf_noise1 = nullptr;

  QString path_clean;
  QString path_noise1;


  std::atomic<bool> flag_tune_play;
  short* buf_tune = nullptr;


public:
  int device_clean = 4;
  int device_noise1 = 4;
  int device_noise2 = 4;

  Player();
  ~Player();

  int On();
  void Off();
  void PlayTune();

  void PlayClean();
  void PlayNoise1();

  void SetCleanPath(QString path);

public slots :
  void SetNoise1Path(QString path);

};
