#include < graphics.h>
#include < stdio.h>

#include "defs.h"
#include "mytools.c"

#define IsClickedOnFreeCells(c,r) (c>9 && r>45 && c<9+4*68 && r<125)
#define IsClickedOnFilledCells(c,r) (c>358 && r>45 && c<358+4*68 && r<125)

PlayCards FreeCells[4];
PlayCards FilledCells[4];
PlayCards Cards[8][20];

int IndexArray[8]={7,7,7,7,6,6,6,6};
FILE *buffer;

struct CardAttributes Cards13[13]={
    {1,1,26,32},
    {2,1,26,10,26,54},
    {3,2,26,10,26,32,26,54},
    {4,2,13,10,39,10,13,54,39,54},
    {5,3,13,10,39,10,26,32,13,54,39,54},
    {6,4,13,10,39,10,13,32,39,32,13,54,39,54},
    {7,5,13,10,13,32,39,10,39,32,26,21,13,54,39,54},
    {8,5,13,10,7,32,39,10,45,32,26,21,26,43,13,54,39,54},
    {9,5,13,10,10,27,39,10,42,27,26,35,10,46,13,63,42,46,39,63},
    {10,5,13,10,7,27,45,10,39,27,26,18,26,50,13,43,7,58,45,43,39,58},
    {0},
    {0},
    {0}
};


int spade[16][16]={
                0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
                0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,
                1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,
                0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,
                0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,

    };


int ispade[16][16]={
                0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,
                0,1,1,1,0,0,0,1,0,0,1,1,1,1,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,

    };


int diamond[16][16]={
                0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0
    };


int badam[16][16]={
                0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
                0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,
                0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,
                0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0
    };


int kalaver[16][16]={
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,0,
                0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

    };


char *king_logo[30]={

                "    BLLLLLLLLLLLLLLLLLLLLB      ",
                "     BLLLLLLLBLBLBLBLBLBB       ",
                "      BBLBLBLBbBbBbBbBbB        ",
                "       BbBbBbbbbbbbbbbB         ",
                "       BbbbbbBBBBBBBBBB         ",
                "       BBBBBBBWWWWWWWWBB        ",
                "       BYBYBYBWWWWWWWWBYB       ",
                "       BYBYBYBBBBBWBBBBYB       ",
                "       BYBYBYBBWBBBWBBBB        ",
                "       BYBYBYBWWWWBWWWBYB       ",
                "       BYBYBYBWWWWBWWWBYB       ",
                "       BYBYBYBWWWWBWWWBYB       ",
                "       BYBYBYBWBWBBBWWBYB       ",
                "       BYBYBYBWBBWWWBWBYB       ",
                "     BBBYBYBYBWWWBBBWWBBYBB     ",
                "    BYYBYBYBYBWWWWWWWWBYBYYB    ",
                "   BYBYBYBYBYBWBBWWWBWBYBYYB    ",
                "  BBBYBBYYBYBYBBYBYBYBYBYYBBLBB ",
                "BBBLBBYYYBYYBYBYYBYBBBYYBBBLBBRB",
                "LBbLBBBBBYBBYBYYBYBYBLBbBBLBBRBb",
                "LBBBLBBLBBBBBBBYYBYBBLBBBLBBRBBB",
                "BLBBLBbBBLBLBLBBBBBBLBbBLBBRBbbb",
                "BLBbBLBbbBBBBBBLBLBBLBBBLBBRBbBB",
                "BLBBBLBbbbLLLLBBBBBLBbBLBBRBbbbb",
                "BBLBBLBbbbLBBBLLbbBLBBBLBBRBbBBB",
                "LBLBbBLBbbLLLLLLbbBLBBLBBRBbbbbb",
                "BBLBBBLBBBBBBBBBBBLBbBLBBRBbBBBB",
                "LLBLBBLBBRRRBRRRBBLBBBLBBRBbBLLL",
                "BBBLBBLBRRBRRRBRRBLBBLBBRBbBLBBB",
                "LLBLBbBLBRBRBRBRRBLBBLBBRBbBLLBR",
            };



