#include <iostream>
#include <cstdlib>
#include<ctime>
#include "Btree.h"

using namespace std;

class Computer
{
public:
    Computer* next;
    Computer* prev;
    Computer* Router;
    BTree* Tree;

    BigInt ComputerID=0;
    int key=0;
    Computer():next(nullptr),prev(nullptr),Router(nullptr),Tree(nullptr) {}
    ~Computer()
    {
        delete next;
        delete prev;
        delete Router;
    }
};

class Circular_Network
{
public:
    Computer* root;
    int Count = 0;

    Circular_Network() :root(nullptr) {}

    void ActiateMachine(BigInt data,int size)
    {
        Computer* temp = new Computer;
        temp->ComputerID = data;
        temp->Tree = new BTree(size);
        
        if (!root)
        {
            temp->next = temp;
            temp->prev = temp;
            root = temp;
        }
        else
        {
            Computer* last = root->prev;
            temp->next = root;
            root->prev = temp;
            temp->prev = last;
            last->next = temp;
        }
        Count++;
    }

    void Display(int active)
    {
        Computer* temp = root;
        for (int n = 0; n < active; n++)
        {
            cout << temp->ComputerID;
            if (n <active-1)
                cout << "->";
            temp = temp->next;
        }
        
    }
    void DisplayReverse(int cap)
    {
        cout << "\nComputer Cycle Reverse : ";
        Computer* temp = root->prev;
        for (int n = 0; n < cap; n++)
        {
            cout << temp->ComputerID;
            if (n < cap - 1)
                cout << "->";
            temp = temp->prev;
        }
    }

    void AddRouter(BigInt data, Computer* head)
    {
        Computer* temp = new Computer;
        temp->ComputerID = data;
        if (!head->Router)
        {
            temp->next = temp;
            temp->prev = temp;
            head->Router = temp;
        }
        else
        {
            Computer* last = head->Router->prev;
            temp->next = head->Router;
            head->Router->prev = temp;
            temp->prev = last;
            last->next = temp;
        }
    }

    BigInt succ(BigInt computer, int id)
    {
        BigInt Two(2);
        BigInt temp = (computer + Two.power(id));
        return temp;
    }

    void RoutingSystem(int size,int active,string index)
    {
        Computer* temp = root;
        while (temp->ComputerID != index)
            temp=temp->next;

        BigInt Two(2);
        BigInt MaxId(root->prev->ComputerID);
        BigInt Capacity = Two.power(size);

        //cout << "\n\nUnit : " << temp->ComputerID << " \n";
        int n = 0;
        while (n < active)
        {
            Computer* ptr = root;
            bool check = true;
            BigInt location(0);
            while (check)
            {
                location = succ(temp->ComputerID, n);
                check = false;
                if (location > MaxId)
                {
                    if (location > Two.power(size))
                    {
                        Computer* q = root;
                        location = location.modwithstring(location, Two.power(size).value);
                        bool check1 = true;
                        while (check1)
                        {
                            check1 = false;
                            if (location > q->ComputerID)
                            {
                                check1 = true;
                                q = q->next;
                            }
                        }
                        ptr = q;
                    }
                    else
                        ptr = root;
                }
                else if (location > ptr->ComputerID && location<=MaxId)
                {
                    ptr = ptr->next;
                    check = true;
                }
            }
           // cout << "[" << n + 1 << "] -> " << ptr->ComputerID << " : (" << location << ")\n";
            
            AddRouter(ptr->ComputerID, temp);
            n++;
            
        }
        /*
        cout << "Routiing Cycle : ";
        for (int n = 0; n < active; n++)
        {
            cout << temp->Router->ComputerID;
            if (n < active - 1)
                cout << "->";
            temp->Router = temp->Router->next;
        }
        */
        
    }

    void DisplayRoutingCycle(int active, string index)
    {
        Computer* temp = root;
        while (temp->ComputerID != index)
            temp = temp->next;
        cout << "Routiing Cycle : ";
        for (int n = 0; n < active; n++)
        {
            cout << temp->Router->ComputerID;
            if (n < active - 1)
                cout << "->";
            temp->Router = temp->Router->next;
        }
    }

    void RouterMaker(int size, int capacity)
    {
        Computer* temp = root;

        for (int n = 0; n < size; n++)
        {
            Computer* ptr = root;
            int m = 1;
            bool checker = true;
            BigInt location(0);
            while (m <= size)
            {
                if (checker)
                    location = succ(temp->ComputerID, m);

                if (location <= ptr->ComputerID)
                {
                    AddRouter(ptr->ComputerID, temp);
                    cout << location << "->" << ptr->ComputerID << endl;
                    checker = true;
                    ptr = root;
                }
                else
                {
                    m--;
                    checker = false;
                }
                m++;
                ptr = ptr->next;
            }
            temp = temp->next;
            cout << endl;
        }
    }
};
