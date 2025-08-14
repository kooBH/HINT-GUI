
#include "KPlotStreamer.h"



KPlotStreamer::KPlotStreamer(int _width,int _height, int _n_hop,int _n_disp):
  stream_wav_noisy(_width,_height,_n_hop,_n_disp),
  stream_wav_estim(_width,_height,_n_hop,_n_disp),
  //stream_spec_noisy(_width,_height, 512),
  //stream_spec_estim(_width,_height, 512)
  stream_spec_noisy(282,150, 2048),
  stream_spec_estim(282,150, 2048)
{
  width = _width;
  height = _height;
  n_hop = _n_hop;
  n_disp = _n_disp;

  setLayout(&layout_main);

  layout_main.addWidget(&stream_wav_noisy, 0, 0, Qt::AlignLeft);
  layout_main.addWidget(&stream_wav_estim, 0, 1, Qt::AlignLeft);
  layout_main.addWidget(&stream_spec_noisy, 1, 0, Qt::AlignLeft);
  layout_main.addWidget(&stream_spec_estim, 1, 1, Qt::AlignLeft);

  buffer = new short*[2];
  for (int i = 0; i < 2; i++) {
    buffer[i] = new short[n_hop];
  }

}

KPlotStreamer::~KPlotStreamer() {
  for (int i = 0; i < 2; i++)
    delete[] buffer[i];
  delete[] buffer;
}

void KPlotStreamer::resizeEvent(QResizeEvent* e) {
  int newWidth = e->size().width();
  int newHeight = e->size().height();
 // printf("KPlotStreamer::resizeEvent | %d %d <- %d %d\n",e->size().width(), e->size().height(), e->oldSize().width(), e->oldSize().height());

  QSize newSize(int(newWidth/2 - 50),int(newHeight/2 - 50));
  stream_wav_noisy.resizeStream(newSize);
  stream_wav_estim.resizeStream(newSize);
  stream_spec_noisy.resizeStream(newSize);
  stream_spec_estim.resizeStream(newSize);
}

void KPlotStreamer::slot_update(short* buf) {
  for (int i = 0; i < n_hop; i++) {
    buffer[0][i] = buf[2*i];
    buffer[1][i] = buf[2*i + 1];
  }

  stream_wav_noisy.Stream(buffer[0]);
  stream_wav_estim.Stream(buffer[1]);
  stream_spec_noisy.Stream(buffer[0]);
  stream_spec_estim.Stream(buffer[1]);

}

void KPlotStreamer::SetColormapRange(int c_max, int c_min) {
  stream_spec_noisy.color_max = c_max;
  stream_spec_noisy.color_min = c_min;
  stream_spec_estim.color_max = c_max;
  stream_spec_estim.color_min = c_min;
}