int
main()
{
    int gd=DETECT,gm;
    int choice,ch;


    initgraph(&gd,&gm,"");

    MouseInstall();
    InitEnvironment();
    InitGame();
    DisplayCards();
    showmouseptr();
    StartGame();
    return 0;
}



StartGame()
{
    int mc,mr,mb=0,omc,omr,omb;
    int freecell_no=0,old_freecell_no,filledcell_no;
    int fselected=0,cselected=0,tcselected=0;
    int ci,cj,tci,tcj;
    int s,t,temp;

    showmouseptr();

    while(1)
    {
        while(1)
        {
            getmousepos(&omc,&omr,&omb);
            if(omb==0)
                break;
        }
        while(1)
        {
            getmousepos(&mc,&mr,&mb);
            if((mb & 1)==1)
                break;
        }

        if(IsClickedOnFreeCells(mc,mr))
        {
            old_freecell_no=freecell_no;
            freecell_no=(mc-9)/68;

            if(FreeCells[freecell_no].value!=0)
            {
                if(!fselected && !cselected)
                {
                    fselected=freecell_no+1;
                    hidemouseptr();
                    highlight_fcard(
                      9+freecell_no*68,
                      45,FreeCell[freecell_no].value,
                      FreeCells[freecell_no].shape
                    );
                    showmouseptr();
                    continue;
                }
                if(fselected==freecell_no+1)
                {
                    fselected=0;
                    hidemouseptr();
                    draw_fcard(
                       9+freecell_no*68,
                       45,
                       FreeCells[freecell_no].value,
                       FreeCells[freecell_no].shape,0
                    );
                    showmouseptr();
                    continue;
                }
                else
                {
                    /* error    */
                    /* That move is not possible*/
                    ErrorBox();
                    if(fselected)
                    {
                      fselected=0;
                      hidemouseptr();
                      draw_fcard(9+freecell_no*68,
                      45,
                      FreeCells[freecell_no].value,
                      FreeCells[freecell_no].shape,0);
                      showmouseptr();
                      continue;
                    }
                    if(cselected)
                    {
                      tci=cselected/100-1;
                      tcj=IndexArray[cselected/100-1];
                      cselected=0;
                      hidemouseptr();
                      drawcard(
                        9+tci*79,
                        140+tcj*20,
                        Cards[tci][tcj-1].value,
                        Cards[tci][tcj-1].shape,0
                      );
                      showmouseptr();
                      continue;
                    }
                }
            }
            if(FreeCells[freecell_no].value==0)
            {
                if(fselected)
                {
                   fselected=0;

                   FreeCells[freecell_no].value = 
                    FreeCells[old_freecell_no].value;
                   FreeCells[freecell_no].shape = 
                    FreeCells[old_freecell_no].shape;
                   FreeCells[old_freecell_no].value = 0;
                   FreeCells[old_freecell_no].shape = 
                                NOSHAPE;

                   hidemouseptr();
                   draw_fcard(
                    9+freecell_no*68,
                    45,
                    FreeCells[freecell_no].value,
                    FreeCells[freecell_no].shape,0
                   );
                   clear_fcard(9+old_freecell_no*68,45);
                   showmouseptr();
                   continue;
                }
                if(cselected)
                {
                    ci=cselected/100-1;
                    cj=IndexArray[cselected/100-1];
                    cselected=0;

                    FreeCells[freecell_no].value = 
                        Cards[ci][cj-1].value;
                    FreeCells[freecell_no].shape = 
                        Cards[ci][cj-1].shape;
                    Cards[ci][cj-1].value = 0;
                    Cards[ci][cj-1].shape = NOSHAPE;

                    hidemouseptr();

                    draw_fcard(
                       9+freecell_no*68,
                       45,
                       FreeCells[freecell_no].value,
                       FreeCells[freecell_no].shape,0
                    );
                    clearcard(9+ci*79,140+cj*20);
                    if(IndexArray[ci]!=1)
                       drawcard(
                        9+ci*79,
                        140+(cj-1)*20,
                        Cards[ci][cj-2].value,
                        Cards[ci][cj-2].shape,0
                       );
                    IndexArray[ci]=IndexArray[ci]-1;

                    showmouseptr();
                    continue;
                }
            }
        }

        else
        if((temp=IsClickedOnCards(mc,mr))!=0)
        {
            tcselected=temp;
            tci=tcselected/100-1;
            tcj=IndexArray[tcselected/100-1];

            if(!fselected && !cselected && IndexArray[tci])
            {
                cselected=tcselected;
                hidemouseptr();
                highlight_card(
                    9+tci*79,
                    140+tcj*20,
                    Cards[tci][tcj-1].value,
                    Cards[tci][tcj-1].shape
                );
                showmouseptr();
                continue;
            }

            if(fselected)
            {
                if(IndexArray[tci] != 0)
                {
                    s=FreeCells[fselected-1].shape;
                    t=Cards[tci][tcj-1].shape;
                }

                if(((FreeCells[fselected-1].value+1 == 
                        Cards[tci][tcj-1].value)
                    && (((s==DIAMOND || s==BADAM) &&
                      (t==SPADE || t==KALAVER))
                    || ((s==SPADE || s==KALAVER) && 
                      (t==DIAMOND || t==BADAM))))
                    || (IndexArray[tci]==0))
                {
                   Cards[tci][tcj].value = 
                    FreeCells[fselected-1].value;
                   Cards[tci][tcj].shape = 
                    FreeCells[fselected-1].shape;
                   FreeCells[fselected-1].value = 0;
                   FreeCells[fselected-1].shape = NOSHAPE;
                   IndexArray[tci]=IndexArray[tci]+1;

                   hidemouseptr();
                   clear_fcard(9+(fselected-1)*68,45);
                   drawcard(
                    9+tci*79,
                    140+(tcj+1)*20,
                    Cards[tci][tcj].value,
                    Cards[tci][tcj].shape,0
                   );
                   showmouseptr();
                   fselected=0;
                   continue;
                }
                else
                {
                    /*  error   */
                    /* That move is not possible*/
                    continue;
                }
            }
            if(cselected==tcselected)
            {
                cselected=0;
                hidemouseptr();
                drawcard(
                    9+tci*79,
                    140+tcj*20,
                    Cards[tci][tcj-1].value,
                    Cards[tci][tcj-1].shape,0
                );
                showmouseptr();
                continue;
            }
            if(cselected)
            {
                ci=cselected/100-1;
                cj=IndexArray[cselected/100-1];

                s=Cards[ci][cj-1].shape;
                t=Cards[tci][tcj-1].shape;

                if(((Cards[ci][cj-1].value+1==
                       Cards[tci][tcj-1].value)
                   && (((s==DIAMOND || s==BADAM) && 
                      (t==SPADE || t==KALAVER))
                   || ((s==SPADE || s==KALAVER) && 
                      (t==DIAMOND || t==BADAM))))
                   || (IndexArray[tci]==0))

                {
                    IndexArray[tci]=IndexArray[tci]+1;
                    Cards[tci][tcj].value=
                        Cards[ci][cj-1].value;
                    Cards[tci][tcj].shape=
                        Cards[ci][cj-1].shape;
                    Cards[ci][cj-1].value=0;
                    Cards[ci][cj-1].shape=NOSHAPE;

                    hidemouseptr();

                    drawcard(
                        9+tci*79,
                        140+(tcj+1)*20,
                        Cards[tci][tcj].value,
                        Cards[tci][tcj].shape,
                        0
                    );
                    clearcard(9+ci*79,140+cj*20);
                    if(IndexArray[ci]!=1)
                        drawcard(
                           9+ci*79,
                           140+(cj-1)*20,
                           Cards[ci][cj-2].value,
                           Cards[ci][cj-2].shape,
                           0
                        );
                    IndexArray[ci]=IndexArray[ci]-1;

                    showmouseptr();
                    cselected=0;
                    continue;
                }
                else
                {
                    /*  error   */
                    /*That move is not possible*/
                    continue;
                }
            }
        }

        else
        if(IsClickedOnFilledCells(mc,mr))
        {
            filledcell_no=(mc-358)/68;

            if(fselected)
            {
                s=FreeCells[fselected-1].shape;
                t=FilledCells[filledcell_no].shape;

                if((FilledCells[filledcell_no].value+1==
                    FreeCells[fselected-1].value)
                  && (s==t || 
                    FilledCells[filledcell_no].value==0))
                {
                 FilledCells[filledcell_no].value=
                    FreeCells[fselected-1].value;
                 FilledCells[filledcell_no].shape=
                    FreeCells[fselected-1].shape;
                 FreeCells[fselected-1].value=0;
                 FreeCells[fselected-1].shape=NOSHAPE;

                 hidemouseptr();
                 clear_fcard(9+(fselected-1)*68,45);
                 draw_fcard(
                   358+filledcell_no*68,
                   45,
                   FilledCells[filledcell_no].value,
                   FilledCells[filledcell_no].shape,
                   0
                 );
                 showmouseptr();

                 fselected=0;
                 continue;
                }
                else
                {
                    /*  error   */
                    /*That move is not possible*/
                    continue;
                }
            }
            if(cselected)
            {
                ci=cselected/100-1;
                cj=IndexArray[cselected/100-1];

                s=Cards[ci][cj-1].shape;
                t=FilledCells[filledcell_no].shape;

                if((FilledCells[filledcell_no].value+1==
                        Cards[ci][cj-1].value)
                   && (s==t || 
                     FilledCells[filledcell_no].value==0))
                {
                    FilledCells[filledcell_no].value=
                        Cards[ci][cj-1].value;
                    FilledCells[filledcell_no].shape=
                        Cards[ci][cj-1].shape;
                    Cards[ci][cj-1].value=0;
                    Cards[ci][cj-1].shape=NOSHAPE;

                    hidemouseptr();
                    clearcard(9+ci*79,140+cj*20);
                    if(IndexArray[ci]!=1)
                       drawcard(
                        9+ci*79,
                        140+(cj-1)*20,
                        Cards[ci][cj-2].value,
                        Cards[ci][cj-2].shape,
                        0
                       );
                    IndexArray[ci]=IndexArray[ci]-1;
                    draw_fcard(
                      358+filledcell_no*68,
                      45,
                      FilledCells[filledcell_no].
                                value,
                      FilledCells[filledcell_no].
                                shape,
                      0
                    );
                    showmouseptr();

                    cselected=0;
                    continue;
                }
                else
                {
                    /*  error   */
                    /*That move is not possible*/
                    continue;
                }
            }
        }
        else
        {
            if(fselected || cselected)
            {
                /*  error   */
                /*That move is not possible*/
            }
            continue;
        }
    }
}



