#include "main.h"
#include "callbacks.h"
#include "support.h"


int
closeApp(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
    return FALSE;
}



void 
newGameCallback( GtkWidget *widget,
        gpointer data )
{
    if(gamerunning)
    {
        createDialogBox("Do you really want to end this game ?",
                startNewGame,
                cancelCallback );
    }
    else
        initGame();
}



void
menuQuit( GtkWidget *widget,
    gpointer data )
{
    if(gamerunning)
    {
        createDialogBox("Do you really want to quit ?",
                gtk_exit,
                cancelCallback );
    }
    else
        gtk_exit(1);
}



void 
itemCallback( GtkWidget *widget,
        gpointer data )
{
    g_print("Menu item selected\n");
}



void 
buttonClicked( GtkWidget *widget, gpointer data)
{
    GtkWidget *pixmapwid;
    struct Tile *tile = (struct Tile *)data;
    gchar *xpm_file;
    int i, j, nr, nc, or, oc, sr, er, sc, ec;

    if(!gamerunning)
        return;
    if ( !selected )
    {
        if ( turn == BLACK && tile->player == BLACK )
        {
            selected = tile->row * 10 + tile->col;
            /*makebox();*/
        }
        if ( turn == WHITE && tile->player == WHITE )
        {
            selected = tile->row * 10 + tile->col;
            /*makebox();*/
        }
        g_print("Selected : %d\n",selected);
    }
    else
    {
        if (tile->player == turn)
        {
            selected = tile->row * 10 + tile->col;
            return;
        }
        
        nr = tile->row;
        nc = tile->col;
        or = selected / 10;
        oc = selected % 10;
        g_print("%d%d -- %d%d\n", or, oc, nr, nc);
        if ( tile->player == NOPLAYER &&
            nr >= max( or - 2, 1) && nr <= min( or + 2, 7) &&
            nc >= max( oc - 2, 1) && nc <= min( oc + 2, 7) )
        {
            g_print("\ncorrectly pressed\n");
            selected = 0;
            xpm_file = (turn) ? xpm_whitefile : xpm_blackfile;
            turn = (turn + 1) % 2;

            sr = ( nr == 1 ) ? 1 : nr - 1;
            er = ( nr == 7 ) ? 7 : nr + 1;
            sc = ( nc == 1 ) ? 1 : nc - 1;
            ec = ( nc == 7 ) ? 7 : nc + 1;

            for (i=sr-1; i


void 
cancelCallback( GtkWidget *widget,
        gpointer data )
{
    gtk_widget_destroy (GTK_WIDGET(data));
}



void 
freeChildCallback( GtkWidget *widget )
{
    gtk_widget_destroy (widget);
}

