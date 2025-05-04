#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include <QToolButton>
#include <QTreeView>
#include <qabstractitemmodel.h>

class FileExplorer : public QSplitter {
  Q_OBJECT
public:
  explicit FileExplorer(QWidget *parent = nullptr) : QSplitter(parent) {

    // Set up the file system model
    setupFileTree();

    // Set up the tab widget
    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);

    addWidget(treeView);
    addWidget(tabWidget);
    setSizes({250, 550});

    // Connect signals and slots
    connect(treeView, &QTreeView::clicked, this, &FileExplorer::openFile);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this,
            &FileExplorer::closeTab);
  }

private slots:
  void openFile(const QModelIndex &index) {
    if (model->isDir(index))
      return;

    const QString path = model->filePath(index);
    if (auto *editor = findEditor(path)) {
      tabWidget->setCurrentWidget(editor); // Switch to the existing tab
    } else {
      createNewTab(path); // Create a new tab for the file
    }
  }

  void saveFile(const QModelIndex &index) {
    
  }

  void closeTab(int index) {
    QWidget *tab = tabWidget->widget(index);
    tabWidget->removeTab(index);
    openFiles.remove(tab);
    delete tab;
  }

private:
  QFileSystemModel *model;
  QTreeView *treeView;
  QTabWidget *tabWidget;
  QHash<QString, QTextEdit *> openFiles; // Store opened files with their editors

  void setupFileTree() {
    // Set up the file system model
    model = new QFileSystemModel(this);
    // show hidden files
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden |
                     QDir::System);
    model->setRootPath(QDir::currentPath());
    model->setNameFilterDisables(false);

    // left tree view
    treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setRootIndex(model->index(QDir::currentPath()));
    treeView->setHeaderHidden(true);
    for (int i = 1; i < model->columnCount(); i++) {
      treeView->hideColumn(i); // hide all columns except the first one(Name)
    }
  }

  QTextEdit* findEditor(const QString &path) {
    return openFiles.value(QFileInfo(path).canonicalFilePath(), nullptr);
  }

  void createNewTab(const QString &path) {
    QFileInfo fi(path);
    const QString canonicalPath = fi.canonicalFilePath();

    if (auto *editor = new QTextEdit; !openFiles.contains(canonicalPath)) {
      if (loadFileContent(path, editor)) {
        // Add the editor to the tab widget
        int tabIndex = tabWidget->addTab(editor, fi.fileName());
        tabWidget->setCurrentIndex(tabIndex);
        // add * before tab name for if there is unsaved changes
        connect(editor, &QTextEdit::textChanged, this,
                [this, tabIndex]() {
                  QString tabName = tabWidget->tabText(tabIndex);
                  if (!tabName.startsWith("*")) {
                    tabWidget->setTabText(tabIndex, "*" + tabName);
                  }
                });

        // Add the close button to the tab
        QToolButton *closeButton = new QToolButton;
        closeButton->setIcon(
            QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
        tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide,
                                          closeButton);
        connect(closeButton, &QToolButton::clicked, this,
                [this, tabIndex]() { closeTab(tabIndex); });
        
      }
    }
  }

  bool loadFileContent(const QString &path, QTextEdit *editor) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QMessageBox::warning(this, tr("Error"),
                           tr("Cannot open file: %1").arg(path) +
                               file.errorString());
      return false;
    }
    editor->setPlainText(file.readAll());
    file.close();
    return true;
  }
};