InitGame(int gno)
{
    FILE *fp;
    PlayCards a;
    int i;

    fp=fopen("fcellsdb.dat","rb");
    if(fp==NULL)
    {
        printf("not opened");
        exit();
    }

    if(gno==-1)
        gno=0;

    for(i=0;i<4;i++)
    {
        FreeCells[i].shape=NOSHAPE;
        FreeCells[i].value=0;
        FilledCells[i].shape=NOSHAPE;
        FilledCells[i].value=0;
    }

    for(i=0;i<7;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[0][i].value=a.value;
        Cards[0][i].shape=a.shape;
    }
    for(i=0;i<7;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[1][i].value=a.value;
        Cards[1][i].shape=a.shape;
    }
    for(i=0;i<7;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[2][i].value=a.value;
        Cards[2][i].shape=a.shape;
    }
    for(i=0;i<7;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[3][i].value=a.value;
        Cards[3][i].shape=a.shape;
    }
    for(i=0;i<6;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[4][i].value=a.value;
        Cards[4][i].shape=a.shape;
    }
    for(i=0;i<6;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[5][i].value=a.value;
        Cards[5][i].shape=a.shape;
    }
    for(i=0;i<6;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[6][i].value=a.value;
        Cards[6][i].shape=a.shape;
    }
    for(i=0;i<6;i++)
    {
        fread(&a,sizeof(a),1,fp);
        Cards[7][i].value=a.value;
        Cards[7][i].shape=a.shape;
    }
}



