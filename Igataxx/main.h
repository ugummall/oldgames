#define WHITE 1
#define BLACK 0
#define NOPLAYER -1

#ifndef HAVE_MAIN_H
#define HAVE_MAIN_H

struct Tile 
{
    GtkWidget *widget;
    
    int row;
    int col;
    int player;
};

extern struct Tile tiles[7][7];

extern GtkWidget *app;
extern GtkWidget *blackscore_label;
extern GtkWidget *whitescore_label;
extern GtkWidget *timer_label;
extern int selected ;
extern int gamerunning ;
extern int blackscore ;
extern int whitescore ;
extern int turn ;
extern char xpm_whitefile[31] ;
extern char xpm_blackfile[31] ;
extern char xpm_nonefile[31] ;

#endif /* HAVE_MAIN_H   */