#include <bits/stdc++.h>

using namespace std;



class record{
    public:
        record()=delete;
        record(int,int);
        bool containsP(int);
        virtual ~record();

    protected:

    private:
        int recordSize;
        int pKey;

};

record::record(int n, int p){
    recordSize=n;
    pKey=p;
}

bool record::containsP(int p){
    if (pKey==p){
        return true;
    }
    return false;
}

record::~record(){
    //dtor
}

//////////////////////////////////////////////////////////////////////////////////////

class page{
    public:
        page()=delete;
        page(int);
        bool setPrev(page*);
        page* getPrev();
        bool setNext(page*);
        page* getNext();
        bool enoughSpace(int);
        int searchInPage(int);
        bool insertARecord(int,int);
        int getNumber();
        virtual ~page();

    protected:

    private:
        page* prev;
        page* next;
        int countRecords;
        vector <int> directory;
        vector <record*> records;
        int freeSpace;

};

page::page(int n){
    prev=nullptr;
    next=nullptr;
    countRecords=0;
    freeSpace = n - 16;
}

bool page::setPrev(page* p){
    prev=p;
    return true;
}

page* page::getPrev(){
    return (prev);
}

bool page::setNext(page* p){
    next=p;
    return true;
}

page* page::getNext(){
    return (next);
}

bool page::enoughSpace(int n){
    if (freeSpace>=n+4){
        return true;
    }
    return false;
}

int page::searchInPage(int p){
    int i;
    for(i=0;i<records.size();i++){
        if (records[i]->containsP(p)){
            return i;
        }
    }
    return -1;
}

bool page::insertARecord(int n, int p){
    //cout<<"inserting into correct page\n";
    record * r;
    r = new record(n,p);
    records.push_back(r);
    freeSpace=freeSpace-4-n;
    countRecords++;
    //cout<<"count of records is "<<countRecords<<'\n';
    return true;
}

int page::getNumber(){
    return countRecords;
}

page::~page(){
    //dtor
}

////////////////////////////////////////////////////////////////////////////////////////

class heapFile{
    public:
        heapFile()=delete;
        heapFile(int);
        bool insertRecord(int, int);
        void searchInHeapFile(int);
        void printStatus();
        virtual ~heapFile();

    protected:

    private:
        page* root;
        int pageSize;
        int countPages;

};

heapFile::heapFile(int n){
    pageSize=n;
    root=nullptr;
    countPages=0;
}

bool heapFile::insertRecord(int n, int p){
    page* ptr = root;
    page* ptr1;
    while(1){
        if (ptr==nullptr){
            // insert new page
            // point ptr to that page
            //cout<<"inserting new page\n";
            page* np = new page(pageSize);
            countPages++;
            ptr=np;
            if (root==nullptr){
                root=np;
            }
            else{
                ptr1->setNext(np);
                np->setPrev(ptr1);
            }
            break;
        }
        else{
            // check if record can fit in that page
            if (ptr->enoughSpace(n)){
                break;
            }
            else{
                ptr1=ptr;
                ptr=ptr->getNext();
            }
        }
    }

    // now ptr is pointing to the page in which has enough space to insert the record
    ptr->insertARecord(n,p);
    return true;
}

void heapFile::searchInHeapFile(int p){
    page* ptr = root;
    int t;
    int m = 0;

    while(1){
        if (ptr==nullptr){
            cout<<-1<<" "<<-1<<'\n';
            break;
        }
        t = ptr->searchInPage(p);
        if (t==-1){
            ptr=ptr->getNext();
        }
        else{
            cout<<m<<" "<<t<<'\n';
            break;
        }
        m++;
    }
}

void heapFile::printStatus(){
    cout<<countPages<<" ";
    int i;
    page* ptr = root;

    while(1){
        //cout<<"inside the loop\n";
        if (ptr==nullptr){
            break;
        }
        i = ptr->getNumber();
        cout<<i<<" ";
        ptr=ptr->getNext();
    }
    cout<<'\n';
}

heapFile::~heapFile(){
    //dtor
}



int main(){

    int n;
    cin>>n;
    heapFile* h;
    h = new heapFile(n);

    int a,b,c;

    while(1){
        cin>>a;
        if (a==1){
            cin>>b>>c;
            h->insertRecord(b,c);
        }
        else if (a==2){
            h->printStatus();
        }
        else if (a==3){
            cin>>b;
            h->searchInHeapFile(b);
        }
        else{
            break;
        }
    }

    return 0;
}