#include "Player.h"


Player::Player() {
  buf_out = new short[out_channels * n_buf];
  buf_clean = new short[in_channels * n_buf];
  buf_noise = new short[in_channels * n_buf];

  flag_play.store(false);
  flag_tune_play.store(false);
  flag_clean_play.store(false);
  flag_noise_play.store(false);

  req_clean.store(false);
  req_noise.store(false);
}

Player::~Player() {
  delete[] buf_out;
  delete[] buf_clean;
  delete[] buf_noise;

  Off();
}

int Player::On() {
  printf("Player::On()\n");
  int ret = 0;
  if (stream_1 == nullptr) {
      stream_1= new RtOutput(device_1, out_channels, sr_in, sr_out, n_buf, n_buf);
      printf("==Output Stream Open==\n");
      printf("device : %d\n", device_1);
      printf("out channels  : %d\n", out_channels);

      ret = stream_1->PrepStream();

      if (ret) {
        printf("ERROR::Player::On::Failed to Open Stream\n");
        delete stream_1;
        stream_1= nullptr;
        return - 1;
      }

      stream_1->Start();
  }
  else {
    printf("Player::Off::already running.\n");
    return -1;
  }

  //this->Play();
}

void Player::Off() {
  if (stream_1) {
    flag_clean_play.store(false);
    flag_tune_play.store(false);
    flag_noise_play.store(false);

    stream_1->Stop();
    stream_1->Wait();
    delete stream_1;
    stream_1= nullptr;
  }
  else {
    printf("Player::Off::Not running.\n");
  }
}

void Player::InsertClean() {
  printf("Player::InsertClean()\n");
  if (flag_clean_play.load())
    flag_clean_play.store(false);

  while (flag_clean_running.test_and_set(std::memory_order_acquire))
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  std::thread t([&]() {
    WAV wav_input;
    wav_input.OpenFile(path_clean.toStdString());
    wav_input.Print();

    flag_clean_play.store(true);
    n_req.fetch_add(1);
    printf("Player::InsertClean()::Start\n");

    while (!wav_input.IsEOF() && flag_clean_play.load()) {
      if (req_clean.load()) {
        wav_input.ReadUnit(buf_clean, n_buf * in_channels);

        while (flag_buffer.test_and_set(std::memory_order_acquire));
        for (int i = 0; i < n_buf * in_channels; i++)
          buf_out[i * out_channels + clean_channel_idx] += static_cast<short>(buf_clean[i] * scale_clean);
        req_clean.store(false);
        flag_buffer.clear(std::memory_order_release);
      }
      else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }
    flag_clean_running.clear(std::memory_order_release);
    n_req.fetch_sub(1);
    printf("Player::InsertClean()::Done()\n");
    });
  t.detach();
}

void Player::LoopNoise() {
    printf("Player::LoopNoise()\n");

  if(flag_noise_play.load())
    flag_noise_play.store(false);

  while (flag_noise_running.test_and_set(std::memory_order_acquire))
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  std::thread t([&]() {
    WAV wav_noise;
    wav_noise.OpenFile(path_noise.toStdString());
    wav_noise.Print();

    flag_noise_play.store(true);
    n_req.fetch_add(1);
    printf("Player::LoopNoise()::Start\n");

    while (flag_noise_play.load()) {
      if (req_noise.load()) {
        wav_noise.ReadUnit(buf_noise, n_buf * in_channels);

        while (flag_buffer.test_and_set(std::memory_order_acquire));
        for (int i = 0; i < n_buf * in_channels; i++)
          buf_out[i * out_channels + noise_channel_idx] += static_cast<short>(buf_noise[i] * scale_noise);
        req_noise.store(false);
        flag_buffer.clear(std::memory_order_release);
      }
      else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
      if (wav_noise.IsEOF())
        wav_noise.Rewind();
    }
    flag_noise_running.clear(std::memory_order_release);
    n_req.fetch_sub(1);
    printf("Player::LoopNoise()::Done()\n");
  });
  t.detach();

}

void Player::Play(){

  if (flag_play.load()) {
    flag_play.store(false);
  }

  while (flag_play_running.test_and_set(std::memory_order_acquire))
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  std::thread t([&]() {
    printf("Player::Play()\n");
    flag_play.store(true);
    req_clean.store(true);
    req_noise.store(true);

    while (flag_play.load()) {
      if (stream_1->data.queue.size() < 3) {
        while (flag_buffer.test_and_set(std::memory_order_acquire));
        stream_1->AppendQueue(buf_out);
        memset(buf_out, 0, sizeof(short) * out_channels * n_buf);
        req_clean.store(true);
        req_noise.store(true);
        flag_buffer.clear(std::memory_order_release);
      }
      else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }

    flag_play_running.clear(std::memory_order_release);
    printf("Player::Play()::Done\n");

  });
  t.detach();
}

void Player::Stop() {
  flag_play.store(false);
  flag_clean_play.store(false);
  flag_noise_play.store(false);
}

void Player::PlayTune() {

  if (flag_tune_play.load()) {
    flag_tune_play.store(false);
    return;
  }

  std::thread t([&]() {
    printf("Player::PlayTune()\n");
    flag_tune_play.store(true);

    double sr = 16000.0;
    double freq = 500.0;
    int amplitude = 16000;
    int t=0;
    double val;

    while (flag_tune_play.load()) {


      if (req_clean.load()) {

        while (flag_buffer.test_and_set(std::memory_order_acquire));
        for (int i = 0; i < n_buf; i++) {
          val = amplitude * std::sin(2.0 * M_PI * freq * (double)(t++) / sr);
          for (int j = 0; j < out_channels; j++) {
            buf_out[i * out_channels + j] = val;
          }
          if (t == int(sr))t = 0;
        }
        req_clean.store(false);
        flag_buffer.clear(std::memory_order_release);
      }
      else {

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }

    });
  t.detach();


}

void Player::SetCleanPath(QString path) {
  printf("Player::SetCleanPath(%s)\n", path.toStdString().c_str());
  path_clean = path;
}

void Player::SetNoisePath(QString path) {
  printf("Player::SetNoisePath(%s)\n",path.toStdString().c_str());
  path_noise = path;
}

void Player::SetdBClean(double dB) {
  dB_clean = dB;
  scale_clean = std::pow(10.0f, dB / 20.0f);

}
void Player::SetdBNoise1(double dB) {
  dB_noise = dB;
  scale_noise = std::pow(10.0f, dB / 20.0f);
}


void Player::SetCleanIndex(int idx) {
  clean_channel_idx = idx;
}

void Player::SetNoiseIndex(int idx) {
  noise_channel_idx = idx;
}

void Player::SetTuneIndex(int idx) {
  clean_channel_idx = idx;

}

bool Player::IsStreamPlaying() {
  return flag_play.load();
}

bool Player::IsNoisePlaying() {
  return flag_noise_play.load();
}
