#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>
#include <QFileDialog>
#include <QDateTime >
#include <QLineEdit>

#include <QCheckBox>

#include <QMessageBox>

#include "WidgetConfig.h"
#include "WidgetControl.h"
#include "Player.h"

class app : public QWidget {
  Q_OBJECT

private:
  Player* player;

  QFont font;
  QVBoxLayout layout_main;
  QTabWidget widget_main;
  WidgetConfig widget_config;
  WidgetControl widget_control;

  bool is_playing = false;

  //template <typename T>
  //T get(QString, string);
  double get(QString, string);

  void setProcParam();

public:

  app();
  ~app();

public slots:
  void RefreshPlayer();


signals:
};
