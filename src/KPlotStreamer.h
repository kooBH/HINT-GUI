#ifndef _K_WAV_STREAMER_
#define _K_WAV_STREAMER_

#include <QGridLayout>
#include <QWidget>
#include "KWavStream.h"
#include "KSpecStream.h"
#include <QResizeEvent>


class KPlotStreamer: public  QWidget {
  Q_OBJECT

private :
  QGridLayout layout_main;
  KWavStream stream_wav_noisy;
  KWavStream stream_wav_estim;
  KSpecStream stream_spec_noisy;;
  KSpecStream stream_spec_estim;;

  int width, height;
  int n_hop;
  int n_disp;

  short** buffer;


protected :
  void resizeEvent(QResizeEvent* event);

public : 
  KPlotStreamer(int width,int height, int n_hop, int n_disp);
  ~KPlotStreamer();

  void SetColormapRange(int c_max, int c_min);

public slots : 
  void slot_update(short *data);

};

#endif