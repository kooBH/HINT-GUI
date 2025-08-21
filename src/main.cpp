#include <QApplication>
#include "app.h"

int main(int argc, char*argv[]){
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
      #ifdef NDEBUG
      //	ShowWindow(::GetConsoleWindow(), SW_HIDE);
        ShowWindow(::GetConsoleWindow(), SW_SHOW);
      #endif
#endif

	QCoreApplication::addLibraryPath(".");
	QApplication App(argc, argv);

	app  main;
	App.setApplicationName(QString::fromLocal8Bit("HINT Programm - 개발중"));

	main.setStyleSheet(R"(
    QWidget {
        background-color: #f4f6f9;
        font-family: 'Segoe UI', 'Noto Sans', 'Apple SD Gothic Neo';
        font-size: 11pt;
        color: #2c3e50;
    }

  QPushButton {
      background-color: #ecf0f1;      /* 밝은 회색 */
      color: #2c3e50;                 /* 진한 글씨색 */
      border: 1px solid #bdc3c7;      /* 얇은 테두리 */
      border-radius: 6px;
      padding: 6px 12px;
  }

  QPushButton:hover {
      background-color: #dfe6e9;      /* Hover 시 살짝 진한 회색 */
  }

  QPushButton:pressed {
      background-color: #b2bec3;      /* 눌렸을 때 좀 더 진하게 */
  }

  QPushButton:disabled {
      background-color: #f5f6fa;
      color: #95a5a6;
      border: 1px solid #dcdde1;
  }
    QLineEdit, QTextEdit, QPlainTextEdit {
        background-color: white;
        border: 1px solid #bdc3c7;
        border-radius: 4px;
        padding: 4px;
    }
    QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
        border: 1px solid #3498db;
        box-shadow: 0 0 4px #3498db;
    }

    QLabel {
        color: #2c3e50;
    }

    QCheckBox, QRadioButton {
        spacing: 6px;
    }
    QCheckBox::indicator, QRadioButton::indicator {
        width: 16px;
        height: 16px;
    }
    QCheckBox::indicator:checked {
        image: url(:/icons/check.png);
    }

    QComboBox {
        background-color: white;
        border: 1px solid #bdc3c7;
        border-radius: 4px;
        padding: 4px 8px;
    }
    QComboBox:focus {
        border: 1px solid #3498db;
    }
    QComboBox::drop-down {
        border: none;
    }

    QScrollBar:vertical {
        background: transparent;
        width: 10px;
        margin: 0px;
    }
    QScrollBar::handle:vertical {
        background: #95a5a6;
        border-radius: 5px;
        min-height: 20px;
    }
    QScrollBar::handle:vertical:hover {
        background: #7f8c8d;
    }

    QGroupBox {
        border: 1px solid #dcdde1;
        border-radius: 6px;
        margin-top: 10px;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 10px;
        padding: 0 4px;
        color: #34495e;
        font-weight: bold;
    }
  )");
	main.show();
	
	return App.exec();
}