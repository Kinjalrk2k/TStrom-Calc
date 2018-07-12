#include <iostream>
#include <conio.h>
#include <stdio.h>
#define _WIN32_WINNT 0x05232
#include <windows.h>
#include "include.cpp"
#include <time.h>
#include <fstream>
#include <limits>

#define speed_sound 332

using namespace std;

void MaximizeOutputWindow(void)
{
    HWND consoleWindow = GetConsoleWindow(); // This gets the value Windows uses to identify your output window
    ShowWindow(consoleWindow, SW_MAXIMIZE); // this mimics clicking on its' maximize button
}

/*  an structure used for utilising date and time  */
struct date_time
{
    int _sec, _min, _hour, _mday, _mon, _year,  _wday;
};

/*  storing all possible data of a thunderstrom, specifically used for storing
    and reading to and from a files */
struct Tstrom
{
    double time_ms, time_s; //  storing times
    double dist_m, dist_km; //  storing distances
    date_time dt;           //  storing the affect time
};

void print_key(position p, char key[size]);

/*  prompt the user for various aspect of a thunderstrom and also return the time concerned  */
double calc_time()
{
    int start_s, stop_s;    //  respective timestamp for lighting and thunder
    position p;     //  position variable useful for printing around the console window

    //  user prompt
    cout<<endl<<" LSHIFT  when you SEE a Thunderstrom (Lighting)";
    p.x=1;
    p.y=0;
    print_key(p, "LSHIFT");
    while(1)
    {
        //  detecting Left Shift
        bool isCLPressed = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
        if(isCLPressed)
        {
            //staring the clock
            start_s=clock();
            break;
        }

    }

    //  user prompt
	cout<<endl<<endl<<" RSHIFT  when you HEAR the Thunderstrom (Thunder)";
	p.y=3;
	print_key(p, "RSHIFT");
	while(1)
    {
        //  detecting Right Shift
        bool isCLPressed = GetAsyncKeyState(VK_RSHIFT) & 0x8000;
        if(isCLPressed)
        {
            //stopping the clock
            stop_s=clock();
            break;
        }

    }

    //  calc the difference in time btw starting and stopping and return it
    return (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000;
}

/*  function to convert from ms to s */
double ms_to_s(double ms)
{
    return ms/1000;
}

/*  function to calculate distance */
double calc_dist(double s)
{
    return s*speed_sound;
}

/*  function to convert from m to km */
double m_to_km(double m)
{
    return m/1000;
}

/*  function to save a thunderstrom info to a file  */
void save_hist(Tstrom T)
{
    time_t t = time(0);   // get current now
    tm* now = localtime(&t);

    //  reading all the time data into the structure
    T.dt._sec=now->tm_sec;
    T.dt._min=now->tm_min;
    T.dt._hour=now->tm_hour;
    T.dt._mday=now->tm_mday;
    T.dt._mon=now->tm_mon;
    T.dt._year=now->tm_year + 1900;
    T.dt._wday=now->tm_wday;

    //  writing structure to file
    fstream f;
    f.open("HistoryFiles\\hist.dat", ios::out | ios::binary | ios::app);
    f.write((char*) & T, sizeof(T));
    f.close();
}

void export_to_csv();

/*  function to read history from the file  */
int read_hist()
{
    //  position variable useful for printing around the console window
    position start;
    start.x=0;
    start.y=7;

    position start2=start, start3=start;

    //  creating a table to view the history
    int colnum=8;

    //  declaring the column head
    char col_head[size][size]={"Rec No.","Date(DD-MM-YY)","Day of Week", "Recorded at(HH:MM:SS)","Time Difference(ms)","Time Difference(s)","Distance(m)","Distance(km)"};

    //  opening the history file to read
    Tstrom T;
    fstream f;
    f.open("HistoryFiles\\hist.dat", ios::in | ios::binary);

    //  checking if the file exsits
    if(!f)
    {
        start.x=1;
        start.y=7;
        print_key(start, "No History Found!");
        _getch();
        Sleep(3000);
        start.y+=3;
        gotoxy(start);
        return 2;
    }

    //  declaring and finding no. of records
    int rownum=0;
    while(f.read((char*) & T, sizeof(T)))
    {
        rownum++;
    }
    rownum++;

    f.close();

    f.open("HistoryFiles\\hist.dat", ios::in | ios::binary);

    //  declaring and getting the collen sizes
    int collen[size]={0};
    get_collen_rowfill(col_head, colnum, collen);

    //  drawing table borders and filling the column head
    drawtable(start, collen, colnum, rownum, EXT);
    fillrow(start2, col_head, collen, colnum, rownum, 0);

    start3.x+=2;
    start3.y+=3;

    //  i is used for record no.
    int i=1;

    //  loop to print the history record
    while(f.read((char*) & T, sizeof(T)))
    {
        gotoxy(1,start3.y);
        cout<<setw(collen[0]-1)<<setfill(' ')<<i;

        gotoxy(collen[0]+2, start3.y);
        cout<<setw(2)<<setfill('0')<<T.dt._mday<<"-";
        cout<<setw(2)<<setfill('0')<<T.dt._mon<<"-";
        cout<<setw(2)<<setfill('0')<<T.dt._year;

        gotoxy(collen[0]+collen[1]+3, start3.y);
        switch(T.dt._wday)
        {
        case 0:
            cout<<"Sunday"; break;
        case 1:
            cout<<"Monday"; break;
        case 2:
            cout<<"Tuesday"; break;
        case 3:
            cout<<"Wednesday"; break;
        case 4:
            cout<<"Thrusday"; break;
        case 5:
            cout<<"Friday"; break;
        case 6:
            cout<<"Saturday"; break;

        }

        gotoxy(collen[0]+collen[1]+collen[2]+4, start3.y);
        cout<<setw(2)<<setfill('0')<<T.dt._hour<<":";
        cout<<setw(2)<<setfill('0')<<T.dt._min<<":";
        cout<<setw(2)<<setfill('0')<<T.dt._sec;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+5, start3.y);
        cout<<T.time_ms;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+collen[4]+6, start3.y);
        cout<<T.time_s;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+collen[4]+collen[5]+7, start3.y);
        cout<<T.dist_m;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+collen[4]+collen[5]+collen[6]+8, start3.y);
        cout<<T.dist_km;

        start3.y+=2;
        i++;
    }
    gotoxy(1,start3.y);

    f.close();

    //  prompt for options
    cout<<endl<<endl<<" CTRL  Export History to a .csv file    DEL  Delete History      ESC  Return to main menu    INSERT  Open Advanced Options Menu";
    position p;
    p.x=1;
    p.y=(rownum*2)+9;
    print_key(p, "CTRL");

    p.x=40;
    print_key(p, "DEL");

    p.x=65;
    print_key(p, "ESC");

    p.x=93;
    print_key(p, "INSERT");

    p.x=0;
    p.y+=3;
    gotoxy(p);

    //  handling options input
    while(1)
    {
        if((GetKeyState(VK_CONTROL) & 0x8000)!=0)
            return 0;
        if((GetKeyState(VK_DELETE) & 0x8000)!=0)
            return 1;
        if((GetKeyState(VK_ESCAPE) & 0x8000)!=0)
            return 2;
        if((GetKeyState(VK_INSERT) & 0x8000)!=0)
            return 3;

    }
}

