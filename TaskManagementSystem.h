#include <iostream>
#include <string>
#include "Assignee.h"
#include "Task.h"
using namespace std;


class taskManagementSystem
{
    assigneeTree assigneeDB;
    int assigneeCtr;

    taskTree taskDB;
    int taskCtr;

public:

    taskManagementSystem()
    {
        assigneeCtr = 0;
        taskCtr = 0;
    }

    void addAssignee(string f, string l, string a, string d)
    {
        assignee* p = new assignee;
        p->fName = f;
        p->lName = l;
        p->address = a;
        p->dob = d;
        assigneeCtr++;
        string S = convertIdtoString(assigneeCtr);
        p->id = S;
        p->tag = assigneeCtr;
        assigneeDB.insert(p);
        p = NULL;
        delete p;
    }

    string convertIdtoString(int id)
    {
        string res = "A";
        res += char((id / 100) + 48);
        res += char(((id % 100) / 10) + 48);
        res += char((id % 10) + 48);
        return res;
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

    void displaybyID(stringstream& o, string id)
    {
        o << assigneeDB.search(convertStringtoId(id));
    }

    void displaybyname(stringstream& o, string f, string l)
    {
        o << assigneeDB.searchName(f, l);
    }

    void addTask(int id, string desc, int pri, string Assignee)
    {
        task* temp = new task;
        temp->id = id;
        temp->description = desc;
        temp->priority = pri;
        temp->assigneeTasked = Assignee;
        temp->rb = 1;
        taskDB.insert(temp);

        int tempId = convertStringtoId(Assignee);

        assigneeDB.tasked(tempId);
        taskCtr++;
        temp = NULL;
        delete temp;
    }

    void AssigneeWithNoTask(stringstream& o)
    {
        string list;
        assigneeDB.freeAssignees(list, assigneeDB.root, false);
        o << list;
    }

    void ShiftTask(string a, string b)
    {
        taskDB.reassign(a, b, taskDB.root);
        assigneeDB.reassign(convertStringtoId(a), convertStringtoId(b));
    }

    void printTaskQueue(stringstream &o)
    {
        o << taskDB.list(assigneeDB,false);     
    }

    void DeleteAssignee(string id)
    {
        assigneeDB.remove(convertStringtoId(id));
    }

    void DisplayAssignee(stringstream& o)
    {
        o << assigneeDB.display();
    }

    void AssigneeInOrder(stringstream& o)
    {
        cout << "1" << endl;
        o << assigneeDB.colorDB();
    }

    void completeTask(int id)
    {
        taskCtr--;
        taskDB.complete(id);
    }

    void updateTaskPriority(int id, int pr)
    {
        task* temp = taskDB.search(taskDB.root, id);
        task* temp2 = temp;
        temp->completed = true;
        temp2->priority = pr;
        taskDB.insert(temp2);
    }

    void findHighestPriorityTask(stringstream& o)
    {
        o<<taskDB.firstpriority(assigneeDB);
    }

    void findTasksByAssignee(string id, stringstream& o)
    {
        int i = convertStringtoId(id);
        assignee* temp = assigneeDB.searchRecur(i, assigneeDB.root);
        o << taskDB.AssigneeSearch(temp->fName, id);
    }

    void countTotalTasks(stringstream& o)
    {
        string x = "Total Tasks in the System: ";
        x += char(taskCtr + 48);
        x += "\n";
        o << x;
    }

    void displayCompletedTasks(stringstream& o)
    {
        o << "Completed Tasks:\n";
        o << taskDB.list(assigneeDB,true);
    }

    void searchTasksByPriorityRange(int a, int b, stringstream& o)
    {
        o << "Tasks within Priority Range (";
        o << char(a + 48);
        o << " to ";
        o << char(b + 48);
        o << "):\n";
        o << taskDB.list(assigneeDB,a,b);
    }

    void PrintTreeInorder(stringstream& o)
    {
        o << taskDB.colorPrint();
    }

    void deleteTask(int id)
    {
        taskDB.remove(id);
    }
};