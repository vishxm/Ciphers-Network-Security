/*
Submitted by UI17CO49 Vishwas Modhera

C++ Program to implement 6X6 playfair cipher

Structure
    functions
    - matrix generation
    - encryption
    - decryption 
    - show contents

*/

#include<bits/stdc++.h>
#include<string.h>
#include<fstream>
using namespace std;

//global matrix
char pfMatrix[6][6] = {'0'};
void menu(void);

// playfair matrix generation function
void mat_gen(string key)
{
    //set to keep track of characters visited
    unordered_set <char> charset;

    //this array will have all possible characters in 6x6 playfair matrix
    char ch_array[36] = {'0'};
    for(char l='a'; l<='z'; l++)
        ch_array[l-'a'] = l;

    for(int l=26; l<36; l++)
        ch_array[l] = l-26 + 48;
    
    int n = key.length(), k = 0;
    int i = 0, j = 0;

    //inserting unique characters from key into matrix
    while(k<n)
    {
        if(j==6)
        {   
            i = i + 1;
            j = 0;
        }
        if(i==6)
            break;

        if(charset.find(key[k]) == charset.end())
        {
            charset.insert(key[k]);
            pfMatrix[i][j] = key[k];
            j = j + 1;
            //cout<<"\nInserting "<<key[k]<<" at "<<i<<j-1;
        }
        k = k + 1;
    }

    //this loop to feed remaining characters to the matrix
    k = 0;
    while(i<6)
    {
        while(j<6)
        {
            if(charset.find(ch_array[k]) == charset.end())
            {
                pfMatrix[i][j] = ch_array[k];
                charset.insert(ch_array[k]);
                j++;
            }
            k++;
        }
        j = 0;
        i++;
    }

    //displaying generated matrix 
    cout<<"\n\n\tThe playfair matrix generated is : \n";
    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            cout << "\t" << pfMatrix[i][j];
        }
        cout<<endl;
    }
    cout<<"\n\n";
}

//encryption function
void encryption(string fname)
{
    //cout<<"\n\n\t\tENCRYPTION PORTAL\n";
    ifstream ipfile;
    ipfile.open(fname, ios::in);
    ofstream opfile;
    opfile.open("cipher.txt", ios::out);
    if(ipfile.is_open() && opfile.is_open())
    {
        cout<<"\n\tThe file has been opened!";
        string word;
        while (ipfile >> word)
        {
            string newword = "";
            for(int i=0; i<word.length(); i++)
            {
                if(isalnum(word[i]))
                    newword += word[i];
            }
            word = newword;
            for(int i=0; i<word.length(); i++)
            {
                //converting to lowercase
                if(word[i] >= 65 && word[i] <= 90)
                    word[i] = char(tolower(word[i]));
                else if(word[i] >= 48 && word[i] <= 57)
                {
                    //do nothing because digits
                }
                else if(word[i] >= 97 && word[i] <= 122 )
                {
                    //do nothing because already lowercase
                }
                else
                {
                    //replace with z
                    word[i] = 'z';
                }
            }

            if(word.length()%2 == 1) //if the word is of even length, add a 'z' at the end
            {
                word = word + 'z';
            }

            int k = 0;
            //dividing the word in pairs of 2 alphabets
            while(k<word.length())
            {
                //storing the 2 characters of a word.
                char a = word[k];
                k = k + 1;
                char b = word[k];
                k = k + 1;
                if(a == b)
                    b = 'z';

                //getting the position
                int x1, y1, x2, y2;
                for(int i=0; i<6; i++)
                {
                    for(int j=0; j<6; j++)
                    {
                        if(a == pfMatrix[i][j])
                        {
                            x1 = i;
                            y1 = j;
                            break;
                        }
                    }
                }
                for(int i=0; i<6; i++)
                {
                    for(int j=0; j<6; j++)
                    {
                        if(b == pfMatrix[i][j])
                        {
                            x2 = i;
                            y2 = j;
                            break;
                        }
                    }
                }
                
                char temp1, temp2;
                //now we compare the position of characters in the matrix
                if(x1 == x2) // same row
                {
                    temp1 = pfMatrix[x1][(y1+1)%6];
                    temp2 = pfMatrix[x2][(y2+1)%6];
                }
                else if(y1 == y2) // same column
                {
                    temp1 = pfMatrix[(x1+1)%6][y1];
                    temp2 = pfMatrix[(x2+1)%6][y2];
                }
                else
                {
                    temp1 = pfMatrix[x1][y2];
                    temp2 = pfMatrix[x2][y1];
                }
                opfile<<temp1<<temp2;
            }
            opfile<<" ";
        }
        ipfile.close();
        opfile.close();
        cout<<"\n\n\tThe file has been encrypted and saved with the name \"cipher.txt\"!\n\n";
    }
    else
    {
        cout<<"\n\tThe program to failed to open the file! :((";
    }
}

