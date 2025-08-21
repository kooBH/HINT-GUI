#include "Player.h"


Player::Player() {
  buf_in = new short[in_channels * n_buf];
  buf_noise1 = new short[in_channels * n_buf];
  buf_tune = new short[in_channels * n_buf];

  flag_noise1_play.store(false);
  flag_tune_play.store(false);

}

Player::~Player() {
  delete[] buf_in;
  delete[] buf_noise1;
  delete[] buf_tune;
  Off();
}

int Player::On() {
  int ret = 0;
  if (stream_clean == nullptr) {
      stream_clean = new RtOutput(device_clean, out_channels, sr_in, sr_out, n_buf, n_buf);
      stream_noise1 = new RtOutput(device_noise1, out_channels, sr_in, sr_out, n_buf, n_buf);
      stream_noise2 = new RtOutput(device_noise2, out_channels, sr_in, sr_out, n_buf, n_buf);
      printf("==Output Stream Open==\n");
      printf("device_clean  : %d\n", device_clean);
      printf("device_noise1 : %d\n", device_noise1);
      printf("device_noise2 : %d\n", device_noise2);
      printf("out channels  : %d\n", out_channels);

      ret += stream_clean->PrepStream();
      ret += stream_noise1->PrepStream();
      ret += stream_noise2->PrepStream();

      if (ret) {
        printf("ERROR::Player::On::Failed to Open Stream\n");
        delete stream_clean;
        delete stream_noise1;
        delete stream_noise2;
        stream_clean = nullptr;
        stream_noise1 = nullptr;
        stream_noise2 = nullptr;
        return - 1;
      }

      stream_clean->Start();
      stream_noise1->Start();
      stream_noise2->Start();
    }
  else {
    printf("Player::Off::already running.\n");
    return -1;
  }
}

void Player::Off() {

  if (stream_clean) {
    stream_clean->Stop();
    stream_noise1->Stop();
    stream_noise2->Stop();

    stream_clean->Wait();
    stream_noise1->Wait();
    stream_noise2->Wait();

    delete stream_clean;
    delete stream_noise1;
    delete stream_noise2;

    stream_clean = nullptr;
    stream_noise1 = nullptr;
    stream_noise2 = nullptr;
  }
  else {
    printf("Player::Off::Not running.\n");
  }

}

void Player::PlayClean(){

  std::thread t([&]() {
    wav_input = new WAV();
    printf("Player::Play(%s)\n", path_clean.toStdString().c_str());
    wav_input->OpenFile(path_clean.toStdString());

    wav_input->Print();


    while (!wav_input->IsEOF()) {
      wav_input->ReadUnit(buf_in, n_buf * in_channels);

      for (int i = 0; i < n_buf * in_channels; i++) {
        buf_in[i] = static_cast<short>(buf_in[i] * scale_noise1);
        if(buf_in[i] > 32768)
          buf_in[i] = 32768;
        else if (buf_in[i] < -32768)
          buf_in[i] = -32768;

      }

      stream_clean->AppendQueue(buf_in);
    }

    delete wav_input;
  });
  t.detach();
}

void Player::PlayNoise1() {

  if (flag_noise1_play.load()) {
    flag_noise1_play.store(false);
    return;
  }

  std::thread t([&]() {
    wav_noise1 = new WAV();
    wav_noise1->OpenFile(path_noise1.toStdString().c_str());

    wav_noise1->Print();

    printf("Player::PlayNoise1(%s)\n", path_noise1.toStdString().c_str());
    flag_noise1_play.store(true);

    while (flag_noise1_play.load()) {
      wav_noise1->ReadUnit(buf_noise1, n_buf * in_channels);
      

      for (int i = 0; i < n_buf * in_channels; i++) {
        buf_noise1[i] = static_cast<short>(buf_noise1[i] * scale_noise1);
        if(buf_noise1[i] > 32768)
          buf_noise1[i] = 32768;
        else if (buf_noise1[i] < -32768)
          buf_noise1[i] = -32768;
      }

      stream_noise1->AppendQueue(buf_noise1);

      while(stream_noise1->data.queue.size() > 3 && flag_noise1_play.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

      if (wav_noise1->IsEOF())
        wav_noise1->Rewind();
    }

    delete wav_noise1;
    });
    t.detach();
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

    while (flag_tune_play.load()) {

      for (int i = 0; i < n_buf; i++) {
        buf_tune[i] = amplitude * std::sin(2.0 * M_PI * freq * (double)(t++) / sr);
        if (t == int(sr))t = 0;
      }
      stream_clean->AppendQueue(buf_tune);
      stream_noise1->AppendQueue(buf_tune);

      while (stream_noise1->data.queue.size() > 3 && flag_tune_play.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    });
  t.detach();


}

void Player::SetCleanPath(QString path) {
  printf("Player::SetCleanPath(%s)\n", path.toStdString().c_str());
  path_clean = path;
}

void Player::SetNoise1Path(QString path) {
  printf("Player::SetNoise1Path(%s)\n",path.toStdString().c_str());
  path_noise1 = path;
}

void Player::SetdBClean(double dB) {
  dB_clean = dB;
  scale_clean = std::pow(10.0f, dB / 20.0f);

}
void Player::SetdBNoise1(double dB) {
  dB_noise1 = dB;
  scale_noise1 = std::pow(10.0f, dB / 20.0f);
}
