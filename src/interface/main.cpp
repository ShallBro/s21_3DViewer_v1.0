#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  MainWindow mainWindow;

  mainWindow.setWindowTitle("3D Viewer v1.0");
  mainWindow.setGeometry(400, 200, 800, 600);
  mainWindow.show();

  return a.exec();
}
