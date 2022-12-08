#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

bool gameOver;
///[gameOver] - ar zaidimas veikia ar ne

int width, height;
///[width, height] - lentos aukstis ir ilgis

int x, y, foodX, foodY, score, tscore, nTail, tailX[1000], tailY[1000], timee;
string failas;
char A[100][100];
///[x, y] - galvos koordinates
///[foodX, foodY] - maisto koordinates
///[score] - skaiciuojami taskai
///[nTail] - ilgis uodegos
///[tailX, tailY] - uodegos daliu koordinates
///[timee] - valdo zaidimo greiti
///[tscore] - temporary score kad didinti zaidimo greiti
///[failas] - failo pavadinimas, pagal kurį atidaro žemėlapį ir jį naudoja žaidime

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN} dir;
///[dir] - kryptis judejimo gyvates galvos

void Setup() ///pradiniai zaidimo nustatymai
{
    srand (time(NULL));
    gameOver=false;
    dir=STOP;
    x=width/2;
    y=height/2;
    while(true) ///tikrina ar maistas neatsirado ant sienos, jeigu ant sienos tai is naujo atsiranda kitur
    {
        foodX=rand()%width;
        foodY=rand()%height;
        if(A[foodY][foodX]!='#')
        {
            break;
        }
    }
    score=0;
    nTail=0;
    timee=100;
}

void Draw() ///piesiamas zaidimas consoleje
{
    system("cls"); ///valo konsole, kad galetu perpiesti zemelapi

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++) ///piešia galvą
        {
            if(i==y && j==x)
            {
                cout<<"O";
            }
            else if(i==foodY&&j==foodX) ///piešia maistą
            {
                cout<<"o";
            }
            else
            {
                bool print = false;
                for(int k=0; k<nTail; k++) ///piešia gyvates uodegą
                {
                    if(tailX[k]==j&&tailY[k]==i)
                    {
                        cout<<"*";
                        print = true;
                    }
                }
                if(!print)
                {
                    cout<<A[i][j];
                }
            }
        }
        cout<<endl;
    }
    cout<<"score: "<<score;




    ///grazaus paprasto kvadrato kodas (pradinis kodas, kurį turėjau perrašyti):
/*
    for(int i=0; i<width+2; i++) ///virsutine siena
    {
        cout << "#";
    }
    cout<<endl;

    for(int i=0; i<height; i++) ///tikrina visko koordinates ir pagal tai nustato koki simboli spausdinti
    {
        for(int j=0; j<width+2; j++)
        {
            if(j==0||j==width+1)
            {
                cout<<"#";
            }
            else if(i==y && j==x)
            {
                cout<<"O";
            }
            else if(i==foodY&&j==foodX)
            {
                cout<<"o";
            }
            else
            {
                bool print = false;
                for(int k=0; k<nTail; k++)
                {
                    if(tailX[k]==j&&tailY[k]==i)
                    {
                        cout<<"*";
                        print = true;
                    }
                }
                if(!print)
                {
                    cout<<" ";
                }
            }
        }
        cout<<endl;
    }

    for(int i=0; i<width+2; i++) ///apatine siena
    {
        cout << "#";
    }
    cout<<endl;
    */
}

