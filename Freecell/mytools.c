#include < sys\stat.h>
#include < dos.h>
#include < fcntl.h>
#include "mytools.h"
#include "defs.h"

union REGS inregs,outregs;
struct SREGS sregs;
int scan,ascii;

setvdumode(char mode)
{
    inregs.h.ah=0;
    inregs.h.al=mode;
    int86(0x10,&inregs,&outregs);
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

getmousepos(int *mx,int *my,int *mb)
{
    inregs.x.ax=3;
    int86(0x33,&inregs,&outregs);
    *mb=outregs.x.bx;
    *mx=outregs.x.cx;
    *my=outregs.x.dx;
}

getkey()
{
    if(kbhit())
        getch();
    while(!kbhit())
        ;
    inregs.h.ah=0;
    int86(0x16,&inregs,&outregs);
    scan=outregs.h.ah;
    ascii=outregs.h.al;
}

readpixel(int c,int r)
{
    inregs.h.ah=13;
    inregs.h.bh=0;
    inregs.x.cx=c;
    inregs.x.dx=r;
    int86(0x10,&inregs,&outregs);
    return(outregs.h.al);
}

savescreen()
{
    char far *scr=(char far *)0xA0000000;
    char arr[38400L];
    int out,plane;
    long int i;

    out=open("lines.scr",O_WRONLY | O_BINARY | O_CREAT,S_IWRITE);
    if(out==-1)
    {
        printf("Unable to open file");
        exit();
    }
    for(plane=0;plane<=3;plane++)
    {
        /*  select read mode 0  */
        outportb(GC_INDEX,MODE_INDEX);
        outportb(GC_DATA,0);
        /*set Read Map Register to enable plane for reading*/
        outportb(GC_INDEX,READ_MAP_INDEX);
        outportb(GC_DATA,plane);

        for(i=0;i<=38400L;i++)
            arr[i]=*(scr+i);
        write(out,arr,38400L);
    }
    close(out);
}

restorescreen()
{
    char far *scr=(char far *)0xA0000000;
    char arr[38400L];
    int in,plane,bytes;
    long int i;

    for(i=0;i<=38400L;i++)
        arr[i]=0;

    /*  set write mode 0    */
    outportb(GC_INDEX,MODE_INDEX);
    outportb(GC_DATA,0);
    /*  set Bit Mask Register to all bits   */
    outportb(GC_INDEX,BIT_MASK_INDEX);
    outportb(GC_DATA,0xFF);

    in=open("lines.scr",O_RDONLY | O_BINARY,S_IREAD);

    if(in==-1)
    {
        printf("Unable to open file");
        exit();
    }

    for(plane=0;plane<=3;plane++)
    {
        outportb(SC_INDEX,MAP_MASK_INDEX);

        if(plane==0)
            outportb(SC_DATA,1);
        if(plane==1)
            outportb(SC_DATA,2);
        if(plane==2)
            outportb(SC_DATA,4);
        if(plane==3)
            outportb(SC_DATA,8);

        bytes=read(in,arr,38400L);
        if(bytes==0)
        {
            printf("Read failed");
            exit();
        }

        for(i=0;i<=38400L;i++)
            *(scr+i)=arr[i];

    }

    /*  reset Bit Mask Register to normal   */
    outportb(GC_INDEX,BIT_MASK_INDEX);
    outportb(GC_DATA,0xFF);
    /* reset Map Mask Register to disable all planes    */
    outportb(SC_INDEX,MAP_MASK_INDEX);
    outportb(SC_DATA,0x0F);

    close(in);
}
