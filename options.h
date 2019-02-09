#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <string>
#include <stack.h>
#include <string.h>
using namespace std ;

class options : public QObject
{
    Q_OBJECT

private :
    node* first = NULL;
    node* last = NULL ;
    node* cursor = NULL ;
    node* copy = NULL;
    Stack undo ;
    Stack redo ;
public:
    doublylist(string text);
    void cursorSet(node *place);
    void textMiner(string data);    //mining the text
    void inserter(char val);       //insert in doubly list
    node *insetAfter(node* place,char val);
    node *nodeConstructor(node *next, node *prev, char val);
    void freeNode(node* place);
    void Redo();
    void Undo();
    void shiftLeft();
    void shiftRight();
    void nodePositioner(node*place ,node* pos);
    void Copy(int number, node *Node);
    char* minedText(node *first);
    void print(node* first);
    int size(node *first);
    void clean();
    node* clear(node *first);
    node *lastNode(node* Node);
    int copyAnalyzer(char *, unsigned int, unsigned int);
    void paste();
    node* listCpy(node*);
    char* stringToChar(string text) ;

signals:
    void siftedText(QString text);
public slots:
    QString textSifter(QString text) ;

};

#endif // OPTIONS_H
