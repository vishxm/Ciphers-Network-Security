/*
Submitted by UI17CO49 (Vishwas Modhera)

Program to implement caesar cipher in C++.
Structure of the Program:
    5 functions (menu, encryption, decryption, frequency analysis, view file)
    1 comparator (for vector sorting)
    driver function - main() 

    brute force analysis will be done when no key is provided during decryption
*/

#include<bits/stdc++.h>
#include<fstream>
using namespace std;

void menu(void);
void encryption(void);
void decryption(void);
void freq_analysis(void);
void show_content(void);

bool cmp(pair<char, int> &a, pair<char, int> &b)
{
    return a.second > b.second;
}


void encryption()
{
    cout<<"\n\n\n----------------------------------------------------\n\n";
    cout<<"\t\tEncryption Portal\n\n";
    cout<<"----------------------------------------------------\n\n";
    string fname;
    cout<<"\tEnter the name of file to be encrypted: ";
    cin>>fname;
    int key;
    cout<<"\tEnter the amount to be shifted (1-256): ";
    cin>>key;
    
    
    ifstream ipfile;
    ofstream opfile;

    ipfile.open(fname, ios::in);
    string opname = "cipher-"+fname;
    opfile.open(opname, ios::out);

    if(ipfile.is_open() && opfile.is_open())
    {
        char temp;
        while(!ipfile.eof())
        {
            ipfile.get(temp);
            opfile<<char((temp+key)%256);
        }
    }
    else
    {
        cout<<"\n\n\tFILES DIDN'T OPEN!";
        menu();
    }
    cout<<"\n\n\t\tENCRYPTION SUCCESSFUL!";
    cout<<"\n\n\t\tFile has been saved with the name "<<opname<<"!\n\n";
    ipfile.close();
    opfile.close();
    menu();
}

void decryption()
{
    cout<<"\n\n\n----------------------------------------------------\n\n";
    cout<<"\t\tDecryption Portal\n\n";
    cout<<"----------------------------------------------------\n\n";
    string fname;
    cout<<"\tEnter the name of file to be decrypted: ";
    cin>>fname;
    int key;
    char choice;
    cout<<"\tDo you want to input the key? (y/n) : ";
    cin>>choice;
    if(choice == 'y')
    {
        cout<<"\tEnter the key for the decryption: ";
        cin>>key;
        ifstream ipfile;
        ofstream opfile;
        
        ipfile.open(fname, ios::in);
        string opname = "decrypted-"+fname;
        opfile.open(opname, ios::out);

        if(ipfile.is_open() && opfile.is_open())
        {
            char temp;
            while(!ipfile.eof())
            {
                ipfile.get(temp);
                opfile<<char((temp-key)%256);
            }
        }
        else
        {
            cout<<"\n\n\tFILES DIDN'T OPEN!";
            menu();
        }
        
        cout<<"\n\n\t\tDECRYPTION SUCCESSFUL!";
        cout<<"\n\n\t\tFile has been saved with the name "<<opname<<"!\n\n";
        ipfile.close();
        opfile.close();
        menu();
    }
    else
    {
        cout<<"\n\n\t\tBRUTE FORCE ANALYSIS will be done now!\n\n";
        int key = 0;
        while(choice == 'n')
        {
            key++;
            ifstream ipfile;
            //ofstream opfile;
            ipfile.open(fname, ios::in);
            //string opname = "brute-decrypted-" + fname;
            //opfile.open(opname, ios::out);
            if(ipfile.is_open())
            {
                //getline(ipfile, line);
                /*for(int i=0; i<line.length(); i++)
                {
                    cout<<char((line[i]-key)%256);
                }*/
                int counter = 50;
                cout<<"\n\t----------------------------------------------------\n\t";
                while(counter-- && !ipfile.eof())
                {
                    char temp;
                    ipfile.get(temp);
                    cout<<char((temp-key)%256);
                }
                cout<<"\n\n\t\tThe key used is "<<key;
                cout<<"\n\t\tDoes this make sense? (y/n) : ";
                cin>>choice;
            }
            ipfile.close();
        }
        ifstream ipfile;
        ofstream opfile;
        ipfile.open(fname, ios::in);
        string opname = "brute-decrypted-" + fname;
        opfile.open(opname, ios::out);
        if(ipfile.is_open() && opfile.is_open())
        {
            char temp;
            while(!ipfile.eof())
            {
                ipfile.get(temp);
                opfile<<char((temp - key)%256);
            }

            ipfile.close();
            opfile.close();

            cout<<"\n\n\t\tDECRYPTION SUCCESSFUL!";
            cout<<"\n\n\t\tFile has been saved with the name "<<opname<<"!\n\n";
            menu();
        }
        else
        {
            cout<<"\n\n\tFILES DIDN'T OPEN!";
            menu();
        }
        
    }
}

