/*                         MAGESCRT.h                             */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#ifdef  EXTERNSCRT
#undef  EXTERNSCRT
#define EXTERNSCRT
#else
#define EXTERNSCRT extern
#endif


typedef struct{
    char* begin;
    char* end;
    char* next;
    char* cursor;
} textblock;

EXTERNSCRT textblock rotoutscratch;

/*prototypes*/
void   inittextblock(textblock*);
void   rewindtextblock(textblock*);
void   buildtextblock(textblock*, size_t);
void   disposetextblock(textblock*);
void   putonetextblockline(textblock*, char*);
void   getonetextblockline(textblock*, char*); /*corrected 070210*/


