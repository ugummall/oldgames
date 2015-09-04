#include 

 int max    (int, int);
 int min    (int, int);

 void initGame ();
 void startNewGame (GtkWidget *,    gpointer);

 void setStyleRecursively (GtkWidget *, gpointer);

 GtkWidget* createPixmapGtkWidgetFromXpmFile    (GtkWidget *, char *);
 void createDialogBox (gchar *, GtkSignalFunc, GtkSignalFunc);
 void freeAllChilds (GtkWidget *);
 void writeScore();
 int isGameOver();
 void flipResults();

