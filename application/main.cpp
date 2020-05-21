// Copyright Lebedev Alexander 2020
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    
  QApplication app(argc, argv);  
    
  Window window;
 
  window.resize(1280, 720);
  window.setWindowTitle("App");
  window.show();
 
  return app.exec();
}
