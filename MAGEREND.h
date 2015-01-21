/*only MAGEREND.c def(int)es EXTERNREND */
 
#ifdef UNIX_X11
#define FONTSCALE 0.019  /*arbitrary, needs to be adjusted for UNIX*/
#endif  
#ifdef MACINTOSH
#define FONTSCALE 0.019 /*arbitrary, needs to be adjusted for MACINTOSH*/
#endif  
#ifdef PCWINDOWS
#define FONTSCALE 0.019 /*arbitrary, needs to be adjusted for PC*/
#endif  

#define MageFontTableSIZE 128 /*first 128 char of ascii table*/

#ifndef EXTERNREND 
  extern int *MageStrokedChar[MageFontTableSIZE];
  extern int MageCharSp[2][3];
  extern int MageChar_A[7][3];
#else   /*EXTERNREND is defined*/
  int MageCharNp[][3] =   /*Non-printing character*/
  {
      1,   0,   0,  /*RENDERstrokedchar() works on 2 punitive move-draw points*/
      2,   0,   0,  /*end*/
  };
  int MageCharSp[][3] =   /*The space character*/
  {
      1,   0,   0,  /*RENDERstrokedchar() works on 2 punitive move-draw points*/
      2,  15,   0,  /*end*/
  };
  int MageCharLF[][3] =   /*Line Feed*/
  {
      1,   0,   0,  
      2,   0,  53,  /*end, width, height offset*/
  };
  int MageCharCR[][3] =   /*Carriage Return*/
  {
      1,   0,   0,  
      2,   0,  53,  /*end, width, height offset*/
  };

  int MageChar_A[][3] = 
  {
      1,   9,  12, /*po(int), x, y*/
      0,  27,  12, /*cross bar first so*/
      1,   4,   0, /*side stokes cover ends of cross bar*/
      0,  18,  34,
      0,  32,   0,
      2,  35,   0  /*end of char, width, height offset for next char*/
  };
  int MageChar_a[][3] = 
  {
      1,  23,  16, /*loop*/
      0,  18,  13,
      0,   8,  11,
      0,   5,   8,
      0,   4,   5,
      0,   5,   2,
      0,   7,   1,
      0,  12,   0,
      0,  17,   1,
      0,  20,   3,
      0,  23,   8,  
      1,   6,  19, /*top & vertical*/
      0,   8,  23, 
      0,  14,  24,
      0,  20,  23,
      0,  23,  20,
      0,  23,   0,
      0,  26,   0,   
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_B[][3] = 
  {
      1,   6,  34, /*top loop*/
      0,  21,  34,
      0,  25,  33,
      0,  28,  30,
      0,  29,  26,
      0,  28,  22,
      0,  25,  19,
      0,  21,  18,
      1,   6,  18, /*bottom loop*/
      0,  22,  18,
      0,  27,  17,
      0,  30,  14,
      0,  31,   9,
      0,  30,   4,
      0,  27,   1,
      0,  22,   0,
      0,   6,   0,
      0,   6,  34, /*side stroke*/   
      2,  35,   0  /*2=end, width, height offset*/
  };
  int MageChar_b[][3] = 
  {
      1,   5,  12, /*loop*/
      0,   6,  18,
      0,   9,  23,
      0,  15,  24,
      0,  21,  23,
      0,  24,  18,
      0,  25,  12,
      0,  24,   6,
      0,  21,   1,
      0,  15,   0,
      0,   9,   1,
      0,   6,   6,
      0,   5,  12,
      1,   5,  34, /*vertical*/
      0,   5,   0,      
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_C[][3] = 
  {
      1,  33,  26,
      0,  32,  28,
      0,  28,  32,
      0,  23,  34,
      0,  17,  34,
      0,  12,  32,
      0,   8,  28,
      0,   6,  24,
      0,   5,  17,
      0,   6,  10,
      0,   8,   6,
      0,  12,   2,
      0,  17,   0,
      0,  23,   0,
      0,  28,   2,
      0,  32,   6,
      0,  33,  10,

      2,  38,   0  /*end, width, height offset*/
  };
  int MageChar_c[][3] = 
  {
      1,  22,   6,
      0,  20,   1,
      0,  14,   0,
      0,   8,   1,
      0,   5,   6,
      0,   4,  12,
      0,   5,  18,
      0,   8,  23,
      0,  14,  24,
      0,  20,  23,
      0,  22,  18,
      2,  26,   0  /*end, width, height offset*/
  };
  int MageChar_D[][3] = 
  {
      1,   6,   0, /*bottom Left*/
      0,  21,   0, 
      0,  26,   2,
      0,  30,   6,
      0,  32,  10,
      0,  33,  17,
      0,  32,  24,
      0,  30,  28,
      0,  26,  32,
      0,  21,  34,
      0,   6,  34, /*vertical*/
      0,   6,  0,
      2,  38,   0  /*end, width, height offset*/
  };
  int MageChar_d[][3] = 
  {
      1,  23,   6, /*loop*/
      0,  19,   1,
      0,  13,   0,
      0,   7,   1,
      0,   4,   6,
      0,   3,  12,
      0,   4,  18,
      0,   7,  23,
      0,  13,  24,
      0,  19,  23,
      0,  23,  18,
      1,  23,  34, /*vertical*/
      0,  23,   0,
      2,  30,   0  /*end, width, height offset*/
  };
  int MageChar_E[][3] = 
  {
      1,   6,  34, /*top*/
      0,  30,  34,
      1,   6,  18, /*middle*/
      0,  28,  18,
      1,   6,   0, /*bottom*/
      0,  30,   0,
      1,   6,  34, /*vertical*/
      0,   6,   0,
      2,  35,   0  /*end, width, height offset*/
  };
  int MageChar_e[][3] = 
  {
      1,  24,   5, /*loop*/
      0,  20,   1,
      0,  14,   0,
      0,   8,   1,
      0,   5,   6,
      0,   4,  12,
      0,   5,  18,
      0,   8,  23,
      0,  14,  24,
      0,  20,  23,
      0,  23,  18,
      0,  24,  12, /*cross bar*/
      0,   4,  12,
      2,  30,   0  /*end, width, height offset*/
  };
  int MageChar_F[][3] = 
  {
      1,   6,  34, /*top*/
      0,  29,  34,
      1,   6,  18, /*middle*/
      0,  26,  18,
      1,   6,  34, /*vertical*/
      0,   6,   0,
      2,  33,   0  /*end, width, height offset*/
  };
  int MageChar_f[][3] = 
  {
      1,  12,  34,
      0,   9,  34,
      0,   7,  31,
      0,   7,   0,
      1,   2,  24, /*cross bar*/
      0,  12,  24,
      2,  15,   0  /*end, width, height offset*/
  };
  int MageChar_G[][3] = 
  {
      1,  33,  26, /*C*/
      0,  32,  28,
      0,  28,  32,
      0,  23,  34,
      0,  17,  34,
      0,  12,  32,
      0,   8,  28,
      0,   6,  24,
      0,   5,  17,
      0,   6,  10,
      0,   8,   6,
      0,  12,   2,
      0,  17,   0,
      0,  23,   0,
      0,  28,   2,
      0,  32,   6,
      0,  34,  10, /*O*/
      1,  22,  16, /*lip*/
      0,  34,  16,
      0,  34,   0,

      2,  38,   0  /*end, width, height offset*/
  };
  int MageChar_g[][3] = 
  {
      1,  23,   6, /*loop*/
      0,  19,   1,
      0,  13,   0,
      0,   7,   1,
      0,   4,   6,
      0,   3,  12,
      0,   4,  18,
      0,   7,  23,
      0,  13,  24,
      0,  19,  23,
      0,  23,  18,
      1,  23,  23, /*vertical*/
      0,  23,   0,
      0,  23,  -4, /*hook*/
      0,  19, -10,
      0,  13, -11,
      0,   7, -10,
      0,   5,  -7,

      2,  30,   0  /*end, width, height offset*/
  };
  int MageChar_H[][3] = 
  {
      1,   6,  18, /*cross*/
      0,  32,  18,
      1,   6,  34, /*left*/
      0,   6,   0,
      1,  32,  34, /*right*/
      0,  32,   0,
      2,  37,   0  /*end, width, height offset*/
  };
  int MageChar_h[][3] = 
  {
      1,   5,  14, /*hump*/
      0,   8,  20,
      0,  16,  24,
      0,  21,  22,
      0,  23,  18,
      0,  23,   0,
      1,   5,  34, /*vertical*/
      0,   5,   0,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_I[][3] = 
  {
      1,   4,  34, /*top serif*/
      0,  10,  34,
      1,   4,   0, /*bottom*/
      0,  10,   0,
      1,   7,  34, /*vertical*/
      0,   7,   0,
      2,  15,   0  /*end, width, height offset*/
  };
  int MageChar_i[][3] = 
  {
      1,   6,  23,
      0,   6,   0,
      
      1,   6,  34, /*dot*/
      0,   6,  32,
      2,  12,   0  /*end, width, height offset*/
  };
  int MageChar_J[][3] = 
  {
      1,   4,   8, /*bottom left*/
      0,   4,   5,
      0,   8,   1,
      0,  12,   0,
      0,  17,   1,
      0,  20,   5,
      0,  20,  34,
      2,  25,   0  /*end, width, height offset*/
  };
  int MageChar_j[][3] = 
  {
      1,   6,  23,
      0,   6,  -9,
      0,   3, -11, /*hook*/
      0,   1, -11,
      
      1,   6,  34, /*dot*/
      0,   6,  32,
      2,  12,   0  /*end, width, height offset*/
  };
  int MageChar_K[][3] = 
  {
      1,  16,  20, /*low leg*/
      0,  30,   0,
      
      1,   6,  11, /*high leg*/
      0,  30,  34,
      
      1,   6,  34, /*side stroke*/
      0,   6,   0,
      2,  35,   0  /*end, width, height offset*/
  };
  int MageChar_k[][3] = 
  {
      1,  12,  15, /*low leg*/
      0,  21,   0,
      
      1,   5,   9, /*high leg*/
      0,  21,  24,
      
      1,   5,  34, /*side stroke*/
      0,   5,   0,
      2,  27,   0  /*end, width, height offset*/
  };
 int MageChar_L[][3] = 
  {
      1,   6,  34,
      0,   6,   0,
      0,  26,   0,
      2,  31,   0  /*end, width, height offset*/
  };
 int MageChar_l[][3] = 
  {
      1,   5,  34,
      0,   5,   0,
      2,  12,   0  /*end, width, height offset*/
  };
 int MageChar_M[][3] = 
  {
      1,   6,   0,
      0,   6,  34,
      0,   9,  34, /*shoulder*/
      0,  22,   0,
      0,  35,  34,
      0,  38,  34, /*shoulder*/
      0,  38,   0,
      2,  43,   0  /*end, width, height offset*/
  };
 int MageChar_m[][3] = 
  {
      1,  21,  14, /*2nd hump*/
      0,  24,  20,
      0,  27,  23,
      0,  32,  24,
      0,  35,  23,
      0,  37,  20,
      0,  37,   0,
      1,   5,  14, /*1st hump*/
      0,   8,  20,
      0,  11,  23,
      0,  16,  24,
      0,  19,  23,
      0,  21,  20,
      0,  21,   0,
      1,   5,  24, /*vertical*/
      0,   5,   0,
      2,  44,   0  /*end, width, height offset*/
  };
 int MageChar_N[][3] = 
  {
      1,   6,   0,
      0,   6,  34,
      0,   8,  34, /*shoulder*/
      0,  29,   0,
      0,  31,   0, /*shoulder*/
      0,  31,  34, 
      2,  38,   0  /*end, width, height offset*/
  };
 int MageChar_n[][3] = 
  {
      1,   5,  14, /*hump*/
      0,   8,  20,
      0,  11,  23,
      0,  16,  24,
      0,  21,  22,
      0,  23,  18,
      0,  23,   0,
      1,   5,  24, /*vertical*/
      0,   5,   0,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_O[][3] = 
  {
      1,  23,   0, /*bottom*/
      0,  28,   2,
      0,  32,   6,
      0,  34,  10,
      0,  35,  17,
      0,  34,  24,
      0,  32,  28,
      0,  28,  32,
      0,  23,  34,
      0,  17,  34,
      0,  12,  32,
      0,   8,  28,
      0,   6,  24,
      0,   5,  17,
      0,   6,  10,
      0,   8,   6,
      0,  12,   2,
      0,  17,   0,
      0,  23,   0,
      2,  39,   0  /*end, width, height offset*/
  };
  int MageChar_o[][3] = 
  {
      1,  23,   5, /*loop*/
      0,  20,   1,
      0,  14,   0,
      0,   8,   1,
      0,   5,   6,
      0,   4,  12,
      0,   5,  18,
      0,   8,  23,
      0,  14,  24,
      0,  20,  23,
      0,  23,  18,
      0,  24,  12,
      0,  23,   5,
      2,  30,   0  /*end, width, height offset*/
  };
  int MageChar_P[][3] = 
  {
      1,   7,  34, /*loop*/
      0,  21,  34,
      0,  26,  33,
      0,  29,  30,
      0,  30,  25,
      0,  29,  20,
      0,  26,  17,
      0,  21,  16,
      0,   7,  16,
      1,   7,  34, /*side*/
      0,   7,   0,
      2,  36,   0  /*end, width, height offset*/
  };
  int MageChar_p[][3] = 
  {
      1,   5,  18, /*loop*/
      0,   8,  23,
      0,  14,  24,
      0,  20,  23,
      0,  23,  18,
      0,  24,  12,
      0,  23,   5,
      0,  20,   1,
      0,  14,   0,
      0,   8,   1,
      0,   5,   6,
      1,   5,  23, /*vertical*/
      0,   5, -11,
      2,  30,   0  /*end, width, height offset*/
  };
  int MageChar_Q[][3] = 
  {
      1,  23,   0, /*bottom*/
      0,  28,   2,
      0,  32,   6,
      0,  34,  10,
      0,  35,  17,
      0,  34,  24,
      0,  32,  28,
      0,  28,  32,
      0,  23,  34,
      0,  17,  34,
      0,  12,  32,
      0,   8,  28,
      0,   6,  24,
      0,   5,  17,
      0,   6,  10,
      0,   8,   6,
      0,  12,   2,
      0,  17,   0,
      0,  23,   0,
      1,  24,   6,
      0,  35,  -2,
      2,  39,   0  /*end, width, height offset*/
  };
  int MageChar_q[][3] = 
  {
      1,  23,   5, /*loop*/
      0,  20,   1,
      0,  14,   0,
      0,   8,   1,
      0,   5,   6,
      0,   4,  12,
      0,   5,  18,
      0,   8,  23,
      0,  14,  24,
      0,  20,  23,
      0,  23,  18,
      1,  23,  23, /*vertical*/
      0,  23, -11,
      2,  30,   0  /*end, width, height offset*/
  };
  int MageChar_R[][3] = 
  {
      1,  21,  16, /*leg*/
      0,  27,  15,
      0,  30,  12,
      0,  31,   2,
      0,  32,   0,
      1,   7,  34, /*loop*/
      0,  21,  34,
      0,  26,  33,
      0,  29,  30,
      0,  30,  25,
      0,  29,  20,
      0,  26,  17,
      0,  21,  16,
      0,   7,  16,
      1,   7,  34, /*side*/
      0,   7,   0,
      2,  37,   0  /*end, width, height offset*/
  };
  int MageChar_r[][3] = 
  {
      1,   5,  14, /*hump*/
      0,   8,  19,
      0,  12,  23,
      0,  15,  23,
      1,   5,  24, /*vertical*/
      0,   5,   0,
      2,  18,   0  /*end, width, height offset*/
  };
  int MageChar_S[][3] = 
  {
      1,  28,  27,
      0,  27,  31,
      0,  22,  33,
      0,  17,  34,
      0,  12,  33,
      0,   8,  31,
      0,   6,  25,
      0,   7,  22,
      0,  11,  19,
      0,  23,  16,
      0,  28,  13,
      0,  30,   8,
      0,  28,   3,
      0,  25,   1,
      0,  17,   0,
      0,   9,   1,
      0,   7,   3,
      0,   5,   8,
      2,  34,   0  /*end, width, height offset*/
  };
  int MageChar_s[][3] = 
  {
      1,  21,  20,
      0,  19,  23,
      0,  13,  24,
      0,   7,  23,
      0,   4,  19,
      0,   7,  14,
      0,  20,  10,
      0,  22,   8,
      0,  22,   5,
      0,  20,   1,
      0,  13,   0,
      0,   6,   1,
      0,   4,   4,
      2,  26,   0  /*end, width, height offset*/
  };
  int MageChar_T[][3] = 
  {
      1,   3,  34,
      0,  29,  34,
      1,  16,  34,
      0,  16,   0,
      2,  32,   0  /*end, width, height offset*/
  };
  int MageChar_t[][3] = 
  {
      1,   7,  31,
      0,   7,   2,
      0,   8,   0,
      0,  11,   0,
      1,   2,  24, /*cross bar*/
      0,  12,  24,
      2,  15,   0  /*end, width, height offset*/
  };
  int MageChar_U[][3] = 
  {
      1,   6,  34,
      0,   6,  12,
      0,   8,   5,
      0,  12,   1,
      0,  19,   0,
      0,  26,   1,
      0,  30,   5,
      0,  32,  12,
      0,  32,  34,
      2,  37,   0  /*end, width, height offset*/
  };
  int MageChar_u[][3] = 
  {
      1,   5,  24,      
      0,   5,   6,
      0,   7,   2,
      0,  12,   0,
      0,  17,   1,
      0,  20,   4,
      0,  23,  10,
      1,  23,  24, /*vertical*/
      0,  23,   0,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_V[][3] = 
  {
      1,   4,  34,
      0,  18,   0,
      0,  32,  34,
      2,  35,   0  /*end, width, height offset*/
  };
  int MageChar_v[][3] = 
  {
      1,   4,  24,      
      0,  13,   0,
      0,  22,  24,
      2,  26,   0  /*end, width, height offset*/
  };
  int MageChar_W[][3] = 
  {
      1,   4,  34,
      0,  13,   0,
      0,  25,  34,
      0,  37,   0,
      0,  46,  34,
      2,  49,   0  /*end, width, height offset*/
  };
  int MageChar_w[][3] = 
  {
      1,   3,  24,  
      0,  11,   0,
      0,  19,  24,
      0,  27,   0,
      0,  35,  24,
      2,  38,   0  /*end, width, height offset*/
  };
  int MageChar_X[][3] = 
  {
      1,   6,  34,
      0,  29,   0,
      1,  29,  34,
      0,   6,   0,
      2,  34,   0  /*end, width, height offset*/
  };
  int MageChar_x[][3] = 
  {
      1,   4,  24,
      0,  21,   0,
      1,  21,  24,
      0,   4,   0,
      2,  26,   0  /*end, width, height offset*/
  };
  int MageChar_Y[][3] = 
  {
      1,   5,  34,
      0,  18,  14,
      1,  31,  34,
      0,  18,  14,
      0,  18,   0,
      2,  35,   0  /*end, width, height offset*/
  };
  int MageChar_y[][3] = 
  {
      1,   3,  24, /*left arm*/
      0,  13,  -3,
      1,  23,  24, /*right arm*/
      0,  13,  -3,
      0,  10,  -9, /*sl bent left*/
      0,   7, -11,
      0,   5, -11, /*hook*/
      2,  26,   0  /*end, width, height offset*/
  };
  int MageChar_Z[][3] = 
  {
      1,   5,  34,
      0,  30,  34,
      0,   3,   0,
      0,  30,   0,
      2,  32,   0  /*end, width, height offset*/
  };
  int MageChar_z[][3] = 
  {
      1,   5,  24,
      0,  21,  24,
      0,   3,   0,
      0,  22,   0,
      2,  26,   0  /*end of character, width, height offset for next char*/
  };
  int MageChar_0[][3] = 
  {
      1,  14,   0, /*bottom*/
      0,  18,   1,
      0,  22,   3,
      0,  24,   8,
      0,  25,  16,
      0,  24,  24,
      0,  22,  29,
      0,  18,  31,
      0,  14,  32,
      0,  10,  31,
      0,   6,  29,
      0,   4,  24,
      0,   3,  16,
      0,   4,   8,
      0,   6,   3,
      0,  10,   1,
      0,  14,   0,
      2,  28,   0  /*end, width, height offset*/
  };
 int MageChar_1[][3] = 
  {
      1,   7,  26, /*top stroke*/
      0,  16,  32, /*vertical*/
      0,  16,   0,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_2[][3] = 
  {
      1,   5,  23,
      0,   6,  27,
      0,  10,  31,
      0,  15,  32,
      0,  20,  31,
      0,  24,  27,
      0,  25,  23,
      0,  24,  19,
      0,  21,  16,
      0,   8,   9,
      0,   5,   5,
      0,   3,   0,
      0,  25,   0,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_3[][3] = 
  {
      1,   5,  25,
      0,   6,  28,
      0,   9,  31,
      0,  14,  32,
      0,  19,  31,
      0,  22,  28,
      0,  23,  25,
      0,  22,  22,
      0,  20,  19,
      0,  16,  17,
      0,  13,  17,
      0,  16,  17,
      0,  21,  15,
      0,  24,  12,
      0,  25,   8,
      0,  24,   4,
      0,  20,   1,
      0,  14,   0,
      0,   8,   1,
      0,   5,   4,
      0,   4,   7,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_4[][3] = 
  {  
      1,  20,   0, 
      0,  20,  32, 
      0,   3,   9, 
      0,  25,   9,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_5[][3] = 
  {
      1,  23,  32,
      0,   8,  32,
      0,   5,  16,
      0,   9,  18,
      0,  15,  20,
      0,  21,  18,
      0,  24,  15,
      0,  25,  10,
      0,  24,   5,
      0,  20,   1,
      0,  14,   0,
      0,   8,   1,
      0,   5,   4,
      0,   4,   7,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_6[][3] = 
  {
      1,   4,  11, /*start inside*/
      0,  10,  19,
      0,  15,  20,
      0,  20,  19,
      0,  24,  15,
      0,  25,  10,
      0,  24,   5,
      0,  20,   1,
      0,  15,   0,
      0,  10,   1,
      0,   5,   6,
      0,   4,  11, /*covers start*/
      0,   4,  16,
      0,   5,  24,
      0,  10,  31,
      0,  15,  32,
      0,  20,  31,
      0,  23,  29,
      0,  24,  27,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_7[][3] = 
  {
      1,   4,  32,
      0,  25,  32,
      0,  10,   0,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_8[][3] = 
  {
      1,  14,   0,  /*bottom*/
      0,  20,   1,
      0,  23,   4,
      0,  24,   9,
      0,  23,  14,
      0,  20,  16,
      0,   9,  19, /*cross over*/
      0,   6,  21,
      0,   5,  25,
      0,   6,  29,
      0,   9,  31,
      0,  14,  32,
      0,  19,  31,
      0,  22,  29,
      0,  23,  25,
      0,  22,  21,
      0,  19,  19,
      0,   8,  16, /*cross over*/
      0,   5,  14,
      0,   4,   9,
      0,   5,   4,
      0,   8,   1,
      0,  14,   0,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageChar_9[][3] = 
  {
      1,  24,  24, /*start inside*/
      0,  23,  19,
      0,  19,  14,
      0,  14,  13,
      0,   9,  14,
      0,   6,  16,
      0,   4,  22,
      0,   6,  28,
      0,   9,  31,
      0,  14,  32,
      0,  19,  31,
      0,  21,  29,
      0,  24,  24, /*covers start*/
      0,  25,  17,
      0,  24,  10,
      0,  21,   3,
      0,  19,   1,
      0,  13,   0,
      0,   7,   1,
      0,   5,   5,
      2,  29,   0  /*end, width, height offset*/
  };
  int MageCharEx[][3] =   /* 33*/ /*exclamation point*/
  {
      1,   8,  34,
      0,   8,   9,
      1,   8,   2,
      0,   8,   0,
      2,  15,   0,
  };
  int MageCharDQ[][3] =   /* 34*/ /*double quote*/
  {
      1,   5,  34,
      0,   5,  23,
      1,  14,  34,
      0,  14,  23,
      2,  18,   0,
  };
  int MageCharNm[][3] =   /* 35*/ /*number symbol*/
  {
      1,  14,  34,
      0,   5,   0,
      1,  24,  34,
      0,  15,   0,
      1,   5,  23,
      0,  27,  23,
      1,   2,  24,
      2,  30,   0,
  };
  int MageCharDl[][3] =   /* 36*/ /*dollar sign*/
  {
      1,  24,  27,
      0,  23,  31,
      0,  18,  33,
      0,  14,  34,
      0,  10,  33,
      0,   6,  31,
      0,   4,  25,
      0,   5,  22,
      0,   9,  19,
      0,  19,  16,
      0,  24,  13,
      0,  26,   8,
      0,  24,   3,
      0,  21,   1,
      0,  14,   0,
      0,   7,   1,
      0,   5,   3,
      0,   3,   8,
      1,  14,  36,
      0,  14,  -5,
      2,  30,   0  /*end, width, height offset*/
  };
  int MageCharPr[][3] =   /* 37*/ /*percent*/
  {
      1,  10,  33, /*upper*/
      0,   6,  32,
      0,   3,  29,
      0,   2,  25,
      0,   3,  21,
      0,   6,  18,
      0,  10,  17,
      0,  14,  18,
      0,  17,  21,
      0,  18,  25,
      0,  17,  29,
      0,  14,  32,
      0,  10,  33,
      1,  35,  16,
      0,  31,  15,
      0,  28,  12,
      0,  27,   8,
      0,  28,   4,
      0,  31,   1,
      0,  35,   0,
      0,  39,   1,
      0,  42,   4,
      0,  43,   8,
      0,  42,  12,
      0,  39,  15,
      0,  35,  16,
      1,  32,  35,
      0,  13,  -1,
      2,  46,   0,
  };
  int MageCharAm[][3] =   /* 38*/ /*ampersand*/
  {
      1,  29,   0,
      0,  10,  23,
      0,   8,  27,
      0,   9,  31,
      0,  11,  33,
      0,  16,  34,
      0,  21,  32,
      0,  22,  28,
      0,  21,  24,
      0,  18,  22,
      0,   8,  15,
      0,   5,  12,
      0,   4,   8,
      0,   5,   4,
      0,   9,   1,
      0,  14,   0,
      0,  19,   1,
      0,  24,   5,
      0,  26,   9,
      0,  27,  14,
      2,  35,   0,
  };
  int MageCharSQ[][3] =   /* 39*/ /*single quote*/

  {
      1,   5,  34,
      0,   5,  23,
      2,  10,   0,
  };
  int MageCharLP[][3] =   /* 40*/ /*left  parenthesis*/
  {
      1,  13,  34,
      0,  10,  30,
      0,   7,  24,
      0,   6,  20,
      0,   5,  12,
      0,   6,   4,
      0,   7,   0,
      0,  10,  -6,
      0,  13, -10,
      2,  17,   0,
  };
  int MageCharRP[][3] =   /* 41*/ /*right parenthesis*/
  {
      1,   4,  34,
      0,   7,  30,
      0,  10,  24,
      0,  11,  20,
      0,  12,  12,
      0,  11,   4,
      0,  10,   0,
      0,   7,  -6,
      0,   4, -10,
      2,  17,   0,
  };
  int MageCharAs[][3] =   /* 42*/ /*asterick*/
  {
      1,  10,  34,
      0,  10,  28,
      1,   4,  30,
      0,  10,  28,
      0,  16,  30,
      1,   6,  23,
      0,  10,  28,
      0,  14,  23,
      2,  20,   0,
  };
  int MageCharPl[][3] =   /* 43*/ /*plus*/
  {
      1,  15,  23,
      0,  15,   1,
      1,   4,  12,
      0,  26,  12,
      2,  30,   0,
  };
  int MageCharCm[][3] =   /* 44*/ /*comma*/
  {
      1,   7,   2,
      0,   8,  -2,
      0,   6,  -8,
      2,  15,   0,
  };
  int MageCharMn[][3] =   /* 45*/ /*minus*/
  {
      1,   4,  12,
      0,  26,  12,
      2,  30,   0,
  };
  int MageCharPe[][3] =   /* 46*/ /*period*/
  {
      1,   7,   2,
      0,   7,  -1,
      2,  15,   0,
  };
  int MageCharFS[][3] =   /* 47*/ /*front slash*/  
  {
      1,  13,  34,
      0,   2,   0,
      2,  14,   0,
  };
  int MageCharCo[][3] =   /* 58*/ /*colon*/
  {
      1,   7,  23,
      0,   7,  20,
      1,   7,   2, /*period*/
      0,   7,  -1,
      2,  15,   0,
  };
  int MageCharSC[][3] =   /* 59*/ /*semicolon*/
  {
      1,   7,  23,
      0,   7,  20,
      1,   7,   2, /*comma*/
      0,   8,  -2,
      0,   6,  -8,
      2,  15,   0,
  };
  int MageCharLT[][3] =   /* 60*/ /*Less Than*/
  {
      1,  26,  24,
      0,   2,  12,
      0,  26,   0,
      2,  28,   0,
  };
  int MageCharEq[][3] =   /* 61*/ /*equals*/
  {
      1,   4,  18,
      0,  26,  18,
      1,   4,   6,
      0,  26,   6,
      2,  30,   0,
  };
  int MageCharGT[][3] =   /* 62*/ /*Greater Than*/
  {
      1,   2,  24,
      0,  26,  12,
      0,   2,   0,
      2,  28,   0,
  };
  int MageCharQs[][3] =   /* 63*/ /*question*/
  {
      1,   6,  26,
      0,   7,  30,
      0,  10,  33,
      0,  15,  34,
      0,  20,  33,
      0,  23,  30,
      0,  25,  26,
      0,  23,  22,
      0,  17,  16,
      0,  15,  13,
      0,  15,  10,
      1,  15,   2, /*.*/
      0,  15,  -1,
      2,  29,   0,
  };
  int MageCharAt[][3] = /* 64*/ /* @ at symbol, special MAGE use, nonprinting*/
  {
      1,  27,   9,
      0,  27,  17,
      0,  26,  21,
      0,  23,  24,
      0,  20,  25,
      0,  17,  24,
      0,  14,  21,
      0,  13,  17,
      0,  14,  13,
      0,  17,  10,
      0,  20,   9,
      0,  27,   9,
      0,  31,  11,
      0,  34,  17,
      0,  34,  24,
      0,  32,  28,
      0,  28,  32,
      0,  23,  34,
      0,  17,  34,
      0,  12,  32,
      0,   8,  28,
      0,   6,  24,
      0,   5,  17,
      0,   6,  10,
      0,   8,   6,
      0,  12,   2,
      0,  17,   0,
      0,  30,   0,
      2,  39,   0  /*end, width, height offset*/
  };
  int MageCharLS[][3] =   /* 91*/ /*L sq */
  {
      1,  11,  34,
      0,   5,  34,
      0,   5, -11,
      0,  11, -11,
      2,  15,   0,
  };  
  int MageCharSl[][3] =   /* 92*/ /*Back Slash, used as Excape, nonprinting*/
  {
      1,  13,   0,
      0,   2,  34,
      2,  14,   0,
  };  
  int MageCharRS[][3] =   /* 93*/ /*R sq */
  {
      1,   3,  34,
      0,   9,  34,
      0,   9, -11,
      0,   3, -11,
      2,  15,   0,
  };  
  int MageCharHt[][3] =   /* 94*/ /*hat*/
  {
      1,   4,  16,
      0,  12,  34,
      0,  20,  16,
      2,  25,   0,
  };  
  int MageCharUS[][3] =   /* 95*/ /*underscore*/
  {
      1,   2,  -7,
      0,  27,  -7,
      2,  28,   0,
  }; 
  int MageCharAc[][3] =   /* 96*/ /*accent*/
  {
      1,   5,  35,
      0,   9,  30,
      2,  18,   0,
  };
  int MageCharLC[][3] =   /*123*/ /*L curly*/
  {
      1,  18,  33,
      0,  13,  33,
      0,  11,  29,
      0,  11,  16,
      0,  10,  13,
      0,   5,  11,
      0,  10,   9,
      0,  11,  -7,
      0,  13, -11,
      0,  18, -11,
      2,  21,   0,
  };
  int MageCharPp[][3] =   /*124*/ /*pipe*/
  {
      1,   7,  34,
      0,   7,   0,
      2,  13,   0,
  };
  int MageCharRC[][3] =   /*125*/ /*R curly*/
  {
      1,   6,  33,
      0,  11,  33,
      0,  13,  29,
      0,  13,  16,
      0,  14,  13,
      0,  19,  11,
      0,  14,   9,
      0,  13,  -7,
      0,  11, -11,
      0,   6, -11,
      2,  21,   0,
  };
  int MageCharTi[][3] =   /*126*/ /*tilda*/
  {
      1,   2,   7,
      0,   4,  12,
      0,   8,  14,
      0,  12,  13,
      0,  19,   9,
      0,  23,   8,
      0,  27,  10,
      0,  29,  15,
      2,  30,   0,
  };
  int MageCharDL[][3] =   /*127*/ /*DEL, nonprinting*/
  {
      1,   0,   0,
      2,   0,   0,
  };

  int *MageStrokedChar[MageFontTableSIZE] =
  {
      &MageCharNp[0][0],  /*  0*/
      &MageCharNp[0][0],  /*  1*/
      &MageCharNp[0][0],  /*  2*/
      &MageCharNp[0][0],  /*  3*/
      &MageCharNp[0][0],  /*  4*/
      &MageCharNp[0][0],  /*  5*/
      &MageCharNp[0][0],  /*  6*/
      &MageCharNp[0][0],  /*  7*/
      &MageCharNp[0][0],  /*  8*/
      &MageCharNp[0][0],  /*  9*/
      &MageCharLF[0][0],  /* 10*/ /*Line Feed*/
      &MageCharNp[0][0],  /* 11*/
      &MageCharNp[0][0],  /* 12*/
      &MageCharCR[0][0],  /* 13*/ /*Carriage Return*/
      &MageCharNp[0][0],  /* 14*/
      &MageCharNp[0][0],  /* 15*/
      &MageCharNp[0][0],  /* 16*/
      &MageCharNp[0][0],  /* 17*/
      &MageCharNp[0][0],  /* 18*/
      &MageCharNp[0][0],  /* 19*/
      &MageCharNp[0][0],  /* 20*/
      &MageCharNp[0][0],  /* 21*/
      &MageCharNp[0][0],  /* 22*/
      &MageCharNp[0][0],  /* 23*/
      &MageCharNp[0][0],  /* 24*/
      &MageCharNp[0][0],  /* 25*/
      &MageCharNp[0][0],  /* 26*/
      &MageCharNp[0][0],  /* 27*/
      &MageCharNp[0][0],  /* 28*/
      &MageCharNp[0][0],  /* 29*/
      &MageCharNp[0][0],  /* 30*/
      &MageCharNp[0][0],  /* 31*/
      &MageCharSp[0][0],  /* 32*/ /*The space character*/
      &MageCharEx[0][0],  /* 33*/ /*exclamation point*/
      &MageCharDQ[0][0],  /* 34*/ /*double quote*/
      &MageCharNm[0][0],  /* 35*/ /*number symbol*/
      &MageCharDl[0][0],  /* 36*/ /*dollar sign*/
      &MageCharPr[0][0],  /* 37*/ /*percent*/
      &MageCharAm[0][0],  /* 38*/ /*ampersand*/
      &MageCharSQ[0][0],  /* 39*/ /*single quote*/
      &MageCharLP[0][0],  /* 40*/ /*left  parenthesis*/
      &MageCharRP[0][0],  /* 41*/ /*right parenthesis*/
      &MageCharAs[0][0],  /* 42*/ /*asterick*/
      &MageCharPl[0][0],  /* 43*/ /*plus*/
      &MageCharCm[0][0],  /* 44*/ /*comma*/
      &MageCharMn[0][0],  /* 45*/ /*minus*/
      &MageCharPe[0][0],  /* 46*/ /*period*/
      &MageCharFS[0][0],  /* 47*/ /*front slash*/
      &MageChar_0[0][0],  /* 48*/   /*zero*/
      &MageChar_1[0][0],  /* 49*/   /*one*/
      &MageChar_2[0][0],  /* 50*/
      &MageChar_3[0][0],  /* 51*/
      &MageChar_4[0][0],  /* 52*/
      &MageChar_5[0][0],  /* 53*/
      &MageChar_6[0][0],  /* 54*/
      &MageChar_7[0][0],  /* 55*/
      &MageChar_8[0][0],  /* 56*/
      &MageChar_9[0][0],  /* 57*/   /*9*/
      &MageCharCo[0][0],  /* 58*/ /*colon*/
      &MageCharSC[0][0],  /* 59*/ /*semicolon*/
      &MageCharLT[0][0],  /* 60*/ /*Less Than*/
      &MageCharEq[0][0],  /* 61*/ /*equals*/
      &MageCharGT[0][0],  /* 62*/ /*Greater Than*/
      &MageCharQs[0][0],  /* 63*/ /*question*/
      &MageCharAt[0][0],  /* 64*/ /*at symbol, special MAGE use, nonprinting*/
      &MageChar_A[0][0],  /* 65*/   /*A*/
      &MageChar_B[0][0],  /* 66*/
      &MageChar_C[0][0],  /* 67*/
      &MageChar_D[0][0],  /* 68*/
      &MageChar_E[0][0],  /* 69*/
      &MageChar_F[0][0],  /* 70*/
      &MageChar_G[0][0],  /* 71*/
      &MageChar_H[0][0],  /* 72*/
      &MageChar_I[0][0],  /* 73*/
      &MageChar_J[0][0],  /* 74*/
      &MageChar_K[0][0],  /* 75*/
      &MageChar_L[0][0],  /* 76*/
      &MageChar_M[0][0],  /* 77*/
      &MageChar_N[0][0],  /* 78*/
      &MageChar_O[0][0],  /* 79*/
      &MageChar_P[0][0],  /* 80*/
      &MageChar_Q[0][0],  /* 81*/
      &MageChar_R[0][0],  /* 82*/
      &MageChar_S[0][0],  /* 83*/
      &MageChar_T[0][0],  /* 84*/
      &MageChar_U[0][0],  /* 85*/
      &MageChar_V[0][0],  /* 86*/
      &MageChar_W[0][0],  /* 87*/
      &MageChar_X[0][0],  /* 88*/
      &MageChar_Y[0][0],  /* 89*/
      &MageChar_Z[0][0],  /* 90*/   /*Z*/
      &MageCharLS[0][0],  /* 91*/ /*L sq */
      &MageCharSl[0][0],  /* 92*/ /*Back Slash, used as Excape, nonprinting*/
      &MageCharRS[0][0],  /* 93*/ /*R sq */
      &MageCharHt[0][0],  /* 94*/ /*hat*/
      &MageCharUS[0][0],  /* 95*/ /*underscore*/
      &MageCharAc[0][0],  /* 96*/ /*accent*/
      &MageChar_a[0][0],  /* 97*/   /*a*/
      &MageChar_b[0][0],  /* 98*/
      &MageChar_c[0][0],  /* 99*/
      &MageChar_d[0][0],  /*100*/
      &MageChar_e[0][0],  /*101*/
      &MageChar_f[0][0],  /*102*/
      &MageChar_g[0][0],  /*103*/
      &MageChar_h[0][0],  /*104*/
      &MageChar_i[0][0],  /*105*/
      &MageChar_j[0][0],  /*106*/
      &MageChar_k[0][0],  /*107*/
      &MageChar_l[0][0],  /*108*/
      &MageChar_m[0][0],  /*109*/
      &MageChar_n[0][0],  /*110*/
      &MageChar_o[0][0],  /*111*/
      &MageChar_p[0][0],  /*112*/
      &MageChar_q[0][0],  /*113*/
      &MageChar_r[0][0],  /*114*/
      &MageChar_s[0][0],  /*115*/
      &MageChar_t[0][0],  /*116*/
      &MageChar_u[0][0],  /*117*/
      &MageChar_v[0][0],  /*118*/
      &MageChar_w[0][0],  /*119*/
      &MageChar_x[0][0],  /*120*/
      &MageChar_y[0][0],  /*121*/
      &MageChar_z[0][0],  /*122*/   /*z*/
      &MageCharLC[0][0],  /*123*/ /*L curly*/
      &MageCharPp[0][0],  /*124*/ /*pipe*/
      &MageCharRC[0][0],  /*125*/ /*R curly*/
      &MageCharTi[0][0],  /*126*/ /*tilda*/
      &MageCharDL[0][0],  /*127*/ /*DEL, nonprinting*/
  };

#endif  /*EXTERNREND is defined*/


