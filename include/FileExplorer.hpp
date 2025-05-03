#include <QDebug>
#include <QDir>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QTreeView>

class FileExplorer : public QSplitter {
  Q_OBJECT
public:
  explicit FileExplorer(QWidget *parent = nullptr) : QSplitter(parent) {
    // Set up the file system model
    model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());

    // left tree view
    treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setRootIndex(model->index(QDir::currentPath()));
    treeView->setHeaderHidden(true);
    for(int i = 1; i < model->columnCount(); i++) {
      treeView->hideColumn(i); // hide all columns except the first one(Name)
    }

    // right text edit
    textEdit = new QTextEdit(this);
    
    // Set up the splitter
    addWidget(treeView);
    addWidget(textEdit);
    setSizes({200,600});

    // Connect the tree view selection signal to the slot
    connect(treeView, &QTreeView::clicked, this,
            &FileExplorer::onFileSelected);
  }

private slots:
  void onFileSelected(const QModelIndex &index) {
    if (model->isDir(index))
      return; // Ignore directory selection

    const QString filePath = model->filePath(index);
    openFile(filePath);
  }

private:
  QFileSystemModel *model;
  QTreeView *treeView;
  QTextEdit *textEdit;

  void openFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QMessageBox::warning(this, "Error",
                           "Could not open file" + file.errorString());
      return;
    }
    QTextStream stream(&file);
    textEdit->clear();
    textEdit->setPlainText(stream.readAll());
    file.close();

    textEdit->setWindowTitle(filePath);
  }
};