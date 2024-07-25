// Deleting a key from a B-tree in C++

#include <iostream>
#include <queue>
#include "bigint.h"

using namespace std;

struct Data
{
    string Name;
    string Path;
    BigInt Hash;
};

class BTreeNode {

    Data* nodekeys;
    //int* nodekeys;                  // -> keys
    int minDegree;                  // -> t;
    BTreeNode** childPointers;      // -> C;
    int numkeys;                    // -> n;
    bool isLeafNode;                // -> leaf;

    /*
    string filePath;
    int  hash_int;
    string hash_string;
    */


public:

    // B-tree node
    BTreeNode(int _minDegree, bool _isLeafNode) {
        minDegree = _minDegree;
        isLeafNode = _isLeafNode;

        nodekeys = new Data[2 * minDegree - 1];
        childPointers = new BTreeNode * [2 * minDegree];

        numkeys = 0;
        
    }

    void traverse() {
        int i = 0;
        while (i < numkeys) {
            if (!isLeafNode)
                childPointers[i]->traverse();
            cout << "Key: " << nodekeys[i].Hash << "\tPath: " << nodekeys[i].Path << "\tName: " << nodekeys[i].Name << endl;
            i++;
        }
        if (!isLeafNode)
            childPointers[i]->traverse();
    }

    //  to be fixed
    bool search(string key) 
    {
        int i = 0;
        while (i < numkeys && key < nodekeys[i].Hash.value)
            ++i;

        if (i < numkeys && key == nodekeys[i].Hash.value)
        {
            cout << "\nName of File : " << nodekeys[i].Name;
            cout << "\nPath : " << nodekeys[i].Path;
            
            return true;
        }

        if (isLeafNode)
            return false;

        return childPointers[i]->search(key);
    }

    int findKey(BigInt targetKey) {
        int index;
        for (index = 0; index < numkeys && nodekeys[index].Hash < targetKey; ++index);
        return index;
    }

    // Insertion non full
    void insertNonFull(BigInt hash, string path, string name)       //target key
    {
        int i;

        if (isLeafNode == true) 
        {
            for (i = numkeys - 1; i >= 0 && nodekeys[i].Hash > hash; i--) 
            {
                nodekeys[i + 1] = nodekeys[i];
            }

            nodekeys[i + 1].Hash = hash;
            nodekeys[i + 1].Name = name;
            nodekeys[i + 1].Path = path;

            numkeys = numkeys + 1;
        }
        else 
        {
            for (i = numkeys - 1; i >= 0 && nodekeys[i].Hash > hash; i--);

            if (childPointers[i + 1]->numkeys == 2 * minDegree - 1) 
            {
                splitChild(i + 1, childPointers[i + 1]);

                if (nodekeys[i + 1].Hash < hash)
                    i++;
            }
            childPointers[i + 1]->insertNonFull(hash, path, name);
        }
    }

    // Function to create a new node and copy keys and child pointers from y
    BTreeNode* createAndCopyNode(int i, BTreeNode* y) 
    {
        BTreeNode* z = new BTreeNode(y->minDegree, y->isLeafNode);
        z->numkeys = minDegree - 1;

        for (int j = 0; j < minDegree - 1; j++)
            z->nodekeys[j] = y->nodekeys[j + minDegree];

        if (!y->isLeafNode) {
            for (int j = 0; j < minDegree; j++)
                z->childPointers[j] = y->childPointers[j + minDegree];
        }

        y->numkeys = minDegree - 1;

        return z;
    }

    // Function to insert the new node into the B-tree at position i
    void insertNode(int i, BTreeNode* y, BTreeNode* z) {
        for (int j = numkeys; j >= i + 1; j--)
            childPointers[j + 1] = childPointers[j];

        childPointers[i + 1] = z;

        for (int j = numkeys - 1; j >= i; j--)
            nodekeys[j + 1] = nodekeys[j];

        nodekeys[i] = y->nodekeys[minDegree - 1];

        numkeys = numkeys + 1;
    }

    // Split child
    void splitChild(int i, BTreeNode* y) {
        BTreeNode* z = createAndCopyNode(i, y);
        insertNode(i, y, z);
    }