void decryption(string fname)
{
    cout<<"\n\n\t\tDECRYPTION PORTAL\n";
    ifstream ipfile;
    ofstream opfile;
    ipfile.open(fname, ios::in);
    opfile.open("recovered.txt", ios::out);
    if(ipfile.is_open() && opfile.is_open())
    {
        cout<<"\n\tThe files have been opened!";
        string word;
        while(ipfile >> word)
        {
            int k = 0;
            //dividing word in pairs of 2 for processing
            while(k < word.length())
            {
                char a = word[k];
                k = k + 1;
                char b = word[k];
                k = k + 1;

                //getting the position
                int x1, y1, x2, y2;
                for(int i=0; i<6; i++)
                {
                    for(int j=0; j<6; j++)
                    {
                        if(a == pfMatrix[i][j])
                        {
                            x1 = i;
                            y1 = j;
                            break;
                        }
                    }
                }
                for(int i=0; i<6; i++)
                {
                    for(int j=0; j<6; j++)
                    {
                        if(b == pfMatrix[i][j])
                        {
                            x2 = i;
                            y2 = j;
                            break;
                        }
                    }
                }

                char temp1, temp2;
                //now we compare the position of characters in the matrix
                if(y1 == y2) //same column
                {
                    temp1 = pfMatrix[(x1-1)%6][y1];
                    temp2 = pfMatrix[(x2-1)%6][y2];
                }
                else if(x1 == x2) //same row
                {
                    temp1 = pfMatrix[x1][(y1-1)%6];
                    temp2 = pfMatrix[x2][(y2-1)%6];
                }
                else
                {
                    temp1 = pfMatrix[x1][y2];
                    temp2 = pfMatrix[x2][y1];
                }
                
                opfile << temp1 << temp2;
            }
            opfile<<" ";
        }
        ipfile.close();
        opfile.close();
        cout<<"\n\n\tThe file has been encrypted and saved with the name \"recovered.txt\"!\n\n";
    }
    else
    {
        cout<<"\n\tThe program to failed to open the file! :((";
    }
    menu();
}

//simple function to display the contents of a file
void show_content(string fname)
{
    cout<<"\n\n\t\tVIEW FILE CONTENTS\n\n";
    cout<<"\tShowing the contents of the file named "<< fname <<".......\n\n";
    ifstream ipfile;
    ipfile.open(fname, ios::in);
    while(!ipfile.eof())
    {
        char temp;
        ipfile.get(temp);
        cout<<temp;
    }
    cout<<"\n\n\t\tClosing the file.....";
    ipfile.close();
    menu();
}

void menu()
{    
    cout<<"\n\n\t\t\tMENU";
    cout<<"\n----------------------------------------------------\n";   
    cout<<"\n\tAvailable choices:";
    cout<<"\n\t\t1. Encryption";
    cout<<"\n\t\t2. Decryption";
    cout<<"\n\t\t3. View contents";
    cout<<"\n\t\t4. Exit";
    int choice;
    cout<<"\n\n\t\tEnter your choice : ";
    cin>>choice;
    string key, fname;
    switch (choice)
    {
    case 1:
        //string key, fname;
        cout<<"\n\n\tENCRYPTION PORTAL";
        cout<<"\n\t\tEnter the key for encryption (do not include spaces) : ";
        cin>>key;
        mat_gen(key);
        cout<<"\t\tEnter the name of the file to be encrypted : ";
        cin>>fname;
        encryption(fname);
        break;
    
    case 2:
        //string key, fname;
        cout<<"\n\n\tDECRYPTION PORTAL";
        cout<<"\n\t\tEnter the key for decryption (do not include spaces) : ";
        cin>>key;
        mat_gen(key);
        cout<<"\t\tEnter the name of the file to be decrypted : ";
        cin>>fname;
        decryption(fname);
        break;

    case 3:
        cout<<"\n\n\tFILE-CONTENT-VIEWING PORTAL";
        //string fname;
        cout<<"\n\t\tEnter the file name : ";
        cin>>fname;
        show_content(fname);
        break;

    case 4:
        cout<<"\n\n\tHOPE YOU HAD A GOOD TIME SEEING PLAYFAIR CIPHER APPLICATION!";
        cout<<"\n\n\tHave a good day!\n\n";
        exit(0);
    default:
        cout<<"\n\tPLEASE CHOOSE A VALID OPTION!\n";
        break;
    }
    menu();
}

int main()
{
    cout<<"----------------------------------------------------\n";
    cout<<"\tWELCOME TO 6X6 PLAYFAIR CIPHER PORTAL!\n";
    cout<<"----------------------------------------------------\n";
    menu();
}