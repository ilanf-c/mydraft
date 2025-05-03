#pragma once
#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT            // have to include this macro to use signals and slots
      private slots : // Uncomment this if you want to use private slots
                      void
                      onPushButtonClicked(); // Example slot for push button

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
};