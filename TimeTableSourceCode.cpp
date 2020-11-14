#include<iostream>
#include<vector>
#include<cstring>
#include<random>
#include<time.h>
#include<iomanip>
#include <stdio.h>
#include <windows.h>
#define sleep Sleep
using namespace std;


class subject
{
    char sname[15];
    char scode[10];
    int hourPerWeek;// hour per week -> 5
    int allotTime;

public:
    subject()
    {
        strcpy(sname,"");
        strcpy(scode,"");
        hourPerWeek = 0;
        allotTime = 0; //sum of time alloted by making "parts" of the subject (sub part)
    }

    subject(char* n,char* c,int t)
    {
        strcpy(sname,n);
        strcpy(scode,c);
        hourPerWeek = t;
        allotTime = 0;
    }

    void in()
    {
        cout << "       *******************************\n";
        cout << "        ENTER DETAILS OF THE SUBJECT: " << endl;
        cout << "       *******************************\n\n";
        cout << "            ENTER SUBJECT NAME: ";
        cin >> sname;
        cout << "            ENTER SUBJECT CODE: ";
        cin >> scode;
        cout << "            ENTER HOURS ALLOTTED PER WEEK: ";
        cin >> hourPerWeek;
    }

    void out()
    {
        cout << "   *************************\n";
        cout << "    DETAILS OF THE SUBJECT: \n";
        cout << "   *************************\n\n";
        cout << "        SUBJECT CODE: " << scode << endl;
        cout << "        SUBJECT NAME: " << sname << endl;
        cout << "        HOURS ALLOTTED PER WEEK: " << hourPerWeek << endl;
    }

    int getHours()
    {
        return hourPerWeek;
    }
    
    friend class subPart;
};

class subPart
{
    subject* s;

public:
    subPart(subject &s1)
    {
        s = &s1;
        s->allotTime += 1;
    }

    char* getSubName()
    {
        return s->sname;
    }

    void out()
    {
        s->out();
    }
};

class timetable
{
    vector<subject> sub;
    int hpd; //m for i
    int dpw; //n for j
    int rpd;
    int totalTime;
    subPart *TT[7][24]; // i,j
public:
    timetable()
    {
        totalTime = 0;
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 24; j++)
            {
                TT[i][j] = nullptr;
            }
        }
        
    }

    void in()
    {
        cout << "   *******************************\n";
        cout << "    ENTER DETAILS FOR TIME TABLE:\n";
        cout << "   *******************************\n\n";
        cout << "        ENTER THE HOURS ALLOTTED PER DAY FOR TEACHING: ";
        cin >> hpd;
        cout << "        ENTER DAYS OF TEACHING PER WEEK: ";
        cin >> dpw;
        cout << "        ENTER RECESS HOURS PER DAY.";
        cin >> rpd;
    }

    void out()
    {
        cout << "    DETAILS OF THE TIME TABLE: " << endl;
        cout << "   ============================\n\n";
        cout << "        HOURS ALLOTTED FOR TEACHING PER DAY: " << hpd << endl;
        cout << "        DAYS ALLOTTED FOR TACHING PER WEEK: " << dpw << endl;
        cout << "        TOTAL TIME ALLOTED FOR TEACHING IN A WEEK: " << totalTime << endl;

        if(sub.size()==0)
        {
            cout << "\n        NO SUBJECTS ALLOTTED RIGHT NOW.\n";
        }
        else
        {
            cout << "\n        THE SUBJECTS ALLOTTED TO THE TIME TABLE: \n";
            cout << "       =========================================\n\n";
            for (int i = 0; i < sub.size(); i++)
            {
                cout << "            ";
                sub[i].out();
                cout << endl << endl;
            }
        }
    }

    void addSub()
    {
        subject s;
        s.in();
        if(s.getHours() + totalTime > hpd*dpw )
        {
            cout << "    TIME EXCEEDING TOTAL TIME ASSIGNED FOR TEACHING...CANNOT ADD SUBJECT.\n";
            return;
        }
        sub.push_back(s);
        totalTime += s.getHours();
    }

    void showSub()
    {
        if(sub.size() == 0)
        {
            cout << "\n\n\n    SUBJECT LIST IS EMPTY.\n";
            return;
        }

        cout << "\n\n\n   ===============\n";
        cout << "    SUBJECT LIST:\n";
        cout << "   ===============\n\n";
        for (int i = 0; i < sub.size(); i++)
        {
            cout << "\n\n    SUBJECT " << i+1 << ":\n\n";
            sub[i].out();
        }
        
    }

    void deleteSub()
    {
        bool f = 1;
        for (int i = 0; i < sub.size(); i++)
        {
            cout << "\n\n";
            sub[i].out();
            cout << "\n    ENTER 0 TO DELETE THIS SUBJECT, OR ANY OTHER NUMBER TO CHECK ANOTHER SUBJECT: ";
            cin >> f;
            if(!f)
            {
                totalTime -= sub[i].getHours();
                sub.erase( sub.begin() + i );
                cout << "    SUBJECT DELETED SUCCESSFULLY.\n";
                break;
            }
        }

        if(f)cout << "    END OF SUBJECT LIST.\n\n";
    }

    void createTT();
    void showTT();
};