    // Deletion operation
    void deletion(BigInt hash, string path, string name) {
        int targetIndex = findKey(hash);

        if (targetIndex < numkeys && nodekeys[targetIndex].Hash == hash) {
            if (isLeafNode)
                removeFromLeaf(targetIndex);
            else
                removeFromNonLeaf(targetIndex);
        }
        else {
            if (isLeafNode) {
                cout << "The file " << name << " does not exist in the tree\n";
                return;
            }

            bool isLastKey;
            if (targetIndex == numkeys)
                isLastKey = true;
            else
                isLastKey = false;

            if (childPointers[targetIndex]->numkeys < minDegree)
                fill(targetIndex);

            if (isLastKey && targetIndex > numkeys)
                childPointers[targetIndex - 1]->deletion(hash, path, name);
            else
                childPointers[targetIndex]->deletion(hash, path, name);
        }
        return;
    }

    // Remove from the if lead
    void removeFromLeaf(int index) {
        for (int i = index + 1; i < numkeys; ++i)
            nodekeys[i - 1] = nodekeys[i];

        numkeys--;

        return;
    }

    BigInt getPredecessor(int targetIndex) 
    {
        BTreeNode* currentNode = childPointers[targetIndex];
        for (; !currentNode->isLeafNode; currentNode = currentNode->childPointers[currentNode->numkeys]);

        return currentNode->nodekeys[currentNode->numkeys - 1].Hash;
    }

    BigInt getSuccessor(int targetIndex) 
    {
        BTreeNode* currentNode = childPointers[targetIndex + 1];
        for (; !currentNode->isLeafNode; currentNode = currentNode->childPointers[0]);

        return currentNode->nodekeys[0].Hash;
    }

    void removeFromNonLeaf(int index) {

        BigInt targetkey = nodekeys[index].Hash;

        if (childPointers[index]->numkeys >= minDegree) {
            BigInt pred = getPredecessor(index);
            nodekeys[index].Hash = pred;
            childPointers[index]->deletion(pred, "", ""); // Updated to match the parameters of deletion
        }

        else if (childPointers[index + 1]->numkeys >= minDegree) {
            BigInt succ = getSuccessor(index);
            nodekeys[index].Hash = succ;
            childPointers[index + 1]->deletion(succ, "", ""); // Updated to match the parameters of deletion
        }

        else {
            merge(index);
            childPointers[index]->deletion(targetkey, "", ""); // Updated to match the parameters of deletion
        }
        return;
    }

    void fill(int index) {
        if (index != 0 && childPointers[index - 1]->numkeys >= minDegree)
            borrowFromPrev(index);

        else if (index != numkeys && childPointers[index + 1]->numkeys >= minDegree)
            borrowFromNext(index);

        else {
            if (index != numkeys)
                merge(index);
            else
                merge(index - 1);
        }
        return;
    }

    void shiftChild(BTreeNode* child) {
        int i = child->numkeys - 1;
        while (i >= 0) {
            child->nodekeys[i + 1] = child->nodekeys[i];
            i--;
        }

        if (!child->isLeafNode) {
            i = child->numkeys;
            while (i >= 0) {
                child->childPointers[i + 1] = child->childPointers[i];
                i--;
            }
        }
    }

    void borrowFromPrev(int index) {
        BTreeNode* child = childPointers[index];
        BTreeNode* sibling = childPointers[index - 1];

        shiftChild(child);

        child->nodekeys[0] = nodekeys[index - 1];

        if (!child->isLeafNode)
            child->childPointers[0] = sibling->childPointers[sibling->numkeys];

        nodekeys[index - 1] = sibling->nodekeys[sibling->numkeys - 1];

        child->numkeys += 1;
        sibling->numkeys -= 1;

        return;
    }

    void shiftSibling(BTreeNode* sibling) {
        int i = 1;
        while (i < sibling->numkeys) {
            sibling->nodekeys[i - 1] = sibling->nodekeys[i];
            i++;
        }

        if (!sibling->isLeafNode) {
            i = 1;
            while (i <= sibling->numkeys) {
                sibling->childPointers[i - 1] = sibling->childPointers[i];
                i++;
            }
        }
    }

