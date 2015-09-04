/*

Minesweeper Game Source Code

Environment:    Console (Graphics Based)
Operating System:   MS-DOS
Programming Language:   C

Source Code

Program : The Minesweeper Game 
Author : G. Uday Kiran Reddy 
Date : Jan 12, 1999 

mines.c

*/

#include <stdio.h>
#include <dos.h>
#include <graphics.h>

#define GC_INDEX 0x3CE
#define GC_DATA 0x3CF
#define SET_RESET_INDEX 0
#define SET_RESET_ENABLE_INDEX 1
#define MODE_INDEX 5
#define BIT_MASK_INDEX 8


#define IsClickedOnBoard(c,r) (c>=1 && c<=8 && r>=1 && r<=8)
#define IsClickedOnReset(mc,mr) (mc>=305 && mc<=336 && mr>=172 && mr<=198)
#define IsClickedOnExit(mc,mr) (mc<100 && mr<100)
#define IsFlagSet(c,r) (a[r-1][c-1][1]=='f')
#define IsUntouched(c,r) (a[r-1][c-1][1]=='u')
#define IsAlreadyClicked(c,r) (a[r-1][c-1][1]=='c')

union REGS inregs,outregs;

int a[8][8][2];
int StackArray[20];
int Top=-1;
char *GameBuffer;
main()


{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"c:\\tc\\bgi");

    InitEnvironment();
    SetGameArray();
    DrawBoard();
    StoreMap(240,220,400,380,GameBuffer);
    showmouseptr();
    Start();
}

InitEnvironment()


{
    if(initmouse()==0)
    {
        printf("Mouse driver not loaded");
        getch();
        exit();
    }

    GameBuffer=(char *)malloc(450);
    if(GameBuffer==NULL)
    {
        printf("Not enough memory");
        getch();
        exit();
    }
}

SetGameArray()


{
    int c,r;
    char *b[8]= {
            "01*3*100",
            "012*2100",
            "00122100",
            "0113*310",
            "01*4**10",
            "013*4210",
            "113*2000",
            "1*211000"
        };

    for(r=0;r<8;r++)
    for(c=0;c<8;c++)
    {
        if(*(b[r]+c)=='*')
            a[r][c][0]='*';
        else
        if(*(b[r]+c)=='0')
            a[r][c][0]='0';
        else
            a[r][c][0]=*(b[r]+c)-48;
        a[r][c][1]='u';
    }
}

Start()


{
    int mb,mc,mr;
    int oldmc,oldmr;
    int oldmm,mm;
    int c,r;
    int stc,str;
    char buffer[1100];

    while(1)
    {
        getmousepos(&mb,&mc,&mr);
        /* if left or right button clicked  */
        if((mb & 1)==1 || (mb & 2)==2)
        {
            c=Calculate(mc,mr);
            r=c/10;
            c=c%10;

            /* Playing the game */
            if(IsClickedOnBoard(c,r))
            {
                /*  if left button clicked then
                    activate the clicked box and 
                    wait for release    */
                if((mb & 1)==1)
                {
                    if(IsFlagSet(c,r))
                        continue;
                    SmilePlease();
                    oldmc=mc;
                    oldmr=mr;
                    StoreMap(
                        240+(c-1)*20+1,
                        220+(r-1)*20+1,
                        240+c*20,220+r*20,
                        buffer
                    );
                    ActivateBox(c,r);
                    oldmm=Calculate(oldmc,oldmr);
                    while((mb & 1)==1)
                    {
                      mm=Calculate(mc,mr);
                      /* if mouse moves out 
                        of current box*/
                      if((oldmm!=mm)&&((mb & 1)==1))
                      {
                              /* if mouse is still 
                            in the game board   */
                      if(IsClickedOnBoard(c,r) && 
                        (mm!=0))
                      {
                       stc=Calculate(oldmc,oldmr);
                       str=stc/10;
                       stc=stc%10;
                       RestoreMap(
                         240+(stc-1)*20+1,
                         220+(str-1)*20+1,
                         240+stc*20,220+str*20,
                         buffer
                         );
                       StoreMap(
                         240+(c-1)*20+1,
                         220+(r-1)*20+1,
                         240+c*20,220+r*20,
                         buffer
                       );
                       if(!IsFlagSet(c,r))
                          ActivateBox(c,r);
                       oldmc=mc;
                       oldmr=mr;
                       oldmm=Calculat(oldmc,oldmr);
                      }
                     else   
                        /* when mouse moves 
                           out of board */
                     {
                       stc=Calculate(oldmc,oldmr);
                       str=stc/10;
                       stc=stc%10;
                       RestoreMap(
                         240+(stc-1)*20+1,
                         220+(str-1)*20+1,
                         240+stc*20,220+str*20,
                         buffer
                       );
                       oldmc=mc;
                       oldmr=mr;
                       break;
                     }
                    }
                    getmousepos(&mb,&mc,&mr);
                    c=Calculate(mc,mr);
                    r=c/10;
                    c=c%10;
                   }
                   NoSmilePlease();
                   showmouseptr();
                   mc=oldmc;
                   mr=oldmr;
                   if(IsClickedOnBoard(c,r))
                   {
                     if(IsFlagSet(c,r))
                        continue;
                     ProcessBoard(mc,mr);
                   }
                   continue;
                }
                /* if right button clicked then
                    set flag    */
                if((mb & 2)==2)
                {
                    if(IsFlagSet(c,r))
                        SetFlag_OFF(c,r);
                    else
                    if(IsUntouched(c,r))
                        SetFlag_ON(c,r);
                    while((mb & 2)==2)
                        getmousepos(&mb,&mc,&mr);
                    continue;
                }
            }
            /* clicking out of game board   */
            /* user wants to reset the game*/
            if(IsClickedOnReset(mc,mr))
            {
                EndGameWaiting(1);
                continue;
            }
            if(IsClickedOnExit(mc,mr))
            {
                setvdumode(3);
                exit();
            }
        }
    }
}

