#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

class N {
public:
    int data;
    N* left;
    N* right;
    N(int data) 
    {
        this->data = data;
        this->left = NULL;
        this->right = NULL;
    }
};

class BT
{
public:
    N* root;

    BT() {
        root = NULL;
    }
    void insert(int data) {
        root = insertNode(root, data);
    }
    bool search(int data) {
        return searchValue(root, data);
    }

    N* insertNode(N* root, int data) {
        if (root == NULL) {
            root = new N(data);
            return root;
        }
        if (data < root->data) {
            root->left = insertNode(root->left, data);
        }
        else if (data > root->data) {
            root->right = insertNode(root->right, data);
        }
        return root;
    }
    bool searchValue(N* root, int data)
    {
        if (root == NULL) {
            return false;
        }
        else if (root->data == data)
        {
            return true;
        }
        else if (data < root->data)
        {
            #pragma omp task shared(root)
            {
                return searchValue(root->left, data);
            }
        }
        else 
        {
            #pragma omp task shared(root)
            {
                return searchValue(root->right, data);
            }
        }
    #pragma omp taskwait   
    }
};

int main()
{
    srand(time(NULL));
    int nodes = 30;
    
    BT t;

    #pragma omp parallel for num_threads(3)
    for (int i = 0; i < nodes; i++)
    {
        int data = rand() % (100);
    #pragma omp critical
        t.insert(data);
    }
    int x = rand() % (100);
    cout << "Searching for: " << x << endl;
    if (t.search(x)) 
    {
        cout << x << " found" << endl;
    }
    else
    {
        cout << x << " not found." << endl;
    }

    return 0;
}