/*  function to export data file to csv (History)   */
void export_to_csv()
{
    Tstrom T;
    fstream f, f2;
    f.open("HistoryFiles\\hist.dat", ios::in | ios::binary);
    f2.open("HistoryFiles\\History.csv", ios::out);

    //  column headers
    f2<<"Rec No.,Date(DD-MM-YY),Day of Week,Recorded at(HH:MM:SS),Time Difference(ms),Time Difference(s),Distance(m),Distance(km)\n";

    int rownum=0;

    //  i is used as a record number
    int i=1;

    //  loop to read from .dat file and write in .csv file
    while(f.read((char*) & T, sizeof(T)))
    {
        f2<<i<<",";

        f2<<T.dt._mday<<"-";
        f2<<T.dt._mon<<"-";
        f2<<T.dt._year<<",";

        switch(T.dt._wday)
        {
        case 0:
            f2<<"Sunday"; break;
        case 1:
            f2<<"Monday"; break;
        case 2:
            f2<<"Tuesday"; break;
        case 3:
            f2<<"Wednesday"; break;
        case 4:
            f2<<"Thrusday"; break;
        case 5:
            f2<<"Friday"; break;
        case 6:
            f2<<"Saturday"; break;
        }

        f2<<",";

        f2<<T.dt._hour<<":";
        f2<<T.dt._min<<":";
        f2<<T.dt._sec<<",";

        f2<<T.time_ms<<",";

        f2<<T.time_s<<",";

        f2<<T.dist_m<<",";

        f2<<T.dist_km;

        f2<<"\n";

        i++;
    }

    f.close();
    f2.close();
}

