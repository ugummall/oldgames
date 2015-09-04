#include "main.h"
#include "support.h"
#include "callbacks.h"
/*#include "mytimer.h"*/



void 
initGame()
{
    GtkWidget *pixmapwidget;
    gint i, j;

    gamerunning = 1;
    blackscore = 2;
    whitescore = 2;
    turn = BLACK;

    writeScore();
/*  startTimer();*/
    g_print("This is setupGame function\n");

    for(i=0; i<7; i++)
    for(j=0; j<7; j++)
    {
        tiles[i][j].player = NOPLAYER;
        freeAllChilds(tiles[i][j].widget);

        if (    i==0 && j==0 || 
            i==6 && j==6 || 
            i==0 && j==6 || 
            i==6 && j==0    )
                    continue;

        pixmapwidget = createPixmapGtkWidgetFromXpmFile (
                        app, 
                        xpm_nonefile
                    );
        gtk_container_add ( 
            GTK_CONTAINER (tiles[i][j].widget), 
            pixmapwidget
        );
    }

    tiles[0][0].player = BLACK;
    tiles[6][6].player = BLACK;
    tiles[0][6].player = WHITE;
    tiles[6][0].player = WHITE;

    pixmapwidget = createPixmapGtkWidgetFromXpmFile (
                    app, xpm_blackfile
                );
    gtk_container_add ( 
        GTK_CONTAINER (tiles[0][0].widget), 
        pixmapwidget
    );
    pixmapwidget = createPixmapGtkWidgetFromXpmFile (
                    app, xpm_blackfile
                );
    gtk_container_add ( 
        GTK_CONTAINER (tiles[6][6].widget), 
        pixmapwidget
    );

    pixmapwidget = createPixmapGtkWidgetFromXpmFile (
                    app, xpm_whitefile
                );
    gtk_container_add ( 
        GTK_CONTAINER (tiles[0][6].widget), 
        pixmapwidget
    );
    pixmapwidget = createPixmapGtkWidgetFromXpmFile (
                    app, xpm_whitefile
                );
    gtk_container_add ( 
        GTK_CONTAINER (tiles[6][0].widget), 
        pixmapwidget
    );
}



void 
startNewGame( GtkWidget *widget,
        gpointer data )
{
    cancelCallback (widget, data);
    initGame();
}



void 
freeAllChilds( GtkWidget *widget )
{
    gtk_container_foreach ( GTK_CONTAINER (widget),
                (GtkCallback) freeChildCallback,
                NULL );
}



GtkWidget * 
createPixmapGtkWidgetFromXpmFile( GtkWidget *widget,
                char *xpm_file )
{
    GdkBitmap *mask;
    GtkWidget *pixmapwidget;
    GdkPixmap *pixmap;

    pixmap = gdk_pixmap_create_from_xpm (widget->window, &mask,
                    NULL,
                    xpm_file);
    pixmapwidget = gtk_pixmap_new (pixmap, mask);
    gtk_widget_show (pixmapwidget);

    return pixmapwidget;
}



