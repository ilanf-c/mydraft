#pragma once // Ensures the header file is included only once during compilation
#include "DocumentModel.hpp"
#include "FileSystemModel.hpp"
#include <QDir>
#include <QFormLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTextEdit>
#include <QTreeView>
#include <QWidget>
#include <qcontainerfwd.h>
class MainWindow : public QMainWindow {
  Q_OBJECT            // have to include this macro to use signals and slots
      private slots : // Uncomment this if you want to use private slots
                      void
                      showFileProperties(const QString &path) {
    QFileInfo fileInfo(path);
    if (!fileInfo.exists()) {
      QMessageBox::warning(this, "File Not Found",
                           "The file does not exist." + fileInfo.fileName());
      return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("File Properties - " + fileInfo.fileName());

    QFormLayout *layout = new QFormLayout(&dialog);
    auto addRow = [&](const QString &label, const QString &value) {
      layout->addRow(new QLabel(label, &dialog), new QLabel(value, &dialog));
    };

    addRow("File Path:", fileInfo.absoluteFilePath());
    addRow("File Size: ", QString("%1 bytes").arg(fileInfo.size()));
    addRow("File Type:", fileInfo.suffix());
    addRow("Last Modified:", fileInfo.lastModified().toString());
    addRow("Last Accessed:", fileInfo.lastRead().toString());
    addRow("Created:", fileInfo.birthTime().toString());
    addRow("Permission:",
           QString("%1%2%3")
               .arg(fileInfo.permission(QFile::ReadUser) ? "r" : "-")
               .arg(fileInfo.permission(QFile::WriteUser) ? "w" : "-")
               .arg(fileInfo.permission(QFile::ExeUser) ? "x" : "-"));

    dialog.exec();
  }

public:
  MainWindow(QWidget *parent = nullptr);
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

    m_fileView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_fileView, &QTreeView::customContextMenuRequested,
            [this](const QPoint &pos) {
              QModelIndex index = m_fileView->indexAt(pos);
              if (index.isValid()) {
                QMenu *menu = new QMenu;
                menu->addAction("Properties", [this, index]() {
                  showFileProperties(m_fsModel->filePath(index));
                });
                menu->exec(m_fileView->viewport()->mapToGlobal(pos));
              }
            });

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