ProcessBoard(int mc,int mr)


{
    int c,r;

    c=mc-240;
    c=c/20;
    c++;

    r=mr-220;
    r=r/20;
    r++;

    switch(a[r-1][c-1][0])
    {
        case '*' :
                EndGame(c,r);
                break;
        case '0' :
                LuckyHint(c,r);
                break;
        default :
                WriteDigit(c,r);
                break;
    }
}

LuckyHint(int c,int r)


{
    int stickybit[8][8];
    int sarr[8][2];
    int count;

    int i,j;

    for(i=0;i<8;i++)
    for(j=0;j<8;j++)
        stickybit[i][j]=0;

    stickybit[r-1][c-1]=1;


    while(1)
    {
        if(c==1 || c==8 || r==1 || r==8)
        {
            if(c==1 && r==1)
            {
                count=3;
                sarr[0][0]=1;sarr[0][1]=2;
                sarr[1][0]=2;sarr[1][1]=2;
                sarr[2][0]=2;sarr[2][1]=1;
            }
            if(c==1 && r==8)
            {
                count=3;
                sarr[0][0]=1;sarr[0][1]=7;
                sarr[1][0]=2;sarr[1][1]=7;
                sarr[2][0]=2;sarr[2][1]=8;
            }
            if(r==1 && c==8)
            {
                count=3;
                sarr[0][0]=7;sarr[0][1]=1;
                sarr[1][0]=7;sarr[1][1]=2;
                sarr[2][0]=8;sarr[2][1]=2;
            }
            if(r==8 && c==8)
            {
                count=3;
                sarr[0][0]=7;sarr[0][1]=8;
                sarr[1][0]=7;sarr[1][1]=7;
                sarr[2][0]=8;sarr[2][1]=7;
            }
            if(c==1 && (r>=2 && r<=7))
            {
                count=5;
                sarr[0][0]=c;sarr[0][1]=r-1;
                sarr[1][0]=c;sarr[1][1]=r+1;
                sarr[2][0]=c+1;sarr[2][1]=r;
                sarr[3][0]=c+1;sarr[3][1]=r-1;
                sarr[4][0]=c+1;sarr[4][1]=r+1;
            }
            if(c==8 && (r>=2 && r<=7))
            {
                count=5;
                sarr[0][0]=c;sarr[0][1]=r-1;
                sarr[1][0]=c;sarr[1][1]=r+1;
                sarr[2][0]=c-1;sarr[2][1]=r;
                sarr[3][0]=c-1;sarr[3][1]=r-1;
                sarr[4][0]=c-1;sarr[4][1]=r+1;
            }
            if(r==1 && (c>=2 && c<=7))
            {
                count=5;
                sarr[0][0]=c-1;sarr[0][1]=r;
                sarr[1][0]=c+1;sarr[1][1]=r;

                sarr[2][0]=c;sarr[2][1]=r+1;
                sarr[3][0]=c-1;sarr[3][1]=r+1;
                sarr[4][0]=c+1;sarr[4][1]=r+1;
            }
            if(r==8 && (c>=2 && c<=7))
            {
                count=5;
                sarr[0][0]=c-1;sarr[0][1]=r;
                sarr[1][0]=c+1;sarr[1][1]=r;

                sarr[2][0]=c;sarr[2][1]=r-1;
                sarr[3][0]=c-1;sarr[3][1]=r-1;
                sarr[4][0]=c+1;sarr[4][1]=r-1;
            }
        }
        else
        {
            count=8;

            sarr[0][0]=c-1;sarr[0][1]=r;
            sarr[1][0]=c+1;sarr[1][1]=r;

            sarr[2][0]=c;sarr[2][1]=r-1;
            sarr[3][0]=c;sarr[3][1]=r+1;

            sarr[4][0]=c-1;sarr[4][1]=r-1;
            sarr[5][0]=c+1;sarr[5][1]=r-1;
            sarr[6][0]=c-1;sarr[6][1]=r+1;
            sarr[7][0]=c+1;sarr[7][1]=r+1;
        }


        for(i=0;i

EndGame(int fc,int fr)


{
    int r,c;

    LooserFace();
    setfillstyle(1,WHITE);
    for(r=1;r<=8;r++)
    {
        for(c=1;c<=8;c++)
        {
            if(IsFlagSet(c,r))
            {
                if(a[r-1][c-1][0]!='*')
                    /* It's a wrong decision    */
                    WriteCrossMark(c,r);
            }
            else
            {
                if(a[r-1][c-1][0]=='*')
                    /* Display Mine Symbol  */
                    WriteMineIcon(c,r,7);
            }
        }
    }

    WriteMineIcon(fc,fr,RED);
    EndGameWaiting(0);
}

EndGameWaiting(int ch)


{
    int mb,mc,mr;
    int flag=1;
    while(1)
    {
        getmousepos(&mb,&mc,&mr);
        if(((mb & 1)==1) && IsClickedOnExit(mc,mr))
        {
            exit();
        }
        if(((mb & 1)==1) && IsClickedOnReset(mc,mr))
        {
            ActivateLogo();
            while((mb & 1)==1)
            {
                getmousepos(&mb,&mc,&mr);
                if(IsClickedOnExit(mc,mr))
                {
                    exit();
                }
                if(IsClickedOnReset(mc,mr))
                {
                    if(flag)
                    {
                        flag=0;
                        ActivateLogo();
                    }
                }
                else
                {
                    if(ch)
                    {
                        DeactivateLogo();
                        return;
                    }
                    if(!flag)
                    {
                        DeactivateLogo();
                        LooserFace();
                        flag=1;
                    }
                }
            }
            if(IsClickedOnReset(mc,mr))
            {
                DeactivateLogo();
                hidemouseptr();
                SetGameArray();
                RestoreMap(240,220,400,380,GameBuffer);
                showmouseptr();
                return;
            }
        }
    }
}

SetFlag_OFF(int c,int r)


{
    a[r-1][c-1][1]='u';

    /* removing flag from game board    */
    hidemouseptr();
    cls(243+(c-1)*20,223+(r-1)*20,240+c*20-3,220+r*20-3,7);
    showmouseptr();
}

SetFlag_ON(int c,int r)


{
    a[r-1][c-1][1]='f';

    hidemouseptr();

    cls(247+(c-1)*20,225+(r-1)*20,253+(c-1)*20,220+r*20-11,4);
    cls(253+(c-1)*20,225+(r-1)*20,253+(c-1)*20,220+r*20-4,0);
    cls(250+(c-1)*20,220+r*20-4,256+(c-1)*20,220+r*20-4,0);

    showmouseptr();
}

ActivateBox(int c,int r)


{
    char s[5];

    if(!IsClickedOnBoard(c,r))
        return;

    hidemouseptr();

    cls(240+(c-1)*20+1,220+(r-1)*20+1,240+c*20,220+(r-1)*20+1,7);
    cls(240+(c-1)*20+1,220+(r-1)*20+1,240+(c-1)*20+1,220+r*20,7);

    cls(240+(c-1)*20+1,220+r*20,240+c*20,220+r*20,7);
    cls(240+c*20,220+(r-1)*20+1,240+c*20,220+r*20,7);

    DottedLine(240+(c-1)*20+1,220+r*20,20,0,0);
    DottedLine(240+c*20,220+(r-1)*20+1,20,1,0);

    showmouseptr();
}

DottedLine(int c,int r,int n,char dir,int color)


{
    int i;

    if(dir)
        for(i=1;i<=n;i++)
        {
            writepixel(c,r,color);
            r=r+2;
            i++;
        }
    else
        for(i=1;i<=n;i++)
        {
            writepixel(c,r,color);
            c=c+2;
            i++;
        }
}

WriteDigit(int c,int r)


{
    char s[5];

    a[r-1][c-1][1]='c';

    hidemouseptr();

    ActivateBox(c,r);

    if(a[r-1][c-1][0]=='0')
        sprintf(s," ");
    else
        sprintf(s,"%d",a[r-1][c-1][0]);

    setcolor(BLUE);
    settextstyle(0,0,1);
    outtextxy(240+(c-1)*20+7,220+(r-1)*20+6,s);

    showmouseptr();
}

WriteCrossMark(int c,int r)


{
    SetFlag_OFF(c,r);
    setcolor(RED);

    hidemouseptr();

    line(240+(c-1)*20+6,220+(r-1)*20+5,240+c*20-6,220+r*20-7);
    line(240+(c-1)*20+6,220+(r-1)*20+6,240+c*20-6,220+r*20-6);
    line(240+(c-1)*20+6,220+(r-1)*20+7,240+c*20-6,220+r*20-5);

    line(240+c*20-6,220+(r-1)*20+5,240+(c-1)*20+6,220+r*20-7);
    line(240+c*20-6,220+(r-1)*20+6,240+(c-1)*20+6,220+r*20-6);
    line(240+c*20-6,220+(r-1)*20+7,240+(c-1)*20+6,220+r*20-5);

    showmouseptr();
}

WriteMineIcon(int c,int r,int color)


{
    int i;

    ActivateBox(c,r);
    hidemouseptr();

    if(color==RED)
      cls(240+(c-1)*20+1,220+(r-1)*20+1,240+c*20-1,220+r*20-1,RED);

    setcolor(BLACK);
    for(i=3;i>=0;i--)
        fillellipse(240+(c-1)*20+10,220+(r-1)*20+10,i,i);

    line(240+(c-1)*20+4,220+(r-1)*20+10,240+c*20-4,220+(r-1)*20+10);
    line(240+(c-1)*20+10,220+(r-1)*20+4,240+(c-1)*20+10,220+r*20-4);

    line(240+(c-1)*20+7,220+(r-1)*20+11,240+c*20-7,220+(r-1)*20+11);
    line(240+(c-1)*20+11,220+(r-1)*20+7,240+(c-1)*20+11,220+r*20-7);

    writepixel(240+(c-1)*20+7,220+(r-1)*20+7,0);

    writepixel(240+c*20-6,220+(r-1)*20+7,0);
    writepixel(240+c*20-7,220+(r-1)*20+8,0);

    writepixel(240+(c-1)*20+7,220+r*20-7,0);

    writepixel(240+c*20-6,220+r*20-7,0);
    writepixel(240+c*20-7,220+r*20-8,0);

    showmouseptr();
}

ActivateLogo()


{
    hidemouseptr();

    /*  setting border in activemode    */
    cls(304,199,337,199,15);
    cls(337,171,337,199,15);
    cls(304,171,338,171,0);
    cls(304,171,304,200,0);

    /* removing circle  */
    setcolor(LIGHTGRAY);
    setfillstyle(1,LIGHTGRAY);
    sector(320,185,0,360,10,10);
    cls(320,185,329,185,LIGHTGRAY);

    /*  drawing circle  */
    setcolor(BLACK);
    setfillstyle(1,YELLOW);
    sector(322,186,0,360,10,10);
    cls(322,186,331,186,YELLOW);

    /* setting eyes */
    cls(318,182,319,183,0);
    cls(325,182,326,183,0);

    /*  setting mouth   */
    line(319,190,325,190);
    line(317,188,319,190);
    line(325,190,327,188);

    showmouseptr();
}

DeactivateLogo()


{
    hidemouseptr();

    /*  setting border in normal mode   */
    cls(304,171,337,171,15);
    cls(304,171,304,199,15);
    cls(304,199,337,199,0);
    cls(337,170,337,200,0);

    /*  removing circle */
    setcolor(LIGHTGRAY);
    setfillstyle(1,LIGHTGRAY);
    sector(322,186,0,360,10,10);
    cls(322,186,331,186,LIGHTGRAY);

    /*  drawing circle  */
    setcolor(BLACK);
    setfillstyle(1,YELLOW);
    sector(320,185,0,360,10,10);
    cls(320,185,329,185,YELLOW);

    /*  setting eyes    */
    cls(316,181,317,182,0);
    cls(323,181,324,182,0);

    /*  setting mouth   */
    line(317,189,323,189);
    line(315,187,317,189);
    line(323,189,325,187);

    showmouseptr();
}

LooserFace()


{
    int i;

    /*  removing normal eyes    */
    cls(316,181,317,182,YELLOW);
    cls(323,181,324,182,YELLOW);

    /*  removing normal mouth   */
    setcolor(YELLOW);
    line(317,189,323,189);
    line(315,187,317,189);
    line(323,189,325,187);

    /*  setting eyes    */
    setcolor(BLACK);
    setfillstyle(1,BLACK);
    sector(317,182,0,360,3,3);  /*  left    */
    sector(323,182,0,360,3,3);  /*  right   */
    setcolor(YELLOW);
    for(i=3;i>=0;i--)
    {
        fillellipse(317,182,i,i);
        fillellipse(323,182,i,i);
    }

    /*  setting mouth   */
    setcolor(BLACK);
    line(317,189,323,189);
    line(315,191,317,189);
    line(323,189,325,191);
}

SmilePlease()


{
    /*  removing normal mouth   */
    setcolor(YELLOW);
    line(317,189,323,189);
    line(315,187,317,189);
    line(323,189,325,187);

    /*  setting eyes    */
    cls(316,182,317,183,BLACK);
    cls(323,182,324,183,BLACK);

    /*  setting mouth   */
    setcolor(BLACK);
    line(318,191,321,191);
    line(318,187,321,187);

    line(318,187,316,189);
    line(321,187,323,189);

    line(318,191,316,189);
    line(321,191,323,189);

    writepixel(317,189,0);
    writepixel(322,189,0);
}

NoSmilePlease()


{
    /*  removing eyes   */
    cls(316,182,317,183,YELLOW);
    cls(323,182,324,183,YELLOW);

    /*  setting eyes    */
    cls(316,181,317,182,BLACK);
    cls(323,181,324,182,BLACK);

    /*  removing mouth  */
    setcolor(YELLOW);
    line(318,191,321,191);
    line(318,187,321,187);

    line(318,187,316,189);
    line(321,187,323,189);

    line(318,191,316,189);
    line(321,191,323,189);

    writepixel(317,189,YELLOW);
    writepixel(322,189,YELLOW);

    /*  setting mouth   */
    setcolor(BLACK);
    line(317,189,323,189);
    line(315,187,317,189);
    line(323,189,325,187);
}

Calculate(int mc,int mr)


{
    int c,r;

    if(mc>240 && mc<401 && mr>220 && mr<381)
    {
        c=mc-240;
        c=c/20;
        c++;

        r=mr-220;
        r=r/20;
        r++;

        return(r*10+c);
    }
    else
        return(0);
}

push(int c,int r)


{
    Top++;
    StackArray[Top]=r*10+c;
}

pop(int *c,int *r)


{
    if(Top==-1)
    {
        *c=-1;
        return;
    }

    *c=StackArray[Top]%10;
    *r=StackArray[Top]/10;
    Top--;
    return;
}

StoreMap(int sx,int sy,int ex,int ey,char *buffer)


{
    int i,j,k=0;

    hidemouseptr();

    for(j=sy;j<=ey;j++)
    for(i=sx;i<=ex;i++)
    {
        readpixel(i,j);
        buffer[k]=outregs.h.al;
        k++;
    }

    showmouseptr();
}

RestoreMap(int sx,int sy,int ex,int ey,char *buffer)


{
    int i,j,k=0;

    hidemouseptr();

    for(j=sy;j<=ey;j++)
    for(i=sx;i<=ex;i++)
    {
        writepixel(i,j,buffer[k]);
        k++;
    }

    showmouseptr();
}

cls(int sx,int sy,int ex,int ey,int color)


{
    int r,c;

    for(r=sy;r<=ey;r++)
    for(c=sx;c<=ex;c++)
        writepixel(c,r,color);
}

setvdumode(int mode)


{
    inregs.h.ah=0;
    inregs.h.al=mode;

    int86(0x10,&inregs,&outregs);
}

readpixel(int c,int r)


{
    inregs.h.ah=0x0D;
    inregs.h.bh=0;
    inregs.x.cx=c;
    inregs.x.dx=r;

    int86(0x10,&inregs,&outregs);
}

initmouse()


{
    inregs.x.ax=0;
    int86(0x33,&inregs,&outregs);
    return(outregs.x.ax);
}

showmouseptr()


{
    inregs.x.ax=1;
    int86(0x33,&inregs,&outregs);
}

hidemouseptr()


{
    inregs.x.ax=2;
    int86(0x33,&inregs,&outregs);
}

getmousepos(int *mb,int *mx,int *my)


{
    inregs.x.ax=3;
    int86(0x33,&inregs,&outregs);
    *mb=outregs.x.bx;
    *mx=outregs.x.cx;
    *my=outregs.x.dx;
}

DrawBoard()


{
    int i;

    cls(225,146,416,396,15);
    cls(225,393,416,396,8);
    cls(413,146,416,396,8);

    cls(225,393,228,393,15);
    cls(225,394,227,394,15);
    cls(225,395,226,395,15);
    cls(225,396,225,396,15);

    cls(413,146,416,146,15);
    cls(413,147,415,147,15);
    cls(413,148,414,148,15);
    cls(413,149,413,149,15);

    cls(229,150,412,392,7);
    cls(241,221,400,380,7); /*game board*/

    cls(237,160,404,210,8);
    cls(240,163,404,210,7);

    cls(246,170,294,200,0); /*Mines*/
    cls(347,170,395,200,0); /*Timer*/

    /*  logo begin  */
    cls(303,170,338,200,0);
    cls(304,171,336,198,15);
    cls(305,172,336,198,7);

    /*  drawing circle  */
    setcolor(BLACK);
    setfillstyle(1,YELLOW);
    sector(320,185,0,360,10,10);
    cls(320,185,329,185,YELLOW);

    /*  eyes    */
    cls(316,181,317,182,0);
    cls(323,181,324,182,0);
    /*  mouth   */
    line(317,189,323,189);
    line(315,187,317,189);
    line(323,189,325,187);
    /* logo end */

    cls(237,208,401,210,15);
    cls(402,160,404,210,15);

    cls(237,208,239,208,8);
    cls(237,209,238,209,8);
    cls(237,210,237,210,8);

    cls(402,160,404,160,8);
    cls(402,161,403,161,8);
    cls(402,162,402,162,8);



    for(i=241;i<=381;i=i+20)
        cls(i,221,i,380,15);
    for(i=221;i<=381;i=i+20)
    {
        cls(241,i-1,400,i-1,0);
        cls(241,i,400,i,15);
    }
    for(i=261;i<=401;i=i+20)
        cls(i-1,221,i-1,380,0);

    cls(237,217,240,384,8);
    cls(237,217,404,220,8);

    cls(401,217,404,384,15);
    cls(237,381,404,384,15);

    cls(237,381,240,381,8);
    cls(237,382,239,382,8);
    cls(237,383,238,383,8);
    cls(237,384,237,384,8);

    cls(401,217,404,217,8);
    cls(401,218,403,218,8);
    cls(401,219,402,219,8);
    cls(401,220,401,220,8);


}

writepixel(int x,int y,int color)


{
    unsigned char far *baseaddress;
    unsigned char pixelmask;
    unsigned int offset,pixelnumber;

    outportb(GC_INDEX,MODE_INDEX);
    outportb(GC_DATA,0);

    baseaddress=MK_FP(0xA000,0);
    offset=y*80+x/8;
    pixelnumber=x % 8;

    pixelmask=0x80>>pixelnumber;
    outportb(GC_INDEX,BIT_MASK_INDEX);
    outportb(GC_DATA,pixelmask);

    outportb(GC_INDEX,SET_RESET_INDEX);
    outportb(GC_DATA,color);

    outportb(GC_INDEX,SET_RESET_ENABLE_INDEX);
    outportb(GC_DATA,0xF);
    baseaddress[offset] |=0xFF;
}
