#include "MainWindow.hpp"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Deque Test");
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  QLabel *label = new QLabel("Deque Test", this);
  label->setAlignment(Qt::AlignCenter);
  QTextEdit *edit = new QTextEdit(this);
  edit->setPlaceholderText("Enter a number");
  layout->addWidget(edit);
  layout->addWidget(label);
  QPushButton *pushButton = new QPushButton("Push", this);
  QPushButton *popButton = new QPushButton("Pop", this);
  QPushButton *injectButton = new QPushButton("Inject", this);
  QPushButton *ejectButton = new QPushButton("Eject", this);
  layout->addWidget(pushButton);
  layout->addWidget(popButton);
  layout->addWidget(injectButton);
  layout->addWidget(ejectButton);
  connect(pushButton, &QPushButton::clicked, this,
          &MainWindow::onPushButtonClicked);
  resize(800, 600);
}

void MainWindow::onPushButtonClicked() {
  // Handle push button click
  QLabel *label = findChild<QLabel *>();
  if (label) {
    label->setText("Push operation clicked");
  }
  QTextEdit *edit = findChild<QTextEdit *>("edit");
  if (edit) {
    QString text = edit->toPlainText();
    // Handle the input text
    // For example, you can print it to the console
    qDebug() << "Input text:" << text;
  }
}

MainWindow::~MainWindow() {
  // Destructor implementation
}
