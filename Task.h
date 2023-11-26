#include <iostream>
#include <string>
#include "Assignee.h"
using namespace std;
struct task
{
    int id;
    string description;
    int priority;
    string assigneeTasked;

    bool rb = 1;
    bool completed = false;
    task* parent = NULL;
    task* left = NULL;
    task* right = NULL;
};

struct taskTree
{
    task* root;
    task* nil;

    taskTree()
    {
        nil = new task;
        nil->rb = 1;
        nil->id = -1;
        nil->left = nullptr;
        nil->right = nullptr;
        root = nil;
    }




    void rr(task* p)
    {
        task* temp = p->left;
        p->left = temp->right;
        if (temp->right != nil)
        {
            temp->right->parent = p;
        }
        temp->parent = p->parent;
        if (p->parent == NULL)
        {
            root = temp;
        }
        else if (p == p->parent->right)
        {
            p->parent->right = temp;
        }
        else
        {
            p->parent->left = temp;
        }
        temp->right = p;
        p->parent = temp;
    }

    void lr(task* p)
    {
        task* temp = p->right;
        p->right = temp->left;
        if (temp->left != nil)
        {
            temp->left->parent = p;
        }
        temp->parent = p->parent;
        if (p->parent == NULL)
        {
            root = temp;
        }
        else if (p == p->parent->left)
        {
            p->parent->left = temp;
        }
        else
        {
            p->parent->right = temp;
        }
        temp->left = p;
        p->parent = temp;
    }

    void fixIn(task* p)
    {
        task* temp;
        while (p->parent->rb)
        {
            if (p->parent == p->parent->parent->right)
            {
                temp = p->parent->parent->left;
                if (!temp->rb)
                {
                    if (p == p->parent->left)
                    {
                        p = p->parent;
                        rr(p);
                    }
                    p->parent->rb = 0;
                    p->parent->parent->rb = 1;
                    lr(p->parent->parent);
                }
                else
                {
                    temp->rb = 0;
                    p->parent->rb = 0;
                    p->parent->parent->rb = 1;
                    p = p->parent->parent;
                }
            }
            else
            {
                temp = p->parent->parent->right;
                if (temp->rb)
                {
                    temp->rb = 0;
                    p->parent->rb = 0;
                    p->parent->parent->rb = 1;
                    p = p->parent->parent;
                }
                else
                {
                    if (p == p->parent->right)
                    {
                        p = p->parent;
                        lr(p);
                    }
                    p->parent->rb = 0;
                    p->parent->parent->rb = 1;
                    rr(p->parent->parent);
                }
            }
            if (p == root)
            {
                break;
            }
            if (p->left == nil && p->right == nil)
            {
                p->rb = 0;
            }
        }
        root->rb = 0;
        cout <<"Color: " << p->rb << endl;
    }

    void insert(task* p)
    {
        p->left = nil;
        p->right = nil;
        task* y = NULL;
        task* x = root;

        while (x != nil) {
            y = x;
            if (p->priority < x->priority)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
            //code here for same priority
        }
        p->parent = y;
        if (y == nullptr)
        {
            root = p;
        }
        else if (p->priority < y->priority)
        {
            y->left = p;
        }
        else
        {
            y->right = p;
        }

        if (p->parent == NULL)
        {
            p->rb = 0;
            return;
        }

        if (p->parent->parent == NULL)
        {
            return;
        }

        fixIn(p);
    }

    void reassign(string a, string b, task* p)
    {
        if (p->left == nullptr && p->right == nullptr)
        {
            return;
        }
        
        if (p->assigneeTasked == a)
        {
            p->assigneeTasked = b;
        }

        reassign(a, b, p->left);
        reassign(a, b, p->right);
    }

    void complete(int id)
    {
        task* temp  = search(root, id);
        temp->completed = true;
    }

    string list(assigneeTree t, bool complete)
    {
        string ans;
        traverse(ans, root, t, complete );
        return ans;
    }

    void traverse(string &ans, task* p, assigneeTree t, bool c)
    {
        if (p == nil)
        {
            return;
        }
        traverse(ans, p->left, t,c);
        if (c && p->completed)
        {
            ans += getInfo(p, t);
        }
        if (!c && !p->completed)
        {
            ans += getInfo(p, t);
        }
        traverse(ans, p->right, t,c);
    }

    string list(assigneeTree t, int a, int b)
    {
        string ans;
        if (a < b)
        {
            traverse(ans, root, t, a, b);
        }
        else
        {
            traverse(ans, root, t, b, a);
        }
        return ans;
        
    }

    void traverse(string& ans, task* p, assigneeTree t, int a, int b)
    {
        if (p == nil)
        {
            return;
        }
        traverse(ans, p->left, t, a,b);
        if (p->priority >= a && p->priority <= b)
        {
            ans += getInfo(p, t);
        }
        traverse(ans, p->right, t, a,b);
    }

    string convertIdtoString(int id)
    {
        string ans;
        ans += char((id / 100) + 48);
        ans += char(((id % 100) / 10) + 48);
        ans += char((id % 10) + 48);
        return ans;
    }

    int convertStringtoId(string id)
    {
        int tag = 0;
        tag += int(id[1]) - 48;
        tag *= 10;
        tag += int(id[2]) - 48;
        tag *= 10;
        tag += int(id[3]) - 48;
        return tag;
    }

