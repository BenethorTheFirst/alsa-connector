#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>
#include "MainWindow.hpp"

int main(int argc, char** argv)
{
  QApplication a(argc, argv);
  MainWindow* w = new MainWindow();
  w->show();
  return a.exec();
}
