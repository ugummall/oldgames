
#include < gtk/gtk.h>
#include "main.h"
#include "callbacks.c"
#include "support.c"
#include "mytimer.c"

struct Tile tiles[7][7];

GtkWidget *app;
GtkWidget *whitescore_label;
GtkWidget *blackscore_label;
GtkWidget *timer_label;
int selected =0;
int gamerunning =0;
int blackscore =0;
int whitescore =0;
int turn =BLACK;
char xpm_whitefile[31] ="whiter.xpm";
char xpm_blackfile[31] ="black.xpm";
char xpm_nonefile[31] ="box.xpm";



GtkWidget *
createMenuBarItem( GtkWidget *menubar,
        gchar *item_label )
{
    GtkWidget *submenu;
    GtkWidget *item;
    
    submenu = gtk_menu_new();
    
    item = gtk_menu_item_new_with_label(item_label);
    gtk_widget_show(item);

    gtk_menu_item_set_submenu( GTK_MENU_ITEM (item), submenu);
    
    gtk_menu_bar_append( GTK_MENU_BAR (menubar), item);

    return submenu;
}



GtkWidget *
createMenuItem( GtkWidget *menu,
        gchar *item_label,
        GtkSignalFunc func,
        gpointer data )
{
    GtkWidget *item;

    if ( item_label == NULL )
        item = gtk_menu_item_new();
    else
    {
        item = gtk_menu_item_new_with_label(item_label);
        gtk_signal_connect( GTK_OBJECT (item), "activate",
                GTK_SIGNAL_FUNC (func), data);  
    }
    gtk_menu_append( GTK_MENU (menu), item);
    gtk_widget_show(item);

    return item;
}



void updateTime( int sec, int min, int hrs)
{
    char timerbuffer[15];
    char s[3],m[3],h[3];
    
    (sec > 9) ? sprintf (s,"%d",sec) : sprintf (s,"0%d",sec);
    (min > 9) ? sprintf (s,"%d",sec) : sprintf (s,"0%d",sec);
    (hrs > 9) ? sprintf (s,"%d",sec) : sprintf (s,"0%d",sec);
        
    sprintf(timerbuffer,"%s:%s:%s",h,m,s);
    gtk_label_set_text ( GTK_LABEL (timer_label), timerbuffer);
}