void timetable::createTT()
{
    for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 24; j++)
            {
                TT[i][j] = nullptr;
            }
        }

    int *assigned = new int[dpw]{0};
    int cur{0};

    for (int i = 0; i < sub.size(); i++)
    {
        vector<int> part(dpw, sub[i].getHours()/dpw);
        for (int j = 0; j < sub[i].getHours()%dpw; j++)
        {
            part[i]++;
        }
        
        int ran;
        srand(time(NULL));
        for (int j = 0; j < part.size(); j++)
        {
            ran = rand()%part.size(); //random value between 0 and partsize-1
            
            int temp = part[ran];
            part[ran] = part[j];
            part[j] = temp;
        }

        cout << endl;

        for (int j = 0; j < part.size(); j++)
        {
            while(part[j]--)
            {
                subPart *sp = new subPart(sub[i]);
                while(assigned[cur] > hpd){cur++;cur = cur%dpw;}
                TT[cur][assigned[cur]] = sp; 
                assigned[cur]++;
                cur++;
                cur = cur%dpw;
                while(assigned[cur] > hpd){cur++;cur = cur%dpw;}
            }
            cur++;
            cur = cur%dpw;
        }
    }

    for (int i = 0; i < dpw; i++)
    {
        int rn;
        for (int j = 0; j < hpd; j++)
        {
            rn = rand()%hpd;

            subPart *sptr = TT[i][j];
            TT[i][j] = TT[i][rn];
            TT[i][rn] = sptr;    
        }
    }   
}

