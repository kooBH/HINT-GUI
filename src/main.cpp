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
	App.setApplicationName(QString::fromLocal8Bit("HINT Programm - ������"));

	main.setStyleSheet(R"(
    QWidget {
        background-color: #f4f6f9;
        font-family: 'Segoe UI', 'Noto Sans', 'Apple SD Gothic Neo';
        font-size: 11pt;
        color: #2c3e50;
    }

  QPushButton {
      background-color: #ecf0f1;      /* ���� ȸ�� */
      color: #2c3e50;                 /* ���� �۾��� */
      border: 1px solid #bdc3c7;      /* ���� �׵θ� */
      border-radius: 6px;
      padding: 6px 12px;
  }

  QPushButton:hover {
      background-color: #dfe6e9;      /* Hover �� ��¦ ���� ȸ�� */
  }

  QPushButton:pressed {
      background-color: #b2bec3;      /* ������ �� �� �� ���ϰ� */
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