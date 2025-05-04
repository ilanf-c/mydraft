#pragma once // Ensures the header file is included only once during compilation
#include <QAbstractListModel>
#include <QFile>
#include <QFileInfo>
#include <QModelIndex>
#include <QVector>
#include <QtConcurrent/QtConcurrent>

class DocumentModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum Roles {
    FilePathRole =
        Qt::UserRole + 1, // Qt::UserRole + 1 is used to ensure custom roles do
                          // not overlap with Qt's predefined roles.
    ModifiedRole,
    ContentRole
    /* add new role */
  };

  explicit DocumentModel(QObject *parent = nullptr)
      : QAbstractListModel(parent) {}

  int rowCount(const QModelIndex &parent = QModelIndex()) const override {
    Q_UNUSED(parent);
    return m_documents.size();
  }

  QVariant data(const QModelIndex &index, int role) const override {
    if (!index.isValid())
      return QVariant();

    const Document *doc = m_documents.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
      return doc->fileName(); // Display the file name
    case FilePathRole:
      return doc->filePath(); // Return the file path
    case ModifiedRole:
      return doc->isModified(); // Return the modified status
    case ContentRole:
      return doc->getContent(); // Return the content of the document
                                /* add new action of role*/
    }
    return QVariant();
  }

  QHash<int, QByteArray> roleNames() const override {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[FilePathRole] = "filePath";
    roles[ModifiedRole] = "modified";
    roles[ContentRole] = "content";
    /* register new role */
    return roles;
  }

  void setModified(int index, bool modified) {
    if (index < 0 || index >= m_documents.size())
      return;

    m_documents[index]->modified = modified;
    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {ModifiedRole});
  }

  void openDocument(const QString &path) {
    if (findDocument(path) != -1)
      return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_documents.append(new Document(path));
    endInsertRows();
  }

  void closeDocument(int index) {
    if (index < 0 || index >= m_documents.size())
      return;

    beginRemoveRows(QModelIndex(), index, index);
    m_documents.removeAt(index);
    endRemoveRows();
  }

private:
  struct Document {
    QString path;
    QString content;
    bool modified;
    Document(const QString &path) : path(path), modified(false) {
      QFile file(path);
      if (file.open(QIODevice::ReadOnly)) {
        content = QString::fromUtf8(file.readAll());
      }
    }

    QString fileName() const { return QFileInfo(path).fileName(); }
    QString filePath() const { return QFileInfo(path).canonicalFilePath(); }
    void setModified(bool modified) { this->modified = modified; }
    bool isModified() const { return modified; }
    QString getContent() const { return content; }
  };

  int findDocument(const QString &path) const {
    const QString canonicalPath = QFileInfo(path).canonicalFilePath();
    for (int i = 0; i < m_documents.size(); i++) {
      if (QFileInfo(m_documents[i]->path).canonicalFilePath() ==
          canonicalPath) {
        return i;
      }
    }
    return -1;
  }

  QVector<Document *> m_documents;
};