void export_to_graph_csv()
{
    Tstrom T;
    fstream f, f2;
    f.open("HistoryFiles\\hist.dat", ios::in | ios::binary);
    f2.open("Graphing\\History.csv", ios::out);

    int i=1;
    while(f.read((char*) & T, sizeof(T)))
    {
        f2<<i<<",";

        f2<<T.dist_m;

        f2<<"\n";

        i++;
    }

    f.close();
    f2.close();
}

void print_key(position p, char key[size])
{
    gotoxy(p);
    cout<<(char)218;
    for(int i=0; i<(strlen(key)); i++)
        cout<<(char)196;
    cout<<(char)191;

    p.y++;  gotoxy(p);
    cout<<(char)179;
    cout<<key;
    cout<<(char)179;

    p.y++;  gotoxy(p);
    cout<<(char)192;
    for(int i=0; i<(strlen(key)); i++)
        cout<<(char)196;
    cout<<(char)217;

}

void print_one_hist(position start, Tstrom T, int i)
{
    position start2=start, start3=start;

    int colnum=8;

    char col_head[size][size]={"Rec No.","Date(DD-MM-YY)","Day of Week", "Recorded at(HH:MM:SS)","Time Difference(ms)","Time Difference(s)","Distance(m)","Distance(km)"};

    int rownum=2;

    int collen[size]={0};
    get_collen_rowfill(col_head, colnum, collen);

    drawtable(start, collen, colnum, rownum, EXT);
    fillrow(start2, col_head, collen, colnum, rownum, 0);

    start3.x+=2;
    start3.y+=3;

        gotoxy(1,start3.y);
        cout<<setw(collen[0]-1)<<setfill(' ')<<i;

        gotoxy(collen[0]+2, start3.y);
        cout<<setw(2)<<setfill('0')<<T.dt._mday<<"-";
        cout<<setw(2)<<setfill('0')<<T.dt._mon<<"-";
        cout<<setw(2)<<setfill('0')<<T.dt._year;

        gotoxy(collen[0]+collen[1]+3, start3.y);
        switch(T.dt._wday)
        {
        case 0:
            cout<<"Sunday"; break;
        case 1:
            cout<<"Monday"; break;
        case 2:
            cout<<"Tuesday"; break;
        case 3:
            cout<<"Wednesday"; break;
        case 4:
            cout<<"Thrusday"; break;
        case 5:
            cout<<"Friday"; break;
        case 6:
            cout<<"Saturday"; break;

        }

        gotoxy(collen[0]+collen[1]+collen[2]+4, start3.y);
        cout<<setw(2)<<setfill('0')<<T.dt._hour<<":";
        cout<<setw(2)<<setfill('0')<<T.dt._min<<":";
        cout<<setw(2)<<setfill('0')<<T.dt._sec;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+5, start3.y);
        cout<<T.time_ms;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+collen[4]+6, start3.y);
        cout<<T.time_s;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+collen[4]+collen[5]+7, start3.y);
        cout<<T.dist_m;

        gotoxy(collen[0]+collen[1]+collen[2]+collen[3]+collen[4]+collen[5]+collen[6]+8, start3.y);
        cout<<T.dist_km;

        start3.y+=2;
    gotoxy(1,start3.y);

}

