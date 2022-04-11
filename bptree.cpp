#include<bits/stdc++.h>

using namespace std;

// Tree node
class node
{
    private:
        int* key, size, capacity;
        node** nodeptr;
        bool is_data;
        node* parent;
    friend class bptree;
    public:
        node(int k, bool ye);
        ~node();
};

node::node(int k, bool ye)
{
    if(ye)
    {
        is_data = true;
        capacity = 2*k;
    }
    else
    {
        is_data = false;
        capacity = 2*k + 1;
    }
    key = new int[capacity];
    nodeptr = new node*[capacity+1];
    size = 0;
    parent = nullptr;
}

node::~node()
{
    delete[] key;
    delete[] nodeptr;
}

class bptree
{
    private:
        int index_count, data_count, t, d;
        node* root;
        void indexInsert(int val, node* cur, node* newnode);
    public:
        bptree(int, int);
        ~bptree();
        node* getRoot();
        int numData();
        int numIndex();
        bool insert(int val);
        bool search(int val);
        void printRoot();
};

bptree::bptree(int tval, int dval)
{
    t = tval;
    d = dval;
    root = nullptr;
    data_count = 0;
    index_count = 0;
}

int bptree::numData()
{
    return data_count;
}

int bptree::numIndex()
{
    return index_count;
}

bool bptree::search(int val)
{
    if(root == nullptr)
    {
        return false;
    }
    node* cur = root;
    // cur becomes a data node now
    while(cur->is_data == false)
    {
        for(int i=0; i<cur->size;i++)
        {
            if(val < cur->key[i])
            {
                cur = cur->nodeptr[i];
                break;
            }
            if(i == cur->size - 1)
            {
                cur = cur->nodeptr[i+1];
                break;
            }
        }
    }
    
    for(int i=0; i<cur->size; i++)
    {
        if(val == cur->key[i])
        {
            return true;
        }
    }
    return false;
}

bool bptree::insert(int val)
{
    if(search(val))
    {
        return false;
    }
    if(root == nullptr)
    {
        root = new node(d, true);
        data_count ++;
        root->key[0] = val;
        root->size = 1;
    }
    node* cur = root;
    while(cur->is_data == false)
    {
        for(int i=0; i<cur->size;i++)
        {
            if(val < cur->key[i])
            {
                cur = cur->nodeptr[i];
                break;
            }
            if(i == cur->size - 1)
            {
                cur = cur->nodeptr[i+1];
                break;
            }
        }
    }

    if(cur->size < 2*d)
    {   
        int i;
        for(i=0; i<cur->size; i++)
        {
            if(cur->key[i] > val)
            {
                break;
            }
        }
        for(int j = cur->size -1; j>i; j--)
        {
            cur->key[j] = cur->key[j-1];
        }
        cur->key[i] = val;
        cur->size++;
        cur->nodeptr[cur->size] = cur->nodeptr[cur->size-1];
        cur->nodeptr[cur->size-1] = nullptr;
    }
    else
    {
        node* newdata = new node(d, true);
        data_count++;
        int keybuf[(2*d)+1];
        for (int i = 0; i < 2*d; i++) 
        {
            keybuf[i] = cur->key[i];
        }
        int i = 0, j;
 
        for(i=0; i< 2*d; i++)
        {
            if(cur->key[i] > val)
            {
                break;
            }
        }

        // Update the current virtual
        // Node to its previous
        for (j = 2*d + 1;j > i; j--) 
        {
            keybuf[j] = keybuf[j - 1];
        }

        keybuf[i] = val;

        cur->size = d;
        newdata->size = d+1;

        cur->nodeptr[cur->size] = newdata;


        newdata->nodeptr[newdata->size] = cur->nodeptr[2*d];
        newdata->nodeptr[0] = cur;
        newdata->parent = cur->parent;

        cur->nodeptr[2*d] = NULL;

        // Update the current virtual
        // Node's key to its previous
        for (i = 0;i < cur->size; i++) 
        {
            cur->key[i] = keybuf[i];
        }

        // Update the newLeaf key to
        // virtual Node
        
        for (i = 0,j = cur->size; i < newdata->size; i++, j++) 
        {
            newdata->key[i] = keybuf[j];
        }

        if(cur == root)
        {
            // Create a new Node
            node* newRoot = new node(t, false);
            index_count++;

            // Update rest field of
            // B+ Tree Node
            newRoot->key[0] = newdata->key[0];
            newRoot->nodeptr[0] = cur;
            newRoot->nodeptr[1] = newdata;
            newRoot->size = 1;
            root = newRoot;
            cur->parent = newRoot;
            newdata->parent = newRoot;
        }
        else
        {
     
           indexInsert(newdata->key[0],cur->parent,newdata);
        }
    }
    return true;
}

