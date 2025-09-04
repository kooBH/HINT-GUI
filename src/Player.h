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
  int out_channels = 2;

  int clean_channel_idx = 0;
  int noise_channel_idx = 1;
  int tune_channel_idx = 0;

  int sr_in = 16000;
  int sr_out = 16000;
  int n_buf = 2048;
  double multiple = 5.0;
  int cnt = 0;
  double scale_factor = 10; //UMA-8

  bool isRunning = false;

  RtOutput* stream_1 = nullptr;
  RtOutput* stream_2 = nullptr;

  short* buf_clean = nullptr;
  short* buf_noise = nullptr;
  short* buf_out = nullptr;

  QString path_clean;
  QString path_noise;

  double dB_clean = 0.0;
  double dB_noise = 0.0;

  double scale_clean = 1.0;
  double scale_noise = 1.0;

  // atomic to control on/off
  std::atomic<bool> flag_play;
  std::atomic<bool> flag_clean_play;
  std::atomic<bool> flag_noise_play;

  // atmoic to gurantee only one thread.
  std::atomic_flag flag_clean_running = ATOMIC_FLAG_INIT;
  std::atomic_flag flag_noise_running = ATOMIC_FLAG_INIT;
  std::atomic_flag flag_play_running = ATOMIC_FLAG_INIT;
  std::atomic<bool> flag_tune_play;

  // atomic to manage access to buffer
  std::atomic<int> cnt_req;
  std::atomic<int> n_req;
  std::atomic_flag flag_buffer = ATOMIC_FLAG_INIT;
  std::atomic<bool> req_clean;
  std::atomic<bool> req_noise;


public:
  int device_1 = 4;
  int device_2 = 8;
  QString device_name_1;
  QString device_name_2;

  Player();
  ~Player();

  int On();
  void Off();
  void PlayTune();

  void Play();
  void Stop();

  void SetdBClean(double dB);
  void SetdBNoise1(double dB);

  void SetCleanIndex(int idx);
  void SetNoiseIndex(int idx);
  void SetTuneIndex(int idx);

  bool IsStreamPlaying();
  bool IsNoisePlaying();

public slots :
  void SetNoisePath(QString path);
  void SetCleanPath(QString path);

  void InsertClean();
  void LoopNoise();

signals :
  void SignalUpdateDevice1Name(QString name);

};