void show_content()
{
    ifstream ipfile;
    string fname;
    cout<<"\t\tEnter the name of the file whose contents you would like to see : ";
    cin>>fname;
    ipfile.open(fname, ios::in);
    if(ipfile.is_open())
    {
        cout<<"\n\tThe contents are : \n\n";
        char temp;
        while(!ipfile.eof())
        {
            ipfile.get(temp);
            cout<<temp;
        }
    }
    else
    {
        cout<<"\n\n\tFILES DIDN'T OPEN!";
        menu();
    }
    
    cout<<"\n\n";   
    ipfile.close();
    menu();
}

void freq_analysis()
{
    //clrscr();
    cout<<"\n\n\n----------------------------------------------------\n\n";
    cout<<"\t\tFREQUENCY ANALYSIS Portal\n\n";
    //cout<<"\t\tDue to many ambiguity, this mode only supports keys that are less than 26.!\n";
    cout<<"----------------------------------------------------\n\n";
    ifstream ipfile;
    ofstream opfile;
    string fname;
    cout<<"\tEnter the name of file to be decrypted: ";
    cin>>fname;
    char choice;
    map <char, int> freqmap;
    ipfile.open(fname, ios::in);
    while(!ipfile.eof())
    {
        char temp;
        ipfile.get(temp);
        if(freqmap.find(temp) == freqmap.end())
            freqmap[temp] = 1;
        else
        {
            freqmap[temp]++;
        }
    }
    ipfile.close();
    map <char, int>::iterator itr;
    vector <pair<char, int>> vec;
    
    for(itr=freqmap.begin(); itr!=freqmap.end(); itr++)
    {
        vec.push_back(make_pair(itr->first, itr->second));
    }

    sort(vec.begin(), vec.end(), cmp);
    /*cout<<endl;
    for(int i = 0; i<vec.size(); i++)
    {
        cout<<"\t\t"<<vec[i].second;
    }*/

    
    int i = -1;
    while(++i<10)
    {
        int key = int((vec[i].first - 'e'))%256;
        if(key<0)
            key += 256;
        ipfile.open(fname, ios::in);
        int counter = 50;
        string d_temp = "";
        cout<<"\n"<<key<<"\t";
        while(counter-- && !ipfile.eof())
        {
            char temp; 
            ipfile.get(temp);
            cout<<char((temp - key)%256); 
        }
        ipfile.close();  
    }
    cout<<"\n\n\tWhich key decrypts most sensible text? : ";
    int key_choice;
    cin>>key_choice;
    ipfile.open(fname, ios::in);
    string opname = "frequencyanalysis-" + fname; 
    opfile.open(opname, ios::out);
    while(!ipfile.eof())
    {
        char temp;
        ipfile.get(temp);
        opfile<<char((temp - key_choice)%256);
    }
    ipfile.close();
    opfile.close();
    cout<<"\n\n\t\tFile has been decrypted and saved with name "<<opname;
    menu();
}

void menu()
{
    cout<<"\n\n\n\t\tMENU:";
    cout<<"\n----------------------------------------------------";
    cout<<"\n\t\tAvailable Choices : \n";
    cout<<"\n\t\t1. Encryption";
    cout<<"\n\t\t2. Decryption";
    cout<<"\n\t\t3. Frequency Analysis";
    cout<<"\n\t\t4. Show file contents";
    cout<<"\n\t\t5. Exit";
    cout<<"\n\n\t\tEnter your choice: ";
    int choice;
    cin>>choice;
    switch(choice)
    {
        case 1: encryption(); break;
        case 2: decryption(); break;
        case 3: freq_analysis(); break;
        case 4: show_content(); break;
        case 5: cout<<"\n\n\t\tI hope you had a great time!\n\t\t\tBYE!\n\n\n"; exit(0);
        default: cout<<"\nEnter a valid choice, please!"; menu();
    }
}

int main()
{
    cout<<"\n\n----------------------------------------------------\n\n";
    cout<<"\t\tWELCOME TO THE CAESAR CIPHER APP!\n\n";
    cout<<"----------------------------------------------------\n\n\n\n";
    menu();
    return 0;
}