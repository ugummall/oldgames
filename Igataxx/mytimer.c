#include "mytimer.h"

static gint nSeconds = 0;
static gint nMinutes = 0;
static gint nHours = 0;

static gint timer = 0;
static gint bTimerRunning = 0;



int
timerCallback (gpointer data)
{
    nSeconds++;
    
    if (nSeconds == 60)
    {
        nSeconds = 0;
        nMinutes++;
        
        if (nMinutes == 60)
        {
            nMinutes = 0;
            nHours++;
        }
    }
    
    updateTime(nSeconds, nMinutes, nHours);
}



void startTimer ()
{
    if ( !bTimerRunning )
    {
        nSeconds = 0;
        timer = gtk_timeout_add (1000, timerCallback, NULL);
        bTimerRunning = 1;
    }
}



void stopTimer ()
{
    if ( bTimerRunning )
    {
        gtk_timeout_remove (timer);
        bTimerRunning = 0;
    }
}
