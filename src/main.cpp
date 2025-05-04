#include "FileSystemModel.hpp"
#include "MainWindow.hpp"
// #include "FileExplorer.hpp"
#include <QApplication>
#include <QSplitter>
#include <qtreeview.h>

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  // Create the file explorer
  // FileExplorer *fileExplorer = new FileExplorer();

  // fileExplorer->show();

  MainWindow *mainWindow = new MainWindow();
  mainWindow->setWindowTitle("File Explorer");
  mainWindow->resize(800, 600);
  mainWindow->show();

  return app.exec();

  return 0;
} // end main