MouseInstall()
{
    clrscr();
    if(initmouse()==0)
    {
        printf("Mouse driver not loaded\n");
        getkey();
        exit();
    }
}



InitEnvironment()
{
    int i,j;
    setvdumode(18);

    /* Drawing screen starts here   */
    for(j=0;j<=479;j++)
    for(i=0;i<=639;i++)
        writepixel(i,j,2);

    Window(0,0,639,479,"Free Cell  #12432");

    setcolor(BLACK);
    outtextxy(10,30," Game ");
    outtextxy(60,30," Help ");

    draw_FT_cells();
    draw_king();
}



draw_king()
{
    /*  King logo   */
    draw_king_logo(9+4*68+(358-(9+4*68)-32)/2,45+((125-45)-30)/2,1);

    drawrectangle(303-5,70-5,303+32+5,70-5,LIGHTGREEN);
    drawrectangle(303-5,70-5,303-5,70+30+5,LIGHTGREEN);
    drawrectangle(303+32+5,70-5,303+32+5,70+30+5,BLACK);
    drawrectangle(303-5,70+30+5,303+32+5,70+30+5,BLACK);
}



draw_FT_cells()
{
    int i;
    /*  Free cells  and  Target cells   */
    for(i=9;i<=9+4*68;i++)
    {
        writepixel(i,45,BLACK);
        writepixel(i,125,LIGHTGREEN);

        writepixel(i+358-9,45,BLACK);
        writepixel(i+358-9,125,LIGHTGREEN);
    }
    for(i=45;i<=125;i++)
    {
        /*  FreeCells   */
        writepixel(9,i,BLACK);
        writepixel(9+1*68-1,i,LIGHTGREEN);
        writepixel(9+1*68-2,i,LIGHTGREEN);

        writepixel(9+1*68,i,BLACK);
        writepixel(9+2*68-1,i,LIGHTGREEN);
        writepixel(9+2*68-2,i,LIGHTGREEN);

        writepixel(9+2*68,i,BLACK);
        writepixel(9+3*68-1,i,LIGHTGREEN);
        writepixel(9+3*68-2,i,LIGHTGREEN);

        writepixel(9+3*68,i,BLACK);
        writepixel(9+4*68-1,i,LIGHTGREEN);
        writepixel(9+4*68-2,i,LIGHTGREEN);

        /*  Target Cells    */
        writepixel(358,i,BLACK);
        writepixel(358+1*68-1,i,LIGHTGREEN);
        writepixel(358+1*68-2,i,LIGHTGREEN);

        writepixel(358+1*68,i,BLACK);
        writepixel(358+2*68-1,i,LIGHTGREEN);
        writepixel(358+2*68-2,i,LIGHTGREEN);

        writepixel(358+2*68,i,BLACK);
        writepixel(358+3*68-1,i,LIGHTGREEN);
        writepixel(358+3*68-2,i,LIGHTGREEN);

        writepixel(358+3*68,i,BLACK);
        writepixel(358+4*68-1,i,LIGHTGREEN);
        writepixel(358+4*68-2,i,LIGHTGREEN);
    }
}