void timetable::showTT()
{
    int halfDay = hpd/2;
    cout << "===========";
    for(int i = 0;i < hpd + rpd; i++)cout << setfill('=') << setw(15) << "=";
    cout << endl << setfill(' ');
    cout << "===========";
    for(int i = 0;i < hpd + rpd; i++)cout << setfill('=') << setw(15) << "=";
    cout << endl << setfill(' ');

    cout << " DAY/PERIOD";
    for (int i = 0; i < halfDay; i++)
    {
        cout << setw(14) << "PERIOD " << i+1;
    }
    for (int i = 0; i < rpd; i++)
    {
        cout << setw(14) << "| RECESS |" << " ";
    }
    for (int i = halfDay; i < hpd; i++)
    {
        cout << setw(14) << "PERIOD " << i+1;
    }

    cout << endl;
    cout << "===========";
    for(int i = 0;i < hpd + rpd; i++)cout << setfill('=') << setw(15) << "=";
    cout << endl;

    for (int i = 0; i < dpw; i++)
    {
        cout << setfill(' ');
        switch(i+1)
        {
            case 1: cout << setw(10) << "MONDAY";
                    break;
            case 2: cout << setw(10) << "TUESDAY";
                    break;
            case 3: cout << setw(10) << "WEDNESDAY";
                    break;
            case 4: cout << setw(10) << "THURSDAY";
                    break;
            case 5: cout << setw(10) << "FRIDAY";
                    break;
            case 6: cout << setw(10) << "SATURDAY";
                    break;
        }
        for (int j = 0; j < halfDay; j++)
        {
            if(TT[i][j] == nullptr)cout << setw(15) << " ";
            else cout << setw(15) << TT[i][j]->getSubName();
        }
        for (int j = 0; j < rpd; j++)
        {
            cout << setw(15) << "| RECESS |";
        }
        for (int j = halfDay; j < hpd; j++)
        {
            if(TT[i][j] == nullptr)cout << setw(15) << " ";
            else cout << setw(15) << TT[i][j]->getSubName();
        }
        cout << endl;
    }
    
    cout << "===========";
    for(int i = 0;i < hpd + rpd; i++)cout << setfill('=') << setw(15) << "=";
    cout << endl;
    cout << "===========";
    for(int i = 0;i < hpd + rpd; i++)cout << setfill('=') << setw(15) << "=";
    cout << endl << setfill(' ');
}

void welcome()
{
    system("CLS");
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << "                              ";
    cout << "   -------------------------------\n";
    cout << "                              ";
    cout << "  ---------------------------------\n";
    cout << "                              ";
    cout << "| |  TIME TABLE CREATION PROGRAM  | |\n";
    cout << "                              ";
    cout << "  ---------------------------------\n";
    cout << "                              ";
    cout << "   -------------------------------\n";


    sleep(3000);
}

void thankYou()
{
    system("CLS");
     cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << "                              ";
    cout << "    =====================================\n";
    cout << "                              ";
    cout << "  =========================================\n";
    cout << "                              ";
    cout << "|| ||  THANK YOU FOR USING OUR PROGRAM  || ||\n";
    cout << "                              ";
    cout << "  =========================================\n";
    cout << "                              ";
    cout << "    =====================================\n";


    sleep(5000);
}

int main()
{
    welcome();

    system("CLS");
    
    int choice;
    timetable Table;

    system("CLS");
    cout << "\n\n\n";
    Table.in();
    do
    {
        system("CLS");
    cout << "\n\n\n";
        cout << "\n\n   ***********************\n";
        cout << "    TIME TABLE MAIN MENU:\n";
        cout << "   ***********************\n\n";
        cout << "    ENTER YOUR CHOICE:\n";
        cout << "        1.ADD A NEW SUBJECT.\n";
        cout << "        2.DELETE A SUBJECT.\n";
        cout << "        3.SHOW ADDED SUBJECTS.\n";
        cout << "        4.CREATE TIMETABLE.\n";
        cout << "        5.EXIT.\n\n";
        cout << "        CHOICE:  ";
        cin >> choice;
        
        switch(choice)
        {
            case 1:
                system("CLS");
                Table.addSub();
                cout << "\n        PRESS ANY KEY TO CONTINUE......";
                _getwche();
                break;
            
            case 2:
                system("CLS");
                Table.deleteSub();
                cout << "\n        PRESS ANY KEY TO CONTINUE......";
                _getwche();
                break;
            
            case 3:
                system("CLS");
                Table.showSub();
                cout << "\n        PRESS ANY KEY TO CONTINUE......";
                _getwche();
                break;

            case 4:
                system("CLS");
                Table.createTT();
                cout << "    CREATING TIME TABLE..";
                sleep(300);
                cout << ".";
                sleep(300);
                cout << ".";
                sleep(300);
                cout << ".";
                system("CLS");
                
                Table.showTT();
                cout << "\n        PRESS ANY KEY TO CONTINUE......";
                _getwche();
                break;
            
            case 5:
                thankYou();
                exit(0);
        }

    } while (choice != 5 );
    
    return 0;
}
