#pragma once
#include "DocumentModel.hpp"
#include "FileSystemModel.hpp"
#include <QDir>
#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QTextEdit>
#include <QTreeView>
#include <QWidget>
#include <qnamespace.h>

class MainWindow : public QMainWindow {
  Q_OBJECT            // have to include this macro to use signals and slots
      private slots : // Uncomment this if you want to use private slots

                      public : MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  QSplitter *m_splitter;
  QTreeView *m_fileView;
  QTabWidget *m_docView;
  FileSystemModel *m_fsModel;
  DocumentModel *m_docModel;

  void setupUI() {
    m_fsModel = new FileSystemModel(this);
    m_docModel = new DocumentModel(this);

    m_fileView = new QTreeView;
    m_fileView->setModel(m_fsModel);
    m_fileView->setRootIndex(m_fsModel->index(QDir::currentPath()));
    m_fileView->setHeaderHidden(true);
    m_fileView->setColumnHidden(1, true); // Hide the second column

    m_docView = new QTabWidget;
    m_docView->setTabsClosable(true);
    m_docView->setMovable(true);

    m_splitter = new QSplitter;
    m_splitter->addWidget(m_fileView);
    m_splitter->addWidget(m_docView);
    setCentralWidget(m_splitter);
  }

  void connectSignals() {
    connect(m_fileView, &QTreeView::doubleClicked,
            [this](const QModelIndex &index) {
              if (m_fsModel->isDir(index))
                return;
              openFile(m_fsModel->filePath(index));
            });

    connect(m_docView, &QTabWidget::tabCloseRequested, [this](int index) {
      m_docModel->closeDocument(index);
      m_docView->removeTab(index);
    });
  }

  void openFile(const QString &path) {
    m_docModel->openDocument(path);
    const int index = m_docModel->rowCount() - 1;

    QTextEdit *editor = new QTextEdit;

    connect(editor, &QTextEdit::textChanged, [this, index]() {
      m_docModel->setModified(index, true);
      m_docView->setTabText(
          index,
          "*" + m_docModel->data(m_docModel->index(index), Qt::DisplayRole)
                    .toString());
    });

    editor->setPlainText(
        m_docModel->data(m_docModel->index(index), DocumentModel::ContentRole)
            .toString());

    m_docView->addTab(
        editor,
        m_docModel->data(m_docModel->index(index), Qt::DisplayRole).toString());
  }
};