#include "options.h"
#include <QtDebug>

QString options::textSifter(QString text)
{
    clean();
    textMiner(text.toStdString());
    char* txt = options::minedText(this->first);

    return QString(txt) ;

}

char* options::stringToChar(string text)
{
    char *CharedStr = new char[ text.length() + 1 ] ; // Getting Char-space From Heap
    strcpy(CharedStr , text.c_str()) ;              //Copying Data of two strings
    return CharedStr ;
}

void options::textMiner(string data)
{
    char* text = options::stringToChar(data);

    for(unsigned int i=0 ; i<data.size() ; i++)
    {
        if(text[i] == 'R' && cursor && cursor->next )
        {
            shiftRight();
        }
        else if(text[i] == 'L' && cursor )
        {
            shiftLeft();
        }
        else if(text[i] == 'U')
        {
            Undo();
        }
        else if(text[i] == 'Y' )
        {
            Redo();
        }
        else if(text[i] == 'C')
        {
            i = copyAnalyzer(text,i,data.size());
        }
        else if(text[i] == 'P' && this->copy)
        {
            paste();
        }
        else if(text[i]>='a' && text[i]<='z')
        {
            inserter(text[i]);
            redo.clean();
        }
    }
}

void options::inserter(char val)
{
    node* first = this->first ;
    if(!first)
    {
        first = nodeConstructor(NULL,NULL,val);
        undo.Push(first);   //pushing a copy of nodes in undo stack
        this->first = first ;
        this->last = first ;

        cursorSet(first);
        this->cursor->next = NULL;
        this->cursor->prev = NULL;

        return ;
    }
    else
    {
        int guard = 1 ; if(!cursor) guard = 0 ;

        node* Node = insetAfter(this->cursor,val) ;
        undo.Push(Node);
        if(guard && cursor) cursorSet(cursor->next) ;
    }
}

node* options::insetAfter(node *place, char val)
{
    if(!place)
    {
        node* newNode = nodeConstructor(this->first,NULL,val);
        cursorSet(newNode) ;
        if(this->first)
            this->first->prev = newNode;
        this->first = newNode;
        return newNode;
    }
    else if(!place->next)   // if cursor was the last node
    {
        node* newNode = nodeConstructor(NULL,place,val) ;
        place->next = newNode ;
        this->last = newNode ;
        return newNode;
    }
    else
    {
        node* newNode = nodeConstructor(place->next,place,val) ;
        if(place->next)
            place->next->prev = newNode;
        place->next = newNode ;
        return newNode;
    }
}

node *options::nodeConstructor(node*next , node* prev,char val)
{
    node* newNode = new node ;
    newNode->next = next ;
    newNode->prev = prev ;
    newNode->val = val ;
    return newNode ;
}

void options::freeNode(node *place)
{
    if(place && place->val=='L')
    {
        if(!cursor)
            cursor = first ;
        else
            cursorSet(cursor->next) ;
        return;
    }
    else if(place && place->val=='R')
    {
        if(cursor)
            cursorSet(cursor->prev) ;
        return ;
    }
    else if(place && place->val>='0' && place->val<='9')
    {
        int num = place->val-'0';
        node* _num = redo.Pop();
        for(int i=0 ; i<num ; i++)
            Undo();
        redo.Push(_num);
        return;
    }


    node* list = this->first ;

    while(list)
    {
        if(list == place)
        {
            if(list->prev == NULL)
            {
                first = first->next ;
                if(first)
                    first->prev = NULL ;
                cursorSet(NULL);
            }
            else
            {
                node* prev = list->prev;
                node* next = list->next;
                prev->next = next;
                if(next)
                    next->prev = prev ;

                if(cursor && cursor->val == list->val)
                    cursorSet(list->prev);

            }
            break ;
        }
        list = list->next ;
    }
}

void options::Redo()
{
    if(!redo.first)
        return;

    node* pos = redo.first->pos;
    node* list = this->first;

    if(pos->val == 'R')
    {
        if(cursor && cursor->next)
            cursorSet(cursor->next) ;
        undo.Push(redo.Pop());
        cout<<" 1."<<endl;
        return ;
    }
    else if(pos->val == 'L')
    {
        if(cursor)
            cursorSet(cursor->prev) ;
        undo.Push(redo.Pop());
        cout<<" 2."<<endl;
        return ;
    }
    else if(pos->val >='0' && pos->val<='9')
    {
        int num = pos->val - '0';
        node* _num = redo.Pop();
        for(int i=0; i<num ; i++)
            Redo();
        undo.Push(_num);

        return ;
    }
    else if(!pos->prev)
    {
        pos->next = this->first;
        if(this->first)
            this->first->prev = pos;
        this->first = pos ;

        if(!cursor)
            this->cursor = this->first;
        undo.Push(redo.Pop());
        cout<<" 3."<<cursor->val<<endl;
        return;
    }
    else
    {
        cout<<" 4."<<cursor->val<<endl;
        while(list)
        {
            if(pos && list == pos->prev)
            {
                nodePositioner(list,pos);
                break ;
            }
            list = list->next;
        }
        undo.Push(redo.Pop());
    }
}