void 
createDialogBox( gchar *mesg,
        GtkSignalFunc okFunc,
        GtkSignalFunc cancelFunc )
{
    GtkWidget *dialogbox;
    GtkWidget *pixmapwid;
    GtkWidget *label, *button, *hbox;

    dialogbox = gtk_dialog_new();
    label = gtk_label_new(mesg);
    gtk_box_pack_start( GTK_BOX ( GTK_DIALOG (dialogbox)->vbox), 
            label, FALSE,FALSE, 0);
    gtk_widget_show(label);
    
    button = gtk_button_new_with_label("OK");
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_signal_connect ( GTK_OBJECT (button), "clicked",
            GTK_SIGNAL_FUNC (okFunc), dialogbox);
    gtk_box_pack_start( GTK_BOX (GTK_DIALOG (dialogbox)->action_area),
            button, TRUE, TRUE, 0);
    gtk_widget_show (button);

    button = gtk_button_new ();
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_signal_connect ( GTK_OBJECT (button), "clicked",
            GTK_SIGNAL_FUNC (cancelFunc), dialogbox);
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add ( GTK_CONTAINER (button), hbox);
    gtk_widget_show (hbox);
    pixmapwid = createPixmapGtkWidgetFromXpmFile (app, xpm_whitefile);
    gtk_box_pack_start ( GTK_BOX (hbox), pixmapwid, FALSE, FALSE, 0);
    label = gtk_label_new ("Cancel");
    gtk_box_pack_start ( GTK_BOX (hbox), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    gtk_box_pack_start ( 
            GTK_BOX (GTK_DIALOG (dialogbox)->action_area),
            button, TRUE, TRUE, 0
    );
    gtk_widget_grab_default (button);
    gtk_widget_show(button);
    
    gtk_widget_show (dialogbox);
}



void 
setStyleRecursively( GtkWidget *widget,
        gpointer data )
{
    GtkStyle *style;

    style = (GtkStyle *) data;

    gtk_widget_set_style (widget, style);

    if ( GTK_IS_CONTAINER (widget) )
    {
        gtk_container_foreach ( GTK_CONTAINER (widget),
                    setStyleRecursively,
                    style);
    }
}



int
isGameOver()
{
    int i, j, ii, jj, sr, er, sc, ec;
    
    if (blackscore + whitescore == 49)
    {
        gamerunning = 0;
        selected = 0;
        return 1;
    }
    
    for (i=0; i<7; i++)
    for (j=0; j<7; j++)
    {
        if (tiles[i][j].player == turn)
        {
            sr = max (i-2, 0);
            er = min (i+2, 6);
            sc = max (j-2, 0);
            ec = min (j+2, 6);
            
            for (ii=sr; ii<=er; ii++)
            for (jj=sc; jj<=ec; jj++)
            {
                if (tiles[ii][jj].player == NOPLAYER)
                    return 0;
            }
        }
    }
    
    gamerunning = 0;
    selected = 0;   
    return 1;
}



void
flipResults()
{
    GtkWidget *pixmapwid;
    char *xpm_file;
    gint i, j, empty;
    
    g_print("\nwait to let the user to think\n");
    sleep (1);
    
    empty = 49 - blackscore - whitescore;
    
    for (i=0; i<7; i++)
    for (j=0; j<7; j++)
    {
        freeAllChilds (tiles[i][j].widget);
        if (blackscore)
        {
            blackscore--;
            pixmapwid = createPixmapGtkWidgetFromXpmFile (
                        app,
                        xpm_blackfile
                    );
        }
        else
        if (empty)
        {
            empty--;
            pixmapwid = createPixmapGtkWidgetFromXpmFile (
                        app,
                        xpm_nonefile
                    );
        }
        else
        {
            whitescore--;
            pixmapwid = createPixmapGtkWidgetFromXpmFile (
                        app,
                        xpm_whitefile
                    );
        }
        
        gtk_container_add( 
            GTK_CONTAINER (tiles[i][j].widget),
            pixmapwid
        );
    }
}



void
writeScore()
{
    char score[15];
    
    if (blackscore < 10)
        sprintf(score, "Dark: 0%d | ", blackscore);
    else
        sprintf(score, "Dark: %d | ", blackscore);
    
    gtk_label_set_text ( GTK_LABEL (blackscore_label), score);
    
    if (whitescore < 10)
        sprintf(score, "Light: 0%d | ", whitescore);
    else
        sprintf(score, "Light: %d | ", whitescore);
    
    gtk_label_set_text ( GTK_LABEL (whitescore_label), score);
}



int 
max(int a, int b)
{
    if(a > b)
        return a;
    else
        return b;
}



int 
min(int a, int b)
{
    if(a < b)
        return a;
    else
        return b;
}