DisplayCards()
{
    int i,j;

    for(j=0;j<4;j++)
    for(i=0;i<7;i++)
        drawcard(
            9+j*79,
            140+(i+1)*20,
            Cards[j][i].value,
            Cards[j][i].shape,
            0
        );

    for(;j<8;j++)
    for(i=0;i<6;i++)
        drawcard(
            9+j*79,
            140+(i+1)*20,
            Cards[j][i].value,
            Cards[j][i].shape,
            0
        );
}



drawcard(int sx,int sy,int value,int shape,int bg)
{
    int r,c;
    int i,color=RED;
    int **p;
    char s[4];

    if(shape==DIAMOND)
        p=diamond;
    if(shape==BADAM)
        p=badam;
    if(shape==SPADE)
    {
        color=BLACK;
        p=spade;
    }
    if(shape==KALAVER)
    {
        color=BLACK;
        p=kalaver;
    }

    if(bg==0)
    {
        bg=WHITE;
    }
    else
    {
        if(shape==DIAMOND || shape==BADAM)
        {
            bg=BLACK;
            color=LIGHTCYAN;
        }
        else
        {
            bg=LIGHTCYAN;
            color=WHITE;
        }
    } 

    for(r=sy+3;r<=sy+80-3;r++)
    for(c=sx;c < sx+68;c++)
        writepixel(c,r,bg);

    setcolor(bg);

    line(sx+3,sy,sx+68-3,sy);
    line(sx+2,sy+1,sx+68-2,sy+1);
    line(sx+1,sy+2,sx+68-1,sy+2);
    line(sx+3,sy+80,sx+68-3,sy+80);
    line(sx+2,sy+80-1,sx+68-2,sy+80-1);
    line(sx+1,sy+80-2,sx+68-1,sy+80-2);


    if(bg==WHITE)
        setcolor(BLACK);
    else
        setcolor(color);
    line(sx+3,sy,sx+68-3,sy);
    line(sx+3,sy+80,sx+68-3,sy+80);
    line(sx,sy+3,sx,sy+80-3);
    line(sx+68,sy+3,sx+68,sy+80-3);

    line(sx+3,sy,sx,sy+3);
    line(sx+68-3,sy,sx+68,sy+3);
    line(sx,sy+80-3,sx+3,sy+80);
    line(sx+68,sy+80-3,sx+68-3,sy+80);

    for(i=0;i < Cards13[value-1].st;i++)
    {
        drawimage(
            sx+Cards13[value-1].a[i][0],
            sy+Cards13[value-1].a[i][1],
            p,
            color,
            1
        );
    }
    for(; i < Cards13[value-1].no_of_images;i++)
    {
        drawimage(
            sx+Cards13[value-1].a[i][0],
            sy+Cards13[value-1].a[i][1],
            p,
            color,
            0
        );
    }

    setcolor(color);

    if(value > 1 && value<10)
        sprintf(s,"%d",value);
    else
    switch(value)
    {
        case 1:
                sprintf(s,"A");
                break;
        case 11:
                sprintf(s,"J");
                break;
        case 12:
                sprintf(s,"Q");
                break;
        case 13:
                sprintf(s,"K");
                break;
        case 10:
                sprintf(s,"1");
                break;
    }
    outtextxy(sx+3,sy+5,s);
    if(value==10)
    {
        sprintf(s,"%d",0);
        outtextxy(sx+8,sy+5,s);
    }

    drawimage(sx+3,sy+14,p,color,112);
    for(r=5;r<=22;r++)
    for(c=3;c<=13;c++)
    {
        color=readpixel(sx+c,sy+r);
        writepixel(sx+67-c,sy+80-r,color);
    }
}