void Input() ///laukiama paspaudimo zmogaus wasd, kad judinti gyvate
{
    if(_kbhit())///suzinome ar buvo paspaustas mygtukas
    {
        switch(_getch())///issiaiskiname koks mygtukas buvo paspaustas
        {
        case 'a': ///jeigu 'a' tai i kaire kryptis, 'd' i desine ir t.t.
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void Logic() ///zaidimo logika visa
{
    int prevX=tailX[0], prevY=tailY[0], prevX2, prevY2;
    ///naudosime situos prevX ir prevX2 ir t.t., kad padidinti uodega ir gauti jos daliu koordinates
    tailX[0]=x;
    tailY[0]=y;
    for(int i=1; i<nTail; i++)
    {
        prevX2=tailX[i];
        prevY2=tailY[i];
        tailX[i]=prevX;
        tailY[i]=prevY;
        prevX=prevX2;
        prevY=prevY2;
    }

    switch (dir) ///daro judejima pati, tikrina krypti ir judina pagal ja
    {
    case LEFT: ///jeigu i kaire, tai mazina x, i desine didina x ir t.t. Taip pat tikrina ar neatsitrenke i siena
        if(A[y][x-1]!='#')
        {
            x--;
        }
        else
        {
            dir = STOP;
        }
        break;
    case RIGHT:
        if(A[y][x+1]!='#')
        {
            x++;
        }
        else
        {
            dir = STOP;
        }
        break;
    case UP:
        if(A[y-1][x]!='#')
        {
            y--;
        }
        else
        {
            dir = STOP;
        }
        break;
    case DOWN:
        if(A[y+1][x]!='#')
        {
            y++;
        }
        else
        {
            dir = STOP;
        }
        break;
    }

    if(x>=width+1)///tikrina koordinates galvos, jeigu perejo per siena tai padaro, kad islystu kitoje puseje
    {
        x=0;
    }
    else if(x<0)
    {
        x=width-1;
    }
    else if(y>=height)
    {
        y=0;
    }
    else if(y<0)
    {
        y=height-1;
    }

    for(int i=0; i<nTail; i++) ///tikrina ar galvos ir uodegos koordinates sutampa, jeigu taip tai pralaimi zaidima
    {
        if(tailX[i]==x&&tailY[i]==y)
        {
            gameOver=true;
        }
    }

    if(x==foodX&&y==foodY) ///ka daryti jei suvalgo maista
    {
        score++;
        tscore++;
        bool testy = true;
        while(true) ///tikrina ar maistas neatsirado ant sienos, jeigu ant sienos tai is naujo atsiranda kitur  also ant uodegos tikrina
        {
            foodX=rand()%width;
            foodY=rand()%height;
            if(A[foodY][foodX]!='#')
            {
                if(nTail == 0) ///kad nesulauzytu loop tikrinam ar isvis egzistuoja uodega
                {
                    break;
                }
                for(int i=0; i<nTail; i++)
                {
                    if(foodX!=tailX[i]&&foodY!=tailY[i])
                    {
                        testy = false;
                        break;
                    }
                }
                if(testy==false)
                {
                    break;
                }
            }
        }
        nTail++;
    }

    if(tscore>=5&&timee>0)///greitina zaidima jeigu suvalgo 5 maisto
    {
        tscore=0;
        timee=timee-20;
    }
}

void Read()
{
    cout<<"Kuriame zemelapyje norite zaisti?"<<endl<<"[M1]"<<endl<<"[M2]"<<endl;
    cin>>failas;
    failas = failas + ".txt"; ///prideda .txt prie pavadinimo, kad panaudotu failo atidaryme ifstream
    ifstream fd(failas);
    fd>>height>>width;
    cout<<height<<" "<<width<<endl; ///nustato didi zemelapio

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            char mapTile;
            fd>>mapTile;
            switch(mapTile) ///tikrina koki simboli gavo ir pagal tai nustato ar tai tarpas ar tai siena ar tai dar kas
            {
                case '#':
                    A[i][j]='#';
                    break;
                case 'S': ///vietose, kuriu negali pasiekti gyvatele, kad neatsirastu maistas, implementavau, jeigu buciau norejes pakeisti spalvas zaidimo, bet veliau supratau, kaip man stipriai patinka tas konsoles paprastumas, plius sukuriau koda, kuris testuoja visas spalvas ir visus zenkliukus ir supratau, kad labai negraziai viskas atrodo
                    A[i][j]='#'; ///bet koda vistiek palikau, jeigu noreciau kada nors ji visdelto nuspalvinti
                    break;
                default:
                    A[i][j]=' ';
                    break;
            }
        }
    }
}

void Play() ///daro, kad vyktu zaidimas loopindamas viska
{
    while(!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(timee);
    }
}

int main() ///main.... dabar paziurejau i savo praeito zaidimo koda ir supratau, kad net main nera panasus
{
    Read();
    Setup();
    Play();

    cout<<endl;
    for(int i=5; i>0; i--)
    {
        cout<<i<<endl;
        Sleep(1000);
    }
}