int
main(int argc, char *argv[])
{
    GtkWidget *vbox, *hbox;
    GtkWidget *mb,*me,*mi;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *table;
    GtkWidget *pixmapwid;
    GdkPixmap *pixmap;
    GtkStyle *style, *defstyle;
    GdkBitmap *mask;

    GdkColor red =    { 0, 0xffff, 0x0000, 0x0000 };
    GdkColor green =  { 0, 0x0000, 0xffff, 0x0000 };
    GdkColor blue =   { 0, 0x0000, 0x0000, 0xffff };
    GdkColor yellow = { 0, 0xffff, 0xffff, 0x0000 };
    GdkColor purple = { 0, 0xffff, 0x0000, 0xffff };
    GdkColor cyan =   { 0, 0x0000, 0xffff, 0xffff };
    GdkColor white =  { 0, 0xffff, 0xffff, 0xffff };
    GdkColor black =  { 0, 0x0000, 0x0000, 0x0000 };

    gint i, j;

    gtk_init(&argc, &argv);

    gdk_color_alloc (gdk_colormap_get_system (), &blue);
    defstyle = gtk_widget_get_default_style ();
    style = gtk_style_copy (defstyle);
    for ( i=0; i<5; i++)
    {
        style->fg[i] = blue;
        style->text[i] = blue;
        style->bg[i] = blue;
    }
    gtk_widget_push_style (style);

    app = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title( GTK_WINDOW (app), "Example");
    gtk_widget_set_usize(app, 420, 450);
    gtk_widget_set_uposition (app, 0, 0);
    gtk_signal_connect( GTK_OBJECT(app), "delete_event",
            GTK_SIGNAL_FUNC (closeApp), NULL);
    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add( GTK_CONTAINER (app), vbox);
    gtk_widget_show(vbox);
    mb = gtk_menu_bar_new();
    gtk_box_pack_start( GTK_BOX (vbox), mb, FALSE, FALSE, 0);
    gtk_widget_show(mb);

    /*  Game Menu   */
    me = createMenuBarItem( mb, (gchar *)"Game");

    createMenuItem( me, NULL, NULL, NULL);
    createMenuItem( 
            me, "    New Game     Ctrl+N ", 
            newGameCallback, NULL
    );
    createMenuItem( me, NULL, NULL, NULL);
    createMenuItem( 
            me, "    Undo Move     Ctrl+Z ", 
            itemCallback, NULL
    );
    createMenuItem( me, NULL, NULL, NULL);
    createMenuItem( 
            me, "    Exit         Ctrl+Q ", 
            menuQuit, NULL
    );


    /*  Settings    Menu    */
    me = createMenuBarItem( mb, "Settings");

    createMenuItem( 
            me, " Preferences... ", 
            GTK_SIGNAL_FUNC (itemCallback),
            NULL
    );

    /*  Help    Menu    */
    me = createMenuBarItem( mb, "Help");

    createMenuItem( me, "    About...    ", itemCallback, NULL);

    /*  ************    */


    table = gtk_table_new(7,7,TRUE);
    gtk_box_pack_start( GTK_BOX (vbox), table, TRUE, TRUE, 0);
    
    for(i=0; i<7; i++)
    for(j=0; j<7; j++)
    {
        button = gtk_button_new();
        (&tiles[i][j])->widget = button;
        (&tiles[i][j])->row = i+1;
        (&tiles[i][j])->col = j+1;
        (&tiles[i][j])->player = NOPLAYER;
        
        pixmapwid = createPixmapGtkWidgetFromXpmFile (
                        app, 
                        xpm_nonefile
                );
        gtk_container_add (
                GTK_CONTAINER (button),
                pixmapwid
        );

        gtk_signal_connect ( 
                GTK_OBJECT (button), 
                "clicked",
                GTK_SIGNAL_FUNC (buttonClicked), 
                &tiles[i][j]
        );
                
        gtk_table_attach_defaults( 
                GTK_TABLE (table), 
                button,
                j, j+1, i, i+1
        );
        setStyleRecursively (button, style);
        gtk_widget_show (button);
    }
    gtk_widget_show(table);
    
    hbox = gtk_hbox_new (FALSE, 0);
    
        label = gtk_label_new ("Configure the application");
    gtk_box_pack_start ( 
        GTK_BOX (hbox), label, 
        FALSE, FALSE, 0
    );
    gtk_widget_show (label);
    
        label = gtk_label_new ("                            ");
    gtk_box_pack_start ( 
        GTK_BOX (hbox), label, 
        FALSE, FALSE, 0
    );
    gtk_widget_show (label);
    
        blackscore_label = gtk_label_new ("Dark: 00 | ");
    gtk_box_pack_start ( 
        GTK_BOX (hbox), blackscore_label, 
        FALSE, FALSE, 0
    );
    gtk_widget_show (blackscore_label);
    
        whitescore_label = gtk_label_new ("Light: 00 | ");
    gtk_box_pack_start ( 
        GTK_BOX (hbox), whitescore_label, 
        FALSE, FALSE, 0
    );
    gtk_widget_show (whitescore_label);
    
        timer_label = gtk_label_new ("00:00:00");
    gtk_box_pack_start ( 
        GTK_BOX (hbox), timer_label, 
        FALSE, FALSE, 0
    );
    gtk_widget_show (label);
    
    gtk_box_pack_start ( 
        GTK_BOX (vbox), hbox, 
        FALSE, FALSE, 0
    );
    gtk_widget_show (hbox);
    
    gtk_widget_pop_style ();
    gtk_widget_show(app);
    
    gtk_main();

    return 0;
}