void hist_adv()
{
    adv:
    system("cls");
    cout<<"Advanced";

    position str, p;
	str.x=0;
	str.y=2;

	int no_items=6;
	char menu_items[no_items][260]={"Show Farthest Distance","Show Nearest Distance","Search a record","Delete a Record","Export to a Graph", "Exit"};
	int menu_id;

	menu_id=menu(str, menu_items, no_items);

	switch(menu_id)
	{
    case 0:
        {
            double max=-3.402823466e+38F;
            fstream f;
            Tstrom T, T1;
            f.open("HistoryFiles\\hist.dat", ios::in | ios::binary);

            int i=0, I;
            while(f.read((char*) & T, sizeof(T)))
            {
                if(T.dist_m>max)
                {
                    max=T.dist_m;
                    T1=T;
                    I=i+1;
                }
                i++;
            }

            position p;
            p.x=0;
            p.y=str.y+7;

            print_one_hist(p, T1, I);
            cout<<endl<<"Press any key to continue...";
            _getch();
            goto adv;
        }
        break;

    case 1:
        {
            double min=3.402823466e+38F;
            fstream f;
            Tstrom T, T1;
            f.open("HistoryFiles\\hist.dat", ios::in | ios::binary);

            int i=0, I;
            while(f.read((char*) & T, sizeof(T)))
            {
                if(T.dist_m<min)
                {
                    min=T.dist_m;
                    T1=T;
                    I=i+1;
                }
                i++;
            }

            position p;
            p.x=0;
            p.y=str.y+7;

            print_one_hist(p, T1, I);
            cout<<endl<<"Press any key to continue...";
            _getch();
            goto adv;
        }
        break;

    case 2:
        {
            int rec_no;
            Tstrom T;
            cout<<"Enter Rec No.: ";
            cin>>rec_no;
            ifstream fio("HistoryFiles\\hist.dat", ios::in | ios::binary);
            ofstream file("HistoryFiles\\temp.dat", ios::out | ios::binary);

            int i=0;
            //while(!fio.eof())
            int chk=0;
            while(fio.read((char*)&T, sizeof(T)))
            {
                loop:
                i++;
                //fio.read((char*)&T, sizeof(T));
                if(i==rec_no)
                {
                    position p;
                    p.x=0;
                    p.y=str.y+7;

                    print_one_hist(p, T, i);
                    chk++;
                    break;
                }

            }
            if(chk==0)
                cout<<endl<<"Record No.: "<<rec_no<<" is invalid";

            cout<<endl<<"Press any key to continue...";
            _getch();
            goto adv;
        }
        break;

    case 3:
        {
            int rec_no;
            Tstrom T;
            cout<<"Enter Rec No.: ";
            cin>>rec_no;
            ifstream fio2("HistoryFiles\\hist.dat", ios::in | ios::binary);
            ofstream file2("HistoryFiles\\temp.dat", ios::out | ios::binary);

            int i2=0;
            //while(!fio.eof())
            int chk=0;
            while(fio2.read((char*)&T, sizeof(T)))
            {
                loop2:
                i2++;
                //fio.read((char*)&T, sizeof(T));
                if(i2==rec_no)
                {
                    chk++;
                    position p;
                    p.x=0;
                    p.y=str.y+7;

                    print_one_hist(p, T, i2);
                    cout<<endl<<"Do you want to delete this record?";

                    p.y=str.y+14;

                    int no_items=2;
                    char menu_items[no_items][260]={"Yes","No"};
                    int menu_id;

                    menu_id=menu(p, menu_items, no_items);

                    switch(menu_id)
                    {
                    case 0:
                        {
                            cout<<endl<<"Record No.: "<<rec_no<<" has been successfully deleted";
                            //goto loop;
                            continue;
                        }
                        break;

                    case 1:
                        break;
                    }

                }
                file2.write((char*)&T, sizeof(T));
            }
            if(chk==0)
                cout<<endl<<"Record No.: "<<rec_no<<" is invalid";
            else

            fio2.close();
            file2.close();

            remove("HistoryFiles\\hist.dat");
            rename("HistoryFiles\\temp.dat","HistoryFiles\\hist.dat");

            cout<<endl<<"Press any key to continue...";
            _getch();
            goto adv;
        }
        break;

    case 4:
        {
            export_to_graph_csv();
            system("Graphing\\plot0.bat");
            system("Graphing\\plot1.bat");
            cout<<endl<<"  Successfully exported to Graphing\\Graph.png";
            cout<<endl<<"  Successfully exported to Graphing\\Graph.svg";
            system("start Graphing");
            Sleep(3000);
            goto adv;
            break;
            goto adv;
        }
        break;
	}

}

