/*    MAGESYNC.h    */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#ifdef EXTERNSYNC
#undef  EXTERNSYNC
#define EXTERNSYNC
#else
#define EXTERNSYNC extern
#endif
 
/*varibles and subroutines needed in more than just MAGESYNC.c routines*/
/*mainly in MUXMMAIN.c and MAGEINIT.c and MAGEDLOG.c */

EXTERNSYNC int Lsockets,Lsocketsame,Lsocketoutactive,Lsockettrigger;
EXTERNSYNC int Lsocketiteminput;/*socket input: do not send back to originator*/
EXTERNSYNC int Lsocketextra; /*flag for send to an extra instance of mage*/
EXTERNSYNC int Lsocketreturn; /*flag for send to request a return connection*/
EXTERNSYNC int Lfinger,Lfingerin; 
  /*ghost of remote cursor when remote cursor in Mage graphics*/
EXTERNSYNC   int Lsocketout,Lsocketoutview,Lsocketoutzoom;
EXTERNSYNC   int Lsocketoutmatrix, Lsocketouttran, Lsocketoutfinger;
EXTERNSYNC   int Lsocketoutcenter, Lsocketoutpick;
EXTERNSYNC   int Lsocketoutidsearch, Lsocketidsearch, Lsocketoutgrapher;
EXTERNSYNC   int Lsocketoutextra,Lsocketoutextraactive,Lsocketoutextragrapher; 
EXTERNSYNC   int Lsocketoutecho;

void adjustzoom(void);
void adjusttran(int);
void adjustview(void);
void syncsocketout(int); /*called from ____MAIN.c/master event loop*/
void syncsocketoutextra(int); /*called from ____MAIN.c/master event loop*/
void syncsocketin(void); /*called from ____MAIN.c/socket event callback*/
void syncsocketinextra(void); /*called from ____MAIN.c/socket event callback*/
#ifdef UNIX_X11
Boolean readsocketworkproc(XtPointer);
Boolean readsocketextraworkproc(XtPointer);
#endif /*UNIX_X11*/