drawimage(int c,int r,int a[][16],int color,int dir)
{
    int i,j,ii,jj;

    if(dir==112)
    {
        for(i=0,ii=0;i<16;i+=2,ii++)
        for(j=0,jj=0;j<16;j+=2,jj++)
            if(a[i][j])
                writepixel(c+jj,r+ii,color);
    }
    if(dir==1)
    {
        for(i=0;i<16;i++)
        for(j=0;j<16;j++)
            if(a[i][j])
                writepixel(c+j,r+i,color);
    }
    if(dir==0)
    {
        for(i=15;i>=0;i--)
        for(j=0;j<16;j++)
            if(a[i][j])
                writepixel(c+j,r+15-i,color);
    }
}



clearcard(int sx,int sy)
{
    int c,r;

    for(r=sy;r<=sy+80;r++)
    for(c=sx;c<=sx+68;c++)
        writepixel(c,r,GREEN);
}



highlight_card(int sx,int sy,int value,int shape)
{
    drawcard(sx,sy,value,shape,1);
}



IsClickedOnCards(int mc,int mr)
{
    int i;

    for(i=0;i<8;i++)
    {
        if(
            (mc>9+i*79 
            && mr>140+IndexArray[i]*20 
            && mc<9+i*79+68 
            && mr<140+IndexArray[i]*20+80)
            && (IndexArray[i] != 0)
        )
            return((i+1)*100+IndexArray[i]);
        
        if(IndexArray[i] == 0)
        if(mc>9+i*79 && mr>160 && mc<9+i*79+68 && mr<160+80)
        return((i+1)*100+IndexArray[i]);

    }
    return(0);
}



draw_fcard(int sx,int sy,int value,int shape,int bg)
{
    int i;

    drawcard(sx,sy,value,shape,1);
    drawcard(sx,sy,value,shape,bg);
    for(i=sy;i<=sy+80;i++)
    {
        writepixel(sx,i,BLACK);
        writepixel(sx+68-1,i,LIGHTGREEN);
    }
}



