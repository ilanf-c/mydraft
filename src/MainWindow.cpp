#include "MainWindow.hpp"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  connectSignals();
}

MainWindow::~MainWindow() {
  // Destructor implementation
}
