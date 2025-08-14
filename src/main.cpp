#include <QApplication>
#include "app.h"

int main(int argc, char*argv[]){
#ifdef _WIN32
      #ifdef NDEBUG
      //	ShowWindow(::GetConsoleWindow(), SW_HIDE);
        ShowWindow(::GetConsoleWindow(), SW_SHOW);
      #endif
#endif

	QCoreApplication::addLibraryPath(".");
	QApplication App(argc, argv);

	app  main;
	main.show();
	
	return App.exec();
}