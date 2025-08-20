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
	App.setApplicationName(QString::fromLocal8Bit("HINT Programm - °³¹ßÁß"));
	main.show();
	
	return App.exec();
}