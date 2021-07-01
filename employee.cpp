#include<iostream>      //input output
#include<fstream>       //read and write from file
#include<iomanip>       //setw
#define SIZE 100
using namespace std;

class employee          //database of employee
{
    int empno;
    int salary;
    char name[SIZE];
    public: void accept();
            void display();
            void modify();
            int getno();
};
void employee :: accept()
{
    cout<<"Enter the employee number:  ";
    cin>>empno;
    cin.ignore();                   //clears one or more character from input buffer
    cout<<"Enter the name of the employee:  ";
    cin.getline(name,SIZE);
    cout<<"Enter salary:  ";
    cin>>salary;
}
void employee :: display()
{
    cout<<'\t'<<empno<<'\t'<<name<<'\t'<<salary<<endl;
}
void employee :: modify()
{
    cin.ignore();
    cout<<"Enter the new name of employee:  ";
    cin.getline(name, SIZE);
    cout<<"Enter the new salary:  ";
    cin>>salary;
}
int employee :: getno()
{
    return empno;
}
//external functions
int create(int);
void display(int);
int add(int);
void deleterec(int);
void modifyrec(int);

int create(int cntr)
{
    int eno;
    employee e;     //to create file we are using this object
    ofstream fout("recs.dat", ios:: binary |ios:: out);     //to create file, output in a file where fout is object of ofstream
    cntr = 0;
    char ask='y';
    while(ask == 'y' || ask == 'Y')
    {
        e.accept();     //object of employee accepts reccord
        fout.write((char*)&e,sizeof(e));
        cntr++;
        cout<<" \n Do you want to add more (y/n):  ";
        cin>>ask;
    }
}
void display(int cntr)
{
    int i;
    employee e;
    if(cntr==0)
        cout<<"Please build a master table first!"<<endl;
    else
    {
        ifstream fin("recs.dat",ios:: binary |ios:: in);        //read mode
        cout<<"\n -------------------------------------- \n";
        cout<<"Sr no"<<setw(5)<<"Employee no";
        cout<<"Name";
        cout<<"Salary"<<endl;
        cout<<"\n -------------------------------------- \n";

        i=1;
        while(fin.read((char*)&e,sizeof(e)))        //to read memory address of the object we get that from &e, sizeof e we do to read that number of bytes
        {
            cout<<"  "<<i<<"  ";
            e.display();
            i++;
        }
        fin.close();
    }
}
int add(int cntr)
{
    employee e;
    ofstream fout("recs.dat", ios:: binary | ios:: app);        //information added from the point where we left, continued not started form beginning
    cout<<"Enter the details of record to be inserted:  ";
    e.accept();         //write object into file
    fout.write((char*)&e,sizeof(e));
    fout.close();
    cntr++;
    cout<<"Record added successfully!"<<endl;
    return cntr;
}
void deleterec(int cntr)
{
    employee e;
    int flag = 0;
    int eno;
    ifstream fin("recs.dat", ios:: binary | ios:: in);      //to read records from recs.dat file
    cout<<"Enter the employee number you want to delete:  ";
    cin>>eno;
    while(fin.read((char*)&e, sizeof(e)))
    {
        if(eno == e.getno())
        {
            flag = 1;
            cntr--;
            break;
        }
        fin.close();
        if(!flag)
            cout<<"\n Record with the given employee number not found";
        else
        {
            ofstream fout("temp.dat", ios:: binary | ios:: out);        //open temp.dat file for write operation
            ifstream fin("recs.dat", ios:: binary | ios:: out);         //open recs.dat file for read operation

            fin.seekg(0, ios:: beg);
            fin.read((char*)&e, sizeof(e));
            while(!fin.eof())
            {
                if(e.getno() != eno)        //if employee number didnot match
                    fin.read((char*)&e, sizeof(e));        //read record from recs.dat file
            }
            fout.close();
            fin.close();
            cout<<"Record with employee number "<<eno<<" deleted successfully"<<endl;
            remove("recs.dat");
            rename("temp.dat", "recs.dat");
        }
    }
}
void modifyrec(int cntr)
{
    employee e;
    int flag = 0;
    int eno;
    ifstream fin("recs.dat", ios:: binary | ios:: in);      //to read records from recs.dat file
    cout<<"Enter the employee number you want to edit:  ";
    cin>>eno;
    while(fin.read((char*)&e, sizeof(e)))
    {
        if(eno == e.getno())
        {
            flag = 1;               //record is available
            cntr--;
            break;
        }
        fin.close();
        if(!flag)
            cout<<"\n Record with the given employee number not found";
        else
        {
            ofstream fout("temp.dat", ios:: binary | ios:: out);        //open temp.dat file for write operation
            ifstream fin("recs.dat", ios:: binary | ios:: in);
            fin.read((char*)&e, sizeof(e));
            while(!fin.eof())
            {
                if(e.getno() == eno)
                {
                    e.modify();
                    fout.write((char*)&e, sizeof(e));
                }
                else
                {
                    fout.write((char*)&e, sizeof(e));
                }
            fin.read((char*)&e, sizeof(e));
            fout.close();               //close temp.dat
            fin.close();                //close recs.dat
            cout<<"Record with employee number "<<eno<<" modified successfully"<<endl;
            remove("recs.dat");
            rename("temp.dat", "recs.dat");
                    }
                }
            }
        }

}
}

int main()
{
    int choice, flag, counter=0;       //local variables
    while(1)
    {
        cout<<"         EMPLOYEE DATABASE          "<<endl;
        cout<<"1. Create \n2. Display \n3. Add record \n4. Delete record \n5. Modify the record \n6. Exit"<<endl;
        cout<<"     Enter your choice:  ";
        cin>>choice;
        switch(choice)
        {
            case 1: counter = create(counter);
                    break;
            case 2: display(counter);
                    break;
            case 3: if(counter == 0)                                        //checks initially if counter is 0 there is no base table
                        cout<<"Please enter a master table first! "<<endl;
                    else
                        counter = add(counter);                             //add the records
                    break;
            case 4: if(counter == 0)                                        //checks initially if counter is 0 there is no base table
                        cout<<"Please enter a master table first! "<<endl;
                    else
                        deleterec(counter);                       //delete the records
                    break;
            case 5: if(counter == 0)                                        //checks initially if counter is 0 there is no base table
                        cout<<"Please enter a master table first! "<<endl;
                    else
                        modifyrec(counter);                        //add the records
                    break;
            case 6: return 0;
            default: cout<<"Enter correct choice!"<<endl;
        }
    }
    return 0;
}