void options::Undo()
{
    node* Node = undo.Pop();
    redo.Push(Node);
    freeNode(Node);
}

void options::shiftLeft()
{
    //cout<<endl<<"L : "<<cursor->val<<endl;
    cursorSet(cursor->prev) ;
    //cout<<endl<<"L : "<<cursor->val<<endl;
    node* Node = nodeConstructor(NULL,NULL,'L');
    undo.Push(Node);
}

void options::shiftRight()
{
    cursorSet(cursor->next) ;
    node* Node = nodeConstructor(NULL,NULL,'R');
    undo.Push(Node);
}

void options::cursorSet(node *place)
{
    this->cursor = place ;
}

void options::nodePositioner(node *place, node *pos)
{
    pos->next = place->next;
    if(place->next)
    {
        place->next->prev = pos ;
    }
    pos->prev = place ;
    place->next = pos ;
    cursorSet(pos);
}

void options::Copy(int number,node* Node)
{
    if(copy)
        copy = clear(copy);
    if(!Node)
        return;

    for(int i=0 ; Node && i<number ; i++)
    {
        if(!copy)
            copy = nodeConstructor(NULL,Node->prev,Node->val);
        else
            copy = nodeConstructor(copy,NULL,Node->val);

        Node=Node->prev;
    }
}

char *options::minedText(node* first)
{
    int Size = size(first);
    if(!Size)
        return '\0';

    char* text = new char[Size+1];
    node* f = first;
    for(int i=0 ; f ; i++)
    {
        text[i] = f->val ;
        text[i+1] = '\0';
        f = f->next ;
    }

    return text ;
}

void options::print(node *first)
{
    while(first)
    {
        cout<<first->val;
        first=first->next;
    }
    cout<<endl ;
}

int options::size(node* first)
{
    int size = 0;
    node* f = first;
    while(f)
    {
        size++ ;
        f = f->next ;
    }
    return size;
}

void options::clean()
{

    clear(first);
    clear(copy);

    // node* first = this->first;
    //    while(first)
    //    {
    //        node* next = first->next;
    //        delete first;
    //        first = next ;
    //    }


    this->first = NULL;
    this->last = NULL ;
    this->cursor = NULL ;
    this->copy = NULL;
    undo.clean();
    redo.clean();
}

node *options::clear(node* first)
{
    while(first)
    {
        node* next = first->next;
        delete first;
        first = next ;
    }

    return NULL ;
}

node *options::lastNode(node *Node)
{
    while(Node && Node->next)
    {
        Node = Node->next;
    }
    return Node ;
}

int options::copyAnalyzer(char *text,unsigned int i , unsigned int size)
{
    if(i+1<size && text[i+1]=='L')
    {
        unsigned int c = 0 , j=0;
        for( j=i+1 ; j<= size && c<i && text[j]=='L' ;j++,c++);

        Copy(c,lastNode(this->first));

        return j-1 ;
    }
    else if(i+1<size && text[i+1]<='9' && text[i+1]>='0')
    {
        node* num = NULL;
        node* last = NULL;

        i++;
        for(;i+1<=size && text[i]<='9' && text[i]>='0';i++)
        {
            if(!num)
            {
                num = nodeConstructor(NULL,NULL,text[i]);
                last = num ;
            }
            else
            {
                last->next = nodeConstructor(NULL,num,text[i]);
                last = last->next;
            }
        }

        int number =atoi(minedText(num));
        Copy(number,lastNode(this->first));
        return i-1 ;
    }
    return i ;
}

void options::paste()
{

    node* listFirst = listCpy(this->copy);
    node* firstCpy = listFirst;
    int number = 0 ;

    while(firstCpy)
    {
        undo.Push(firstCpy);
        firstCpy = firstCpy->next;
        number++;
    }

    char SNum = number+'0';
    node* _Node = nodeConstructor(NULL,NULL,SNum);
    undo.Push(_Node);

    if(cursor)
    {
        node* last = lastNode(listFirst);
        last->next = cursor->next;
        if(cursor->next)
            cursor->next->prev = last ;

        cursor->next = listFirst;
        listFirst->prev = cursor ;
        cursor = last;
    }
    else
    {

        node* last = lastNode(listFirst);
        last->next = this->first;
        if(this->first->next)
            this->first->next->prev = last ;

        this->first = listFirst;
        listFirst->prev = cursor ;
        cursor = last;
    }
}

node *options::listCpy(node *first)
{
    node* cpy = first ;
    node* list = NULL ;
    node* listFirst = NULL;

    while(cpy)
    {
        if(!listFirst)
        {
            list = nodeConstructor(NULL,NULL,cpy->val);
            listFirst = list ;
        }
        else
        {
            list->next = nodeConstructor(NULL,list,cpy->val);

            list = list->next;
        }
        cpy = cpy->next;
    }
    list = NULL ;

    return listFirst;
}


