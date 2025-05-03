#include "MainWindow.hpp"
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>

#define ElementType int

#define ERROR 1e5

typedef enum { push, pop, inject, eject, end } Operation;

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
  MainWindow w;
  w.show();
  return app.exec();

  // printf("Deque Test\n");
  // Deque D = CreateDeque();
  // Operation op;
  // ElementType X;
  // while (1) {
  //   scanf("%d", &op);
  //   if (op == end)
  //     break;
  //   switch (op) {
  //   case push:
  //     scanf("%d", &X);
  //     Push(X, D);
  //     break;
  //   case pop:
  //     printf("%d\n", Pop(D));
  //     break;
  //   case inject:
  //     scanf("%d", &X);
  //     Inject(X, D);
  //     break;
  //   case eject:
  //     printf("%d\n", Eject(D));
  //     break;
  //   case end:
  //     break;
  //   default:
  //     printf("Invalid operation\n");
  //     break;
  //   }
  // }
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