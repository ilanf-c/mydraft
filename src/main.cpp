#include "MainWindow.hpp"
#include <QApplication>
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include <qlistview.h>
#include <stdio.h>
#include <stdlib.h>

#define ElementType int

#define ERROR 1e5

// typedef enum { push, pop, inject, eject, end } Operation;

typedef struct Node *PtrToNode;

struct Node {

  ElementType Element;

  PtrToNode Next, Last;
};

typedef struct DequeRecord *Deque;

struct DequeRecord {

  PtrToNode Front, Rear;
};

Deque CreateDeque();

int Push(ElementType X, Deque D);

ElementType Pop(Deque D);

int Inject(ElementType X, Deque D);

ElementType Eject(Deque D);
int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  // Create a file system model
  QFileSystemModel *model = new QFileSystemModel;
  model->setRootPath(QDir::currentPath()); // Set the current path as root path

  // Create a tree view and a list view
  QTreeView *treeView = new QTreeView;
  QListView *listView = new QListView;

  // Set the model for the tree view and list view
  treeView->setModel(model);
  listView->setModel(model);

  // Set the root index for views
  treeView->setRootIndex(model->index(QDir::currentPath()));
  listView->setRootIndex(model->index(QDir::currentPath()));

  // Create a splitter to hold the tree view and list view
  QSplitter *splitter = new QSplitter;
  splitter->addWidget(treeView);
  splitter->addWidget(listView);
  // Set the window title with the current directory name
  splitter->setWindowTitle(
      QString("File Explorer - %1").arg(QDir::currentPath()));
  
  splitter->show();

  // MainWindow w;
  // splitter->setParent(&w);
  // w.show();
  return app.exec();

  return 0;
} // end main

Deque CreateDeque() {
  Deque D = (Deque)malloc(sizeof(struct DequeRecord) + 1);
  PtrToNode t = (PtrToNode)malloc(sizeof(struct Node));
  t->Element = ERROR;
  t->Next = NULL;
  t->Last = NULL;
  D->Rear = D->Front = t;
  return D;
} // end CreateDeque

int Push(ElementType X, Deque D) {
  // insert item X on the front end of deque D
  PtrToNode t = (PtrToNode)malloc(sizeof(struct Node) + 1);
  if (t == NULL) { // Push failed
    return 0;
  } // end if
  t->Last = NULL;
  t->Element = X;
  if (D->Rear == D->Front) { // deque is empty
    t->Next = D->Front;
    D->Front->Last = t;
    D->Front = t;
    D->Rear = t->Next;
  } else {
    t->Next = D->Front;
    D->Front->Last = t;
    D->Front = t;
  }         // end if-else
  return 1; // Push success
} // end Push

ElementType Pop(Deque D) {
  // remove the front item from the deque D and return it
  if (D->Front == D->Rear) {
    return ERROR; // nothing to pop
  }               // end if
  // get the return value
  PtrToNode t = D->Front;
  ElementType res = t->Element;

  // remove the front item
  D->Front->Next->Last = NULL;
  D->Front = D->Front->Next;
  t->Next = NULL;

  return res;
} // end Pop

int Inject(ElementType X, Deque D) {
  // Insert item X on the rear end of deque D
  PtrToNode t = (PtrToNode)malloc(sizeof(struct Node) + 1);
  if (t == NULL) {
    return 0; // Inject fail
  }           // end if
  t->Next = NULL;
  t->Element = X;
  if (D->Rear == D->Front) {
    t->Last = D->Rear;
    D->Rear->Next = t;
    D->Rear = t;
    D->Front = t->Last;
  } else {
    t->Last = D->Rear;
    D->Rear->Next = t;
    D->Rear = t;
  }         // end if-else
  return 1; // Inject success
} // end Inject

ElementType Eject(Deque D) {
  // Remove the rear item from deque D and return it
  if ((D->Front == D->Rear) || (D->Front->Element == ERROR)) {
    return ERROR; // nothing to Eject
  }               // end if
  // get the return value
  PtrToNode t = D->Rear;
  ElementType res = t->Element;

  // remove the rear item
  D->Rear->Last->Next = NULL;
  D->Rear = D->Rear->Last;
  t->Last = NULL;

  return res;
} // end Eject