#ifndef HAVE_DEFS_H
    #define HAVE_DEFS_H
  #define LEFT 75
  #define RIGHT 77
  #define UP 72
  #define DOWN 80

  #define ESC 1

  #define GC_INDEX 0x3CE
  #define GC_DATA 0x3CF
  #define SC_INDEX 0x3C4
  #define SC_DATA 0x3C5
  #define MODE_INDEX 5

  #define SET_RESET_INDEX 0
  #define SET_RESET_ENABLE_INDEX 1
  
  #define BIT_MASK_INDEX 8
  #define MAP_MASK_INDEX 2

  #define READ_MAP_INDEX 4

  #define NOSHAPE 0
  #define DIAMOND 1
  #define SPADE 2
  #define BADAM 3
  #define KALAVER 4

  #define YES 1
  #define NO 0
  #define EXIT 18

  typedef struct PlayCard
  {
    int value;
    int shape;
  } PlayCards;

  struct CardAttributes
  {
    int no_of_images;
    int st;
    int a[10][2];
  };
#endif  /* end of HAVE_DEFS_H   */