#include <iostream>
#include <string>
using namespace std;

struct assignee
{
	string fName;
	string lName;
	string dob;
	string address;
    string id;
    int tag;
    bool tasked = false;
	assignee* left = NULL;
	assignee* right = NULL;
	assignee* parent = NULL;

	bool rb = 1; //If red then 1, If black then 0
};

struct assigneeTree
{
	assignee* root;
    assignee* nil;

    assigneeTree()
    {
        nil = new assignee;
        nil->rb = 0;
        nil->left = nullptr;
        nil->right = nullptr;
        //nil->parent = nullptr;
        root = nil;
    }

    //right rotate
    void rr(assignee* p)
    {
        assignee* temp = p->left;
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

    //left rotate
    void lr(assignee* p)
    {
        assignee* temp = p->right;
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

    //function to fix tree after insert
    void fixIn(assignee* p)
    {
        assignee* temp;
        while (p->parent->rb)
        {
            if (p->parent == p->parent->parent->right)
            {
                temp = p->parent->parent->left;
                if (temp->rb)
                {
                    temp->rb = 0;
                    p->parent->rb = 0;
                    p->parent->parent->rb = 1;
                    p = p->parent->parent;
                }
                else
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
        }
        root->rb = 0;

        temp = NULL;
        delete temp;
    }

    //function to fix tree after deletion
    void fixOut(assignee* p)
    {
        assignee* temp;
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
	
    void insert(assignee* p) 
    {
        p->left = nil;
        p->right = nil;
        assignee* y = NULL;
        assignee* x = root;

        while (x != nil) {
            y = x;
            if (p->id < x->id) 
            {
                x = x->left;
            }
            else 
            {
                x = x->right;
            }
        }
        p->parent = y;
        if (y == nullptr) 
        {
            root = p;
        }
        else if (p->id < y->id) 
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


        fixIn(p);
    }

    //helper function for delete
    void flip(assignee* a, assignee* b)
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

    assignee* min(assignee* p)
    {
        while (p->left != NULL)
        {
            p = p->left;
        }
        return p;
    }

    void remove(int id)
    {
        assignee* temp = searchRecur(id, root);
        if (temp == nil)
        {
            cout << "Assignee with this ID does not exist" << endl;
            return;
        }
        if (temp->tasked == true)
        {
            cout << "This Assignee can not be deleted as they have tasks yet to complete" << endl;
        }
        else
        {
            assignee* temp2 = temp;
            assignee* temp3;
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
                temp2 = min(temp->right);
                color = temp2->rb;
                temp3 = temp2->right;
                if (temp2->parent == temp)
                {
                    temp3->parent = temp2;
                }
                else 
                {
                    flip(temp2, temp2->right);
                    temp2->right = temp->right;
                    temp2->right->parent = temp2;
                }

                flip(temp, temp2);
                temp2->left = temp->left;
                temp2->left->parent = temp2;
                temp2->rb = temp->rb;
            }
            delete temp;
            if (color == 0)
            {
                fixOut(temp3);
            }
            
        }

    }

    string search(int key)
    {
        return getInfo(searchRecur(key,root));
    }

    assignee* searchRecur(int id, assignee* p)
    {
        if (p == nil || id == p->tag)
        {
            return p;
        }
        else if (id > p->tag)
        {
            return searchRecur(id, p->right);
        }
        else
        {
            return searchRecur(id, p->left);
        }
        if (p->left == nil && p->right == nil && p->tag != id)
        {
            return nil;
        }
    }

    string searchName(string f, string l)
    {
        string answer;
        searchNameRe(f, l, root,answer);
        return answer;
    }

    assignee* searchNameRe(string f, string l, assignee* p, string &s)
    {
        if (p == nil)
        {
            return root;
        }
        searchNameRe(f, l, p->left, s);

        if (p->fName == f && p->lName == l)
        {
            s += getInfo(p);
        }
        searchNameRe(f, l, p->right, s);
        
    }

    string display()
    {
        string s;
        inOrderDisplay(root, s);
        return s;
    }

    void inOrderDisplay(assignee* p, string& s)
    {
        if (p == nil)
        {
            return;
        }
        inOrderDisplay(p->left,s);
        s += getInfo(p);
        inOrderDisplay(p->right, s);
    }

    string colorDB()
    {
        cout << "2" << endl;
        string list;
        inOrderColor(root, list);
        cout << "10" << endl;
        return list;
    }

    void inOrderColor(assignee* p, string& s)
    {
        cout << "3" << endl;
        if (p == nil)
        {
            return;
        }
        cout << "4" << endl;
        inOrderColor(p->left, s);
        cout << "5" << endl;
        s += getColor(p);
        cout << "6" << endl;
        inOrderColor(p->right, s);
        cout << "7" << endl;

    }

    string getColor(assignee* temp)
    {
        cout << "8" << endl;
        string result;
        result += temp->id;
        result += " (";
        if (temp->rb)
        {
            result += "black";
        }
        else
        {
            result += "red";
        }
        result += ")\n";
        cout << "ID: " << result << endl;
        cout << "9" << endl;
        return result;
    }

    string getInfo(assignee* temp)
    {
        string result;
        result = "Name: ";
        result += temp->fName;
        result += " ";
        result += temp->lName;
        result += ", Address: ";
        result += temp->address;
        result += ", DOB: ";
        result += temp->dob;
        result += ", Assignee ID: ";
        result += temp->id;
        result += "\n";

        return result;
    }

    void tasked(int key)
    {
        assignee* temp = searchRecur(key,root);
        temp->tasked = true;

        temp = NULL;
        delete temp;
    }

    void freeAssignees(string& list, assignee* r, bool rev)
    {
        if (r == nil)
        {
            return;
        }
        freeAssignees(list, r->left, rev);
        if(r->tasked == false)
            list += getInfo(r);
        
        
        
        freeAssignees(list, r->right, rev);
    }

    void reassign(int a, int b)
    {
        assignee* temp = searchRecur(a, root);
        temp->tasked = false;
        temp = searchRecur(b, root);
        temp->tasked = true;
        temp = NULL;
        delete temp;
    }

};