int main()
{
    MaximizeOutputWindow();

    system("mkdir Backup");
    system("mkdir Graphing");
    system("mkdir HistoryFiles");

    main_menu:

	system("cls");

	ascii_art("Art\\art.txt", 0, 0);

	position str, p;
	str.x=0;
	str.y=11;

	int no_items=5;
	char menu_items[no_items][260]={"Calculate Lightining Strike Distance","History","Backup","Read from Backup","Exit"};
	int menu_id;

	menu_id=menu(str, menu_items, no_items);

	switch(menu_id)
	{
    case 0:
        {
            again:

                system("cls");

                double time_ms, time_s;
                time_ms=calc_time();
                time_s=ms_to_s(time_ms);

                double dist_m, dist_km;
                dist_m=calc_dist(time_s);
                dist_km=m_to_km(dist_m);

                cout<<endl<<endl<<" Calculated values:";
                cout<<endl<<" Time between Lighting and Thunder: "<<time_ms<<" ms or "<<time_s<<" s";
                cout<<endl<<" Distance of the Lighting Strike: "<<dist_m<<" m or "<<dist_km<<" km";

                Tstrom T;
                T.time_ms=time_ms;
                T.time_s=time_s;
                T.dist_m=dist_m;
                T.dist_km=dist_km;

                save_hist(T);

                cout<<endl<<endl<<endl<<" SPACE  Calculate another Strike      ESC  Return to main menu";
                p.x=1;
                p.y=11;
                print_key(p, "SPACE");

                p.x=38;
                print_key(p, "ESC");

                while(1)
                {
                    if((GetKeyState(VK_SPACE) & 0x8000)!=0)
                        goto again;
                    else if((GetKeyState(VK_ESCAPE) & 0x8000)!=0)
                        goto main_menu;
                }

                break;
        }

    case 1:
        {
            hist_menu:
            system("cls");
            ascii_art("Art\\art2.txt", 0, 0);

            int t;
            t=read_hist();

            switch(t)
            {
            case 0:
                export_to_csv();
                cout<<"Succesfully exported to HistoryFiles\\History.csv";
                system("start HistoryFiles");
                Sleep(3000);
                _getch();
                goto hist_menu;
                break;

            case 1:
                system("del HistoryFiles\\hist.dat");
                cout<<"History deleted successfully!";
                Sleep(3000);
                _getch();
                goto main_menu;
                break;

            case 2:
                goto main_menu;
                break;

            case 3:
                hist_adv();
                goto hist_menu;
                break;

            }

            break;
        }

    case 2:
        {
            cout<<endl;
            export_to_csv();
            export_to_graph_csv();
            system("Graphing\\plot1.bat");

            system("mkdir Backup");
            system("copy Graphing\\Graph.png Backup\\");
            system("copy Graphing\\Graph.svg Backup\\");
            system("copy HistoryFiles\\hist.dat Backup\\");
            system("copy HistoryFiles\\History.csv Backup\\");

            cout<<endl;
            cout<<"Backup Completed! Find nessesary files in the Backup folder";
            system("start Backup");
            cout<<endl<<"Press any key to continue...";
            _getch();
            goto main_menu;
        }
        break;

    case 3:
        {
            cout<<endl<<"Copy the backup files into the \"Backup\" Folder and press ANY KEY to continue...";
            _getch();
            system("copy Backup\\Graph.png Graphing\\");
            system("copy Backup\\Graph.svg Graphing\\");
            system("copy Backup\\hist.dat HistoryFiles\\");
            system("copy Backup\\History.csv HistoryFiles\\");

            cout<<endl;
            cout<<"Backup Transloaded!";
            cout<<endl<<"Press any key to continue...";
            _getch();
            goto main_menu;
        }

    case 4:
        exit(0);

	}

	return 0;
}