    void borrowFromNext(int index) {
        BTreeNode* child = childPointers[index];
        BTreeNode* sibling = childPointers[index + 1];

        child->nodekeys[child->numkeys] = nodekeys[index];

        if (!child->isLeafNode)
            child->childPointers[child->numkeys + 1] = sibling->childPointers[0];

        nodekeys[index] = sibling->nodekeys[0];

        shiftSibling(sibling);

        child->numkeys += 1;
        sibling->numkeys -= 1;

        return;
    }

    void merge(int targetIndex) 
    {
        BTreeNode* childNode = childPointers[targetIndex];
        BTreeNode* siblingNode = childPointers[targetIndex + 1];

        childNode->nodekeys[minDegree - 1] = nodekeys[targetIndex];

        int i = 0;
        while (i < siblingNode->numkeys) {
            childNode->nodekeys[i + minDegree] = siblingNode->nodekeys[i];
            i++;
        }

        if (!childNode->isLeafNode) {
            i = 0;
            while (i <= siblingNode->numkeys) {
                childNode->childPointers[i + minDegree] = siblingNode->childPointers[i];
                i++;
            }
        }

        i = targetIndex + 1;
        while (i < numkeys) {
            nodekeys[i - 1] = nodekeys[i];
            i++;
        }

        i = targetIndex + 2;
        while (i <= numkeys) {
            childPointers[i - 1] = childPointers[i];
            i++;
        }

        childNode->numkeys += siblingNode->numkeys + 1;
        numkeys--;

        delete (siblingNode);
        return;
    }

    void displayBTree() {

        int level = 0;
        if (this == nullptr) {
            cout << "The tree is empty." << endl;
            return;
        }

        queue<BTreeNode*> q;
        q.push(this);

        while (!q.empty()) {
            int size = q.size();

            for (int i = 0; i < size; ++i) {
                BTreeNode* current = q.front();
                q.pop();

                for (int j = 0; j < current->numkeys; ++j)
                    cout << current->nodekeys[j].Hash << " ";

                cout << "| ";

                if (!current->isLeafNode) {
                    for (int j = 0; j <= current->numkeys; ++j) {
                        if (current->childPointers[j] != nullptr)
                            q.push(current->childPointers[j]);
                    }
                }
            }

            cout << endl;
            ++level;
        }
    }
    friend class BTree;

};

class BTree
{
    BTreeNode* root;
    int minDegree;

public:

    BTree(int _minDegree) {
        root = NULL;
        minDegree = _minDegree;
    }

    void traverse() {
        if (root != NULL)
            root->traverse();
    }

    // Insertion operation
    void insertion(BigInt hash, string path, string name) 
    {
        if (root == NULL) {
            root = new BTreeNode(minDegree, true);
            root->nodekeys[0].Hash = hash;
            root->nodekeys[0].Name = name;
            root->nodekeys[0].Path = path;

            root->numkeys = 1;
        }
        else {
            if (root->numkeys == 2 * minDegree - 1) {
                BTreeNode* newRoot = new BTreeNode(minDegree, false);

                newRoot->childPointers[0] = root;

                newRoot->splitChild(0, root);

                int i = 0;
                if (newRoot->nodekeys[0].Hash < hash)
                    i++;
                newRoot->childPointers[i]->insertNonFull(hash, path, name);

                root = newRoot;
            }
            else
                root->insertNonFull(hash, path, name);
        }
    }

    // Delete Operation
    void deletion(BigInt hash, string path, string name) 
    {
        if (!root) {
            cout << "The tree is empty\n";
            return;
        }

        root->deletion(hash, path, name);

        if (root->numkeys == 0) {
            BTreeNode* check = root;
            if (root->isLeafNode)
                root = NULL;
            else
                root = root->childPointers[0];

            delete check;
        }
        return;
    }

    bool search(BigInt key) 
    {
        return (root != nullptr) ? root->search(key.value) : false;
    }

    void displayBTree() 
    {
        if (root != NULL)
            root->displayBTree();
    }

};




