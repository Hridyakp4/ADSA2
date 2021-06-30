#include<iostream>
#include<iomanip>
#include<string.h>
#include<fstream>
#define SIZE 5
using namespace std;

struct student              //student database for multiple students
{
    int rollNo;             //define properties of student
    char name[20];
    float marks;
    int status;
    int link;
};

class linearProbe           //create hash table
{
    char table[30];
    fstream tab;            //to read and write to record from table
    student record;         //through this object we will access the record

    public: linearProbe();  //construct
            void displayAll();
            void Insert(student);
            int searchKey(int);
            void display(int);
            void read(int);
            void write(int);
            int modify(int);
};

linearProbe :: linearProbe()        //construct hash table
{
    int i;
    cout<<"Enter filename:  ";
    cin>>table;                     //displays as table
    record.status = 1;              //record is the reference variable, initially
    record.link = -1;               //link not anywhere
    tab.open(table, ios:: binary | ios:: out);    //open table to write
    tab.close();                    //close
    for(i=0; i<SIZE; i++)
        write(i);
}
void linearProbe :: displayAll()
{
    int i;
    for(i=0; i<SIZE; i++)
        display(i);
}
void linearProbe :: Insert(student s)       //s as an object
{
    int n, i, j, start, k;
    s.status = 0;
    s.link = -1;
    start = s.rollNo % SIZE;        //calculate hash value index of roll no

    for(i=0; i<SIZE; i++)           //to find next empty location
    {
        j = (start+i)% SIZE;
        read(j);
        if(record.status == 0 && (record.rollNo % SIZE)== start) //check vacancy and remainder value of key
            break;          //store in index as status is free
    }
    if(i < 5)
    {
        while(record.link != -1)        //to read link, if not already filled
        {
            j = record.link;
            read(j);
        }
        for(i=0; i<SIZE; i++)
        {
            k = (start+i)% SIZE;        //extra element want to insert, if vacant space available
            read(k);                    //use constructor whenever we define object constructor automatically called
            if(record.status)
            {
                record = s;             //this object calls constructor so wants roll no, name , marks
                write(k);
                read(j);
                record.link = k;
                write(j);
                return;
            }
        }
        cout<<"\n Table is full";
    }
    else
    {
        for(i=0; i<SIZE; i++)           //if we dont have to put in vacant spaces directly put in table
        {
            k = (start+i)%SIZE;
            read(k);
            if(record.status)
            {
                record = s;
                write(k);
                return;
            }
        }
        cout<<"\n Table is full";
    }
}
int linearProbe :: searchKey(int rno)
{
    int start, i, j;
    start = rno % SIZE;     //find remainder, hashing function

    for(i=0; i<SIZE; i++)
    {
        j = (start+i)% SIZE;    //search for next location
        read(j);
        if(record.status == 0 && (record.rollNo %SIZE)== start)
            break;
    }
    if(i<5)
    {
        while(record.rollNo != rno && record.link != -1)
        {
            j = record.link;
            read(j);
        }
        if(record.rollNo == rno)
            return j;
        else
            return -1;
    }
    else
        return -1;
}
void linearProbe :: display(int rno)
{
    int i = rno;
    tab.open(table, ios::binary | ios::in) ;     //open a file in input mode
    tab.seekg(rno*sizeof(student), ios::beg);    //get pointer at beginning
    tab.read((char*)&record, sizeof(student));  //to read a specific number of bytes from the file
                                                //sizeof(students) yields the object that we are referring to, &record is memory address of object called record
    if(record.status == 0)
    {
        cout<<endl<<i<<"  "<<record.rollNo<<"   "<<record.name<<"   "<<record.marks<<endl;
    }
    else
        cout<<endl<<i<<" Empty ";
    tab.close();
}
void linearProbe :: read(int rno)           // reads specific number of bytes from the file
{
    tab.open(table, ios::binary | ios:: in);
    tab.seekg(rno *sizeof(student), ios:: beg);
    tab.read((char*)&record, sizeof(student));
    tab.close();
}
void linearProbe :: write(int rno)          //helps us write a specific number of bytes in file
{
    tab.open(table, ios::binary | ios:: out);
    tab.seekg(rno *sizeof(student), ios:: beg);
    tab.read((char*)&record, sizeof(student));
    tab.close();
}
int linearProbe :: modify(int rno)
{
    int start, i, j;
    start = rno % SIZE;     //find remainder

    for(i=0; i<SIZE; i++)
    {
        j = (start+i)% SIZE;    //search from next location
        read(j);
        if(record.status == 0 && (record.rollNo %SIZE)== start)
            break;
    }
    if(i<5)
    {
        while(record.rollNo != rno && record.link != -1)
        {
            j = record.link;
            read(j);
        }
        if(record.rollNo == rno)
        {
            tab.open(table, ios:: binary| ios:: in| ios:: out);
            tab.seekg(rno *sizeof(student), ios:: beg);
            cout<<"\n Enter modified roll number, name and marks:  ";
            cin>>record.rollNo>>record.name>>record.marks;
            tab.read((char*)&record, sizeof(student));
            tab.close();
            return j;
        }
        else
            return -1;
    }
    else
        return -1;
}
int main()
{
    int choice, rno, check;
    student rs;             //object
    linearProbe lp;
    while(1)
    {
        cout<<"1. Create database \n2. Display database \n3. Add a record \n4. Search a record";
        cout<<"\n5. Modify a record \n6. Exit"<<endl;
        cout<<"Enter your choice:  ";
        cin>>choice;
        switch(choice)
        {
            case 1: cout<<"\n Enter roll number, name and marks:  ";
                    cin>>rs.rollNo>>rs.name>>rs.marks;
                    lp.Insert(rs);
                    break;
            case 2: lp.displayAll();
                    break;
            case 3: cout<<"\n Enter roll number, name and marks:  ";
                    cin>>rs.rollNo>>rs.name>>rs.marks;
                    lp.Insert(rs);
                    break;
            case 4: cout<<"\n Enter roll number to be searched:  ";
                    cin>>rno;
                    check = lp.searchKey(rno);
                    if(check >= 0)
                    {
                        cout<<"\n Record number: "<<check;
                        lp.display(check);
                    }
                    else
                        cout<<"\n Record not found";
                    break;
            case 5: cout<<"\n Enter roll number to be searched:  ";
                    cin>>rno;
                    check = lp.modify(rno);
                    if(check >= 0)
                    {
                        cout<<"\n Record number: "<<check;
                        lp.display(check);
                    }
                    else
                        cout<<"\n Record not found";
                    break;
        }
    }
}