clear_fcard(int sx,int sy)
{
    int i;

    clearcard(sx,sy);
    for(i=sx;i<=sx+68;i++)
    {
        writepixel(i,45,BLACK);
        writepixel(i,125,LIGHTGREEN);
    }
    for(i=45;i<=125;i++)
    {
        writepixel(sx+1,i,BLACK);
        writepixel(sx+68-1,i,LIGHTGREEN);
        writepixel(sx+68,i,LIGHTGREEN);
    }
}



highlight_fcard(int sx,int sy,int value,int shape)
{
    draw_fcard(sx,sy,value,shape,1);
}



draw_king_logo(int sx,int sy,int MAX)
{
    int i,r,c;
    char *kl;

    c=sx;
    r=sy;
    for(i=0;i<30;i++)
    {
        kl=king_logo[i];
        while(*kl)
        {
            switch(*kl)
            {
                case 'B':
                    drawrectangle(c,r,c+MAX,r+MAX,0);
                    break;
                case 'b':
                    drawrectangle(c,r,c+MAX,r+MAX,1);
                    break;
                case 'G':
                    drawrectangle(c,r,c+MAX,r+MAX,2);
                    break;
                case 'L':
                    drawrectangle(c,r,c+MAX,r+MAX,11);
                    break;
                case 'W':
                    drawrectangle(c,r,c+MAX,r+MAX,15);
                    break;
                case 'R':
                    drawrectangle(c,r,c+MAX,r+MAX,5);
                    break;
                case 'Y' :
                    drawrectangle(c,r,c+MAX,r+MAX,14);
                    break;
            }
            kl++;
            c=c+MAX;
        }
        r=r+MAX;
        c=sx;
    }
}



drawrectangle(int sx,int sy,int ex,int ey,int color)
{
    int c,r;

    for(r=sy;r<=ey;r++)
    for(c=sx;c<=ex;c++)
        writepixel(c,r,color);
}



Window(int sx,int sy,int ex,int ey,char *title)
{
    int c,r;

    for(r=sy;r<=22;r++)
    for(c=sx;c<=ex;c++)
        writepixel(c,r,3);
    outtextxy(sx+10,sy+8,title);
}



ErrorBox()
{
    int c,r,b;

    hidemouseptr();
    StoreMap(190,180,450,300);

    drawrectangle(190,180,450,202,3);
    drawrectangle(190,203,450,300,1);
    outtextxy(200,188,"Free Cell");
    outtextxy(240,220,"That move is not possible");

    drawrectangle(300,250,350,252,7);
    drawrectangle(300,250,302,276,7);
    drawrectangle(300,274,350,276,0);
    drawrectangle(348,250,350,276,0);

    outtextxy(320,260,"OK");

    showmouseptr();
    while(1)
    {
        getmousepos(&c,&r,&b);
        if((b & 1 == 1) && (c>302 && c<350) && (r>252 && r<274))
            break;
    }

    hidemouseptr();
    RestoreMap(190,180,450,300);
    showmouseptr();
}



StoreMap(int sx,int sy,int ex,int ey)
{
    FILE *fp;

    struct point
    {
        int color;
    };
    struct point p;

    int r,c;

    fp=fopen("line.scr","wb");
    if(fp==NULL)
    {
        printf("Not enough memory");
        getch();
        exit();
    }

    for(r=sy;r<=ey;r++)
    for(c=sx;c<=ex;c++)
    {
        readpixel(c,r);
        p.color=outregs.h.al;
        fwrite(&p,sizeof(p),1,fp);
    }
    fclose(fp);
}



RestoreMap(int sx,int sy,int ex,int ey)
{
    FILE *fp;

    struct point
    {
        int color;
    };
    struct point p;

    int r,c;

    fp=fopen("line.scr","rb");
    if(fp==NULL)
    {
        printf("Not enough memory");
        getch();
        exit();
    }

    for(r=sy;r<=ey;r++)
    for(c=sx;c<=ex;c++)
    {
        fread(&p,sizeof(p),1,fp);
        writepixel(c,r,p.color);
    }
    fclose(fp);
}