void bptree::indexInsert(int val, node* cur, node* child)
{
    if (cur->size < 2*t+1) 
    {
        int i = 0;

        for(i=0; i<cur->size; i++)
        {
            if(cur->key[i] > val)
            {
                break;
            }
        }
        
        for (int j = cur->size; j > i; j--) 
        {
            cur->key[j] = cur->key[j - 1];
        }
 
        for (int j = cur->size + 1; j > i + 1; j--) 
        {
            cur->nodeptr[j] = cur->nodeptr[j - 1];
        }
 
        cur->key[i] = val;
        cur->size++;
        cur->nodeptr[i + 1] = child;
    }
    // For overflow, break the node
    else {
 
        // For new Interval
        node* newInternal = new node(t, false);
        index_count++;
        newInternal->parent = cur->parent;
        int virtualKey[2*t + 2];
        node* virtualPtr[2*t + 3];
 
        for (int i = 0; i < 2*t+1; i++) 
        {
            virtualKey[i] = cur->key[i];
        }
        
        for (int i = 0; i < 2*t + 2; i++) 
        {
            virtualPtr[i] = cur->nodeptr[i];
        }
 
        int i = 0, j;
 
        // Traverse to find where the new
        // node is to be inserted
        for(i=0; i<2*t + 1; i++)
        {
            if(cur->key[i] > val)
            {
                break;
            }
        }
 
        // Traverse the virtualKey node
        // and update the current key
        // to its previous node key
        for (int j = 2*t + 2; j > i; j--) 
        {
            virtualKey[j] = virtualKey[j - 1];
        }
 
        virtualKey[i] = val;

        for (int j = 2*t + 3; j > i + 1; j--) 
        {
            virtualPtr[j] = virtualPtr[j - 1];
        }
 
        virtualPtr[i + 1] = child;
 
        cur->size = t;
 
        newInternal->size = t+1;
 
        // Insert new node as an
        // internal node
        for (i = 0, j = cur->size + 1; i < newInternal->size; i++, j++) 
        {
            newInternal->key[i] = virtualKey[j];
        }
 
        for (i = 0, j = cur->size + 1; i < newInternal->size + 1; i++, j++) 
        {
            newInternal->nodeptr[i] = virtualPtr[j];
        }
 
        // If cursor is the root node
        if (cur == root) 
        {
            // Create a new root node
            node* newRoot = new node(t, false);
            index_count++;
 
            // Update key value
            newRoot->key[0] = cur->key[cur->size];
 
            // Update rest field of
            // B+ Tree Node
            newRoot->nodeptr[0] = cur;
            newRoot->nodeptr[1] = newInternal;
            newRoot->size = 1;
            root = newRoot;
            cur->parent = newRoot;
            newInternal->parent = newRoot;
        }
 
        else {
            indexInsert(cur->key[cur->size], cur->parent, newInternal);
        }
    }
}

void bptree::printRoot()
{
    for(int i=0; i<root->size; i++)
    {
        cout << root->key[i]<<' ';
    }
    cout << '\n';
}

int main()
{
    int t, d;
    cin >> d >> t;
    bptree* tree = new bptree(t, d);
 
    // Create B+ Tree
    tree->insert(6);
    tree->insert(16);
    tree->insert(26);
    tree->insert(36);
    tree->insert(46);
 
    // Function Call to search node
    // with value 16
    if(tree->search(16))
        cout << "Found\n";
    else
        cout << "Not Found\n";
    
    cout << tree->numData() << ' ' << tree->numIndex() << '\n';
    tree->printRoot();


    return 0;
}