    task* search(task* n, int key) {
        if (n == nil || key == n->id) {
            return n;
        }

        if (key < n->id)
        {
            return search(n->left, key);
        }
        return search(n->right, key);
    }

    string getInfo(task* temp, assigneeTree t)
    {
        //Task ID: 103, Description: Write Unit Tests, Priority: 1, Assignee: Hashir (A003)\n

        assignee* q = t.searchRecur(convertStringtoId(temp->assigneeTasked), t.root);
        
        string result;
        result = "Task ID: ";
        result += convertIdtoString(temp->id);
        result += ", Description: ";
        result += temp->description;
        result += ", Priority: ";
        result += char((temp->priority)+48);
        result += ", Assignee: ";
        result += q->fName;
        result += " (";
        result += temp->assigneeTasked;
        result += ")";
        result += "\n";

        q = NULL;

        return result;
    }

    string firstpriority(assigneeTree t)
    {
        string x = "Highest Priority Task: ";
        task* temp = root;
        while (temp->left != nil)
        {
            temp = temp->left;
        }
        x += getInfo(temp, t);
        
        return x;
    }


    string AssigneeSearch(string name, string id)
    {
        string ans = "Tasks Assigned to \"";
        ans += name;
        ans += " (";
        ans += id;
        ans += ")\":\n";

        assigneelookup(ans,name, id, root);
        return ans;
    }

    void assigneelookup(string& ans, string name, string id, task* p)
    {
        if (p == nil)
        {
            return;
        }
        assigneelookup(ans,name, id, p->left);
        if (!p->completed && p->assigneeTasked == id)
        {
            ans += getInfo(p, name, id);
        }
        assigneelookup(ans, name, id, p->right);
    }

    string getInfo(task* temp, string name, string id)
    {
        string result;
        result = "Task ID: ";
        result += convertIdtoString(temp->id);
        result += ", Description: ";
        result += temp->description;
        result += ", Priority: ";
        result += char((temp->priority) + 48);
        result += ", Assignee: ";
        result += name;
        result += " (";
        result += id;
        result += ")";
        result += "\n";
        return result;
    }

    string colorPrint()
    {
        string ans;
        inOrderColor(root, ans);
        return ans;
    }

    void inOrderColor(task* p, string& ans)
    {
        if (p == nil)
        {
            return;
        }
        inOrderColor(p->left, ans);
        ans += convertIdtoString(p->id);
        if (p->rb)
        {
            ans += " (red)\n";
        }
        else
        {
            ans += " (black)\n";
        }
        inOrderColor(p->right, ans);
    }

    void fixOut(task* p)
    {
        task* temp;
        while (p != root && p->rb == 0)
        {
            if (p == p->parent->left)
            {
                temp = p->parent->right;
                if (temp->rb == 1)
                {
                    temp->rb = 0;
                    p->parent->rb = 1;
                    lr(p->parent);
                    temp = p->parent->right;
                }

                if (temp->left->rb == 0 && temp->right->rb == 0)
                {
                    temp->rb = 1;
                    p = p->parent;
                }
                else
                {
                    if (temp->right->rb == 0)
                    {
                        temp->left->rb = 0;
                        temp->rb = 1;
                        rr(temp);
                        temp = p->parent->right;
                    }

                    temp->rb = p->parent->rb;
                    p->parent->rb = 0;
                    temp->right->rb = 0;
                    lr(p->parent);
                    p = root;
                }
            }
            else
            {
                temp = p->parent->left;
                if (temp->rb == 1)
                {
                    temp->rb = 0;
                    temp->parent->rb = 1;
                    rr(p->parent);
                    temp = p->parent->left;
                }
                if (temp->right->rb == 0 && temp->right->rb == 0)
                {
                    temp->rb = 1;
                    p = p->parent;
                }
                else
                {
                    if (temp->left->rb == 0)
                    {
                        temp->right->rb = 0;
                        temp->rb = 1;
                        lr(temp);
                        temp = p->parent->left;
                    }

                    temp->rb = p->parent->rb;
                    p->parent->rb = 0;
                    temp->left->rb = 0;
                    rr(p->parent);
                    p = root;
                }
            }
        }
        p->rb = 0;
    }

    void flip(task* a, task* b)
    {
        if (a->parent == NULL)
        {
            root = b;
        }
        else if (a == a->parent->left)
        {
            a->parent->left = b;
        }
        else
        {
            a->parent->right = b;
        }
        b->parent = a->parent;

    }

    void remove(int id)
    {
        task* temp = search(root, id);
        if (temp == nil)
        {
            cout << "Task with this ID does not exist" << endl;
            return;
        }
        else
        {
            task* temp2 = temp;
            task* temp3;
            bool color = temp2->rb;
            if (temp->right == nil)
            {
                temp3 = temp->left;
                flip(temp, temp->left);
            }
            else if (temp->left == nil)
            {
                temp3 = temp->right;
                flip(temp, temp->right);
            }
            else
            {
                temp2 = max(temp->left);
                color = temp2->rb;
                temp3 = temp2->left;
                temp->priority = temp2->priority;
                flip(temp2, temp2->left);
            }
            if (color == 0)
            {
                fixOut(temp3);
            }
            delete temp;

            
        }

    }

    task* min(task* p)
    {
        while (p->left != nil)
        {
            p = p->left;
        }
        return p;
    }

    task* max(task* p)
    {
        while (p->right != nil)
            p = p->right;

        return p;

    }

};

