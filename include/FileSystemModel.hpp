#pragma once // Ensures the header file is included only once during compilation
#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel {
  Q_OBJECT
public:
  explicit FileSystemModel(QObject *parent = nullptr)
      : QFileSystemModel(parent) {
    setFilter(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden |
              QDir::System);
    setNameFilterDisables(false);
    setRootPath(QDir::currentPath());
  }
};