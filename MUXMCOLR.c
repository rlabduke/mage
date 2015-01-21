                      /*MUX_COLR.c*/
#include "MAGE.h"  
#include "MAGECOLR.h"  
    
/* declaration outside of any routine thus extern */
/* dcr's best match to std X11 available colors*/
/*pretty good for blk bkg, poor for wht bkg set */
/*lime has not been calc, just plugged in an extra green*/
/*also extra plugs for lilac,peach,peachtint,lilactint*/
unsigned int myX11palette[256][3] =
{
/*  0:   0,   0,   0, deadblack                       */   0,   0,   0,
/*  1:  93,   0,   0, red        0 B {red4}           */ 139,   0,   0,
/*  2:  89,  27,   0, orange     0 B {OrangeRed4}     */ 139,  37,   0,
/*  3:  95,  55,   0, gold       0 B {orange4}        */ 139,  90,   0,
/*  4:  82,  78,   0, yellow     0 B {yellow4}        */ 139, 139,   0,
/*  5:  44,  82,   0, lime       0 B {DarkGreen}      */   0, 100,   0,
/*  6:   7,  85,   0, green      0 B {DarkGreen}      */   0, 100,   0,
/*  7:   0,  82,  31, sea        0 B {SpringGreen4}   */   0, 139,  69,
/*  8:   0,  78,  69, cyan       0 B {cyan4}          */   0, 139, 139,
/*  9:  20,  48,  85, sky        0 B {DodgerBlue4}    */  16,  78, 139,
/* 10:  31,  31,  89, blue       0 B {NavyBlue}       */   0,   0, 128,
/* 11:  50,  23,  93, purple     0 B {purple4}        */  85,  26, 139,
/* 12:  82,   0,  74, magenta    0 B {magenta4}       */ 139,   0, 139,
/* 13:  89,   0,  39, hotpink    0 B {DeepPink4}      */ 139,  10,  80,
/* 14: 115,  50,  60, pink       0 B {coral4}         */ 139,  62,  47,
/* 15:  85,  45, 100, lilac      0 B {coral4}  extra  */ 139,  62,  47,
/* 16:  95,  45,  30, peach      0 B {coral4}  extra  */ 139,  62,  47,
/* 17: 105,  65,  45, peachtint  0 B {coral4}  extra  */ 139,  62,  47,
/* 18:  85,  85,  45, yellowtint 0 B {LightGoldenrod4}*/ 139, 129,  76,
/* 19:  54,  89,  66, greentint  0 B {PaleGreen4}     */  84, 139,  84,
/* 20:  60,  78,  89, bluetint   0 B {LightSkyBlue4}  */  96, 123, 139,
/* 21:  80,  70, 100, lilactint  0 B {LightSkyBlue4}ex*/  96, 123, 139,
/* 22: 115,  80,  90, pinktint   0 B {LightPink4}     */ 139,  95, 101,
/* 23:  74,  74,  74, white      0 B {gray29}         */  74,  74,  74,
/* 24:  35,  35,  35, gray       0 B {gray14}         */  36,  36,  36,
/* 25:  70,  46,  35, brown      0 B {LightSalmon4}   */ 139,  87,  66,
/* 26: 255, 160, 160, red        0 W {LightPink2}     */ 238, 162, 173,
/* 27: 250, 180, 120, orange     0 W {tan1}           */ 255, 165,  79,
/* 28: 230, 175, 160, gold       0 W {LightGoldenrod3}*/ 205, 190, 112,
/* 29: 240, 230, 200, yellow     0 W {PaleGoldenrod}  */ 238, 232, 170,
/* 30: 170, 210, 160, lime       0 W {honeydew3}      */ 193, 205, 193,
/* 31: 187, 218, 171, green      0 W {honeydew3}      */ 193, 205, 193,
/* 32: 170, 240, 190, sea        0 W {honeydew3}      */ 193, 205, 193,
/* 33: 160, 215, 215, cyan       0 W {azure3}         */ 193, 205, 205,
/* 34: 117, 171, 214, sky        0 W {LightSkyBlue3}  */ 141, 182, 205,
/* 35: 160, 160, 255, blue       0 W {LightSteelBlue} */ 176, 196, 222,
/* 36: 179, 140, 214, purple     0 W {thistle3}       */ 205, 181, 205,
/* 37: 230, 117, 210, magenta    0 W {plum}           */ 221, 160, 221,
/* 38: 255, 120, 150, hotpink    0 W {pink2}          */ 238, 169, 184,
/* 39: 250, 160, 170, pink       0 W {PaleVioletRed1} */ 255, 130, 171,
/* 40: 190, 140, 210, lilac      0 W {PaleVioletRed1}x*/ 255, 130, 171,
/* 41: 195, 110,  80, peach      0 W {PaleVioletRed1}x*/ 255, 130, 171,
/* 42: 195, 110,  80, peachtint  0 W {PaleVioletRed1}x*/ 255, 130, 171,
/* 43: 170, 160, 120, yellowtint 0 W {wheat3}         */ 205, 186, 150,
/* 44: 140, 210, 171, greentint  0 W {DarkSeaGreen3}  */ 155, 205, 155,
/* 45: 135, 171, 210, bluetint   0 W {LightSkyBlue2}  */ 164, 211, 238,
/* 46: 185, 151, 210, lilactint  0 W {LightSkyBlue2}ex*/ 164, 211, 238,
/* 47: 200, 120, 120, pinktint   0 W {LightPink3}     */ 205, 140, 149,
/* 48: 156, 156, 156, white      0 W {gray61}         */ 156, 156, 156,
/* 49: 175, 175, 175, gray       0 W {gray69}         */ 176, 176, 176,
/* 50: 230, 170, 145, brown      0 W {burlywood1}     */ 255, 211, 155,
/* 51: 136,   0,   0, red        1 B {red4}           */ 139,   0,   0,
/* 52: 132,  39,   0, orange     1 B {OrangeRed4}     */ 139,  37,   0,
/* 53: 135,  78,   0, gold       1 B {orange4}        */ 139,  90,   0,
/* 54: 121, 117,   0, yellow     1 B {yellow4}        */ 139, 139,   0,
/* 55:  66, 123,   0, lime       1 B {green4}         */   0, 139,   0,
/* 56:  11, 128,   0, green      1 B {green4}         */   0, 139,   0,
/* 57:   0, 125,  50, sea        1 B {SpringGreen4}   */   0, 139,  69,
/* 58:   0, 102,  92, cyan       1 B {cyan4}          */   0, 139, 139,
/* 59:  31,  68, 125, sky        1 B {DodgerBlue4}    */  16,  78, 139,
/* 60:  39,  39, 132, blue       1 B {blue4}          */   0,   0, 139,
/* 61:  75,  27, 132, purple     1 B {purple4}        */  85,  26, 139,
/* 62: 125,   0, 113, magenta    1 B {magenta4}       */ 139,   0, 139,
/* 63: 128,   0,  54, hotpink    1 B {DeepPink4}      */ 139,  10,  80,
/* 64: 150,  65,  78, pink       1 B {coral4}         */ 139,  62,  47,
/* 65: 110,  60, 140, lilac      1 B {coral4}  extra  */ 139,  62,  47,
/* 66: 135,  70,  40, peach      1 B {coral4}  extra  */ 139,  62,  47,
/* 67: 142,  95,  65, peachtint  1 B {coral4}  extra  */ 139,  62,  47,
/* 68: 125, 125,  67, yellowtint 1 B {LightGoldenrod4}*/ 139, 129,  76,
/* 69:  85, 132,  97, greentint  1 B {PaleGreen4}     */  84, 139,  84,
/* 70:  90, 113, 132, bluetint   1 B {LightSkyBlue4}  */  96, 123, 139,
/* 71: 115,  95, 140, lilactint  1 B {LightSkyBlue4}ex*/  96, 123, 139,
/* 72: 150, 105, 117, pinktint   1 B {LightPink4}     */ 139,  95, 101,
/* 73: 101, 101, 101, white      1 B {gray40}         */ 102, 102, 102,
/* 74:  46,  46,  46, gray       1 B {gray18}         */  46,  46,  46,
/* 75:  89,  58,  42, brown      1 B {LightSalmon4}   */ 139,  87,  66,
/* 76: 255, 120, 120, red        1 W {LightCoral}     */ 240, 128, 128,
/* 77: 250, 160,  90, orange     1 W {tan2}           */ 238, 154,  73,
/* 78: 230, 175, 120, gold       1 W {goldenrod2}     */ 238, 180,  34,
/* 79: 240, 230, 150, yellow     1 W {LightGoldenrod} */ 238, 221, 130,
/* 80: 170, 210, 120, lime       1 W {DarkSeaGreen3}  */ 155, 205, 155,
/* 81: 144, 210, 128, green      1 W {DarkSeaGreen3}  */ 155, 205, 155,
/* 82: 140, 230, 170, sea        1 W {honeydew3}      */ 193, 205, 193,
/* 83: 120, 208, 208, cyan       1 W {PaleTurquoise3} */ 150, 205, 205,
/* 84:  93, 160, 214, sky        1 W {SkyBlue3}       */ 108, 166, 205,
/* 85: 120, 120, 255, blue       1 W {MediumSlateBlue}*/ 123, 104, 238,
/* 86: 164, 109, 214, purple     1 W {MediumPurple2}  */ 159, 121, 238,
/* 87: 226,  93, 210, magenta    1 W {orchid}         */ 218, 112, 214,
/* 88: 255,  90, 140, hotpink    1 W {HotPink2}       */ 238, 106, 167,
/* 89: 245, 150, 160, pink       1 W {PaleVioletRed2} */ 238, 121, 159,
/* 90: 180, 130, 200, lilac      1 W {PaleVioletRed2}x*/ 238, 121, 159,
/* 91: 195, 110,  60, peach      1 W {PaleVioletRed2}x*/ 238, 121, 159,
/* 92: 195, 110,  60, peachtint  1 W {PaleVioletRed2}x*/ 238, 121, 159,
/* 93: 170, 160,  90, yellowtint 1 W {DarkKhaki}      */ 189, 183, 107,
/* 94: 105, 179, 128, greentint  1 W {MediumAquamarin}*/ 102, 205, 170,
/* 95: 100, 128, 179, bluetint   1 W {LightSkyBlue3}  */ 141, 182, 205,
/* 96: 160, 118, 180, lilactint  1 W {LightSkyBlue3}ex*/ 141, 182, 205,
/* 97: 190,  90,  90, pinktint   1 W {PaleVioletRed3} */ 205, 104, 137,
/* 98: 117, 117, 117, white      1 W {gray46}         */ 117, 117, 117,
/* 99: 156, 156, 156, gray       1 W {gray61}         */ 156, 156, 156,
/*100: 208, 152, 125, brown      1 W {burlywood2}     */ 238, 197, 145,
/*101: 187,   0,   0, red        2 B {red3}           */ 205,   0,   0,
/*102: 179,  54,   0, orange     2 B {OrangeRed3}     */ 205,  55,   0,
/*103: 175, 110,   0, gold       2 B {orange3}        */ 205, 133,   0,
/*104: 171, 164,   0, yellow     2 B {yellow3}        */ 205, 205,   0,
/*105:  93, 171,   0, lime       2 B {green3}         */   0, 205,   0,
/*106:  15, 179,   0, green      2 B {green3}         */   0, 205,   0,
/*107:   0, 171,  66, sea        2 B {SpringGreen3}   */   0, 205, 102,
/*108:   0, 136, 123, cyan       2 B {cyan3}          */   0, 205, 205,
/*109:  42,  92, 167, sky        2 B {DodgerBlue3}    */  24, 116, 205,
/*110:  46,  46, 183, blue       2 B {blue3}          */   0,   0, 205,
/*111: 100,  35, 175, purple     2 B {purple3}        */ 125,  38, 205,
/*112: 171,   0, 160, magenta    2 B {magenta3}       */ 205,   0, 205,
/*113: 175,   0,  70, hotpink    2 B {DeepPink3}      */ 205,  16, 118,
/*114: 185,  80,  95, pink       2 B {coral3}         */ 205,  91,  69,
/*115: 135,  75, 180, lilac      2 B {coral3} extra   */ 205,  91,  69,
/*116: 175,  95,  50, peach      2 B {coral3} extra   */ 205,  91,  69,
/*117: 180, 125,  85, peachtint  2 B {coral3} extra   */ 205,  91,  69,
/*118: 168, 168,  90, yellowtint 2 B {LightGoldenrod3}*/ 205, 190, 112,
/*119: 117, 187, 140, greentint  2 B {PaleGreen3}     */ 124, 205, 124,
/*120: 120, 140, 183, bluetint   2 B {LightSkyBlue3}  */ 141, 182, 205,
/*121: 158, 120, 180, lilactint  2 B {LightSkyBlue3}ex*/ 141, 182, 205,
/*122: 185, 130, 145, pinktint   2 B {LightPink3}     */ 205, 140, 149,
/*123: 164, 164, 164, white      2 B {gray63}         */ 161, 161, 161,
/*124:  66,  66,  66, gray       2 B {gray25}         */  64,  64,  64,
/*125: 117,  74,  58, brown      2 B {LightSalmon3}   */ 205, 129,  98,
/*126: 255,  80,  80, red        2 W {tomato2}        */ 238,  92,  66,
/*127: 250, 140,  60, orange     2 W {chocolate2}     */ 238, 118,  33,
/*128: 230, 175,  80, gold       2 W {goldenrod}      */ 218, 165,  32,
/*129: 240, 230, 100, yellow     2 W {goldenrod1}     */ 255, 193,  37,
/*130: 170, 210,  80, lime       2 W {PaleGreen3}     */ 124, 205, 124,
/*131: 101, 203,  85, green      2 W {PaleGreen3}     */ 124, 205, 124,
/*132: 110, 220, 150, sea        2 W {MediumAquamarin}*/ 102, 205, 170,
/*133:  80, 200, 200, cyan       2 W {MediumTurquoise}*/  72, 209, 204,
/*134:  70, 148, 214, sky        2 W {SteelBlue3}     */  79, 148, 205,
/*135:  80,  80, 255, blue       2 W {SlateBlue}      */ 106,  90, 205,
/*136: 148,  82, 214, purple     2 W {MediumPurple}   */ 147, 112, 219,
/*137: 222,  70, 207, magenta    2 W {MediumOrchid2}  */ 209,  95, 238,
/*138: 255,  60, 130, hotpink    2 W {VioletRed2}     */ 238,  58, 140,
/*139: 240, 140, 150, pink       2 W {PaleVioletRed}  */ 219, 112, 147,
/*140: 170, 120, 190, lilac      2 W {PaleVioletRed}ex*/ 219, 112, 147,
/*141: 195, 110,  40, peach      2 W {PaleVioletRed}ex*/ 219, 112, 147,
/*142: 195, 110,  40, peachtint  2 W {PaleVioletRed}ex*/ 219, 112, 147,
/*143: 170, 160,  60, yellowtint 2 W {khaki4}         */ 139, 134,  78,
/*144:  70, 148,  85, greentint  2 W {PaleGreen4}     */  84, 139,  84,
/*145:  75,  85, 148, bluetint   2 W {SkyBlue4}       */  74, 112, 139,
/*146: 135,  85, 150, lilactint  2 W {SkyBlue4} extra */  74, 112, 139,
/*147: 180,  60,  60, pinktint   2 W {PaleVioletRed4} */ 139,  71,  93,
/*148:  78,  78,  78, white      2 W {gray30}         */  77,  77,  77,
/*149: 136, 136, 136, gray       2 W {gray53}         */ 135, 135, 135,
/*150: 185, 133, 106, brown      2 W {burlywood}      */ 222, 184, 135,
/*151: 230,   0,   0, red        3 B {red2}           */ 238,   0,   0,
/*152: 226,  74,   0, orange     3 B {OrangeRed2}     */ 238,  64,   0,
/*153: 215, 137,   0, gold       3 B {orange2}        */ 238, 154,   0,
/*154: 218, 210,   0, yellow     3 B {yellow2}        */ 238, 238,   0,
/*155: 117, 216,   0, lime       3 B {green2}         */   0, 238,   0,
/*156:  19, 222,   0, green      3 B {green2}         */   0, 238,   0,
/*157:   0, 214,  85, sea        3 B {SpringGreen2}   */   0, 238, 118,
/*158:   0, 171, 154, cyan       3 B {cyan2}          */   0, 238, 238,
/*159:  54, 120, 222, sky        3 B {DodgerBlue2}    */  28, 134, 238,
/*160:  54,  54, 226, blue       3 B {blue2}          */   0,   0, 238,
/*161: 125,  42, 222, purple     3 B {purple2}        */ 145,  44, 238,
/*162: 218,   0, 203, magenta    3 B {magenta2}       */ 238,   0, 238,
/*163: 222,   0,  85, hotpink    3 B {DeepPink2}      */ 238,  18, 137,
/*164: 220,  95, 112, pink       3 B {coral2}         */ 238, 106,  80,
/*165: 160,  90, 220, lilac      3 B {coral2} extra   */ 238, 106,  80,
/*166: 215, 120,  60, peach      3 B {coral2} extra   */ 238, 106,  80,
/*167: 217, 155, 105, peachtint  3 B {coral2} extra   */ 238, 106,  80,
/*168: 210, 210, 112, yellowtint 3 B {LightGoldenrod2}*/ 238, 220, 130,
/*169: 140, 230, 164, greentint  3 B {PaleGreen2}     */ 144, 238, 144,
/*170: 142, 162, 226, bluetint   3 B {LightSkyBlue2}  */ 164, 211, 238,
/*171: 185, 145, 220, lilactint  3 B {LightSkyBlue2}ex*/ 164, 211, 238,
/*172: 220, 155, 172, pinktint   3 B {LightPink2}     */ 238, 162, 173,
/*173: 214, 214, 214, white      3 B {gray83}         */ 212, 212, 212,
/*174:  93,  93,  93, gray       3 B {gray36}         */  92,  92,  92,
/*175: 140,  93,  70, brown      3 B {LightSalmon2}   */ 238, 149, 114,
/*176: 255,  40,  40, red        3 W {firebrick3}     */ 205,  38,  38,
/*177: 250, 120,  30, orange     3 W {DarkOrange3}    */ 205, 102,   0,
/*178: 230, 175,  40, gold       3 W {goldenrod3}     */ 205, 155,  29,
/*179: 240, 230,  50, yellow     3 W {gold2}          */ 238, 201,   0,
/*180: 170, 210,  40, lime       3 W {LimeGreen}      */  50, 205,  50,
/*181:  58, 195,  42, green      3 W {LimeGreen}      */  50, 205,  50,
/*182:  80, 210, 130, sea        3 W {SeaGreen3}      */  67, 205, 128,
/*183:  40, 192, 192, cyan       3 W {turquoise}      */  64, 224, 208,
/*184:  46, 132, 214, sky        3 W {DodgerBlue3}    */  24, 116, 205,
/*185:  40,  40, 255, blue       3 W {RoyalBlue3}     */  58,  95, 205,
/*186: 132,  54, 214, purple     3 W {DarkOrchid3}    */ 154,  50, 205,
/*187: 218,  42, 207, magenta    3 W {magenta3}       */ 205,   0, 205,
/*188: 255,  30, 120, hotpink    3 W {VioletRed3}     */ 205,  50, 120,
/*189: 235, 130, 140, pink       3 W {PaleVioletRed3} */ 205, 104, 137,
/*190: 160, 110, 180, lilac      3 W {PaleVioletRed3}x*/ 205, 104, 137,
/*191: 195, 110,  20, peach      3 W {PaleVioletRed3}x*/ 205, 104, 137,
/*192: 195, 110,  20, peachtint  3 W {PaleVioletRed3}x*/ 205, 104, 137,
/*193: 170, 160,  30, yellowtint 3 W {LightGoldenrod4}*/ 139, 129,  76,
/*194:  39, 121,  42, greentint  3 W {ForestGreen}    */  34, 139,  34,
/*195:  45,  42, 122, bluetint   3 W {DarkSlateBlue}  */  72,  61, 139,
/*196: 118,  42, 125, lilactint  3 W {DarkSlateBlue}ex*/  72,  61, 139,
/*197: 170,  30,  30, pinktint   3 W {brown}          */ 165,  42,  42,
/*198:  39,  39,  39, white      3 W {gray15}         */  38,  38,  38,
/*199: 117, 117, 117, gray       3 W {gray46}         */ 117, 117, 117,
/*200: 163, 114,  88, brown      3 W {burlywood3}     */ 205, 170, 125,
/*201: 255,   0,   0, red        4 B {red}            */ 255,   0,   0,
/*202: 255,  93,   0, orange     4 B {OrangeRed}      */ 255,  69,   0,
/*203: 255, 165,   0, gold       4 B {orange1}        */ 255, 165,   0,
/*204: 255, 246,   0, yellow     4 B {yellow1}        */ 255, 255,   0,
/*205: 140, 251,   0, lime       4 B {green1}         */   0, 255,   0,
/*206:  23, 255,   0, green      4 B {green1}         */   0, 255,   0,
/*207:   0, 250, 109, sea        4 B {SpringGreen1}   */   0, 255, 127,
/*208:   0, 205, 185, cyan       4 B {cyan1}          */   0, 255, 255,
/*209:  64, 149, 255, sky        4 B {DodgerBlue1}    */  30, 144, 255,
/*210:  62,  62, 255, blue       4 B {blue1}          */   0,   0, 255,
/*211: 150,  54, 255, purple     4 B {purple1}        */ 155,  48, 255,
/*212: 255,   0, 234, magenta    4 B {magenta}        */ 255,   0, 255,
/*213: 255,   0, 101, hotpink    4 B {DeepPink}       */ 255,  20, 147,
/*214: 255, 110, 130, pink       4 B {coral1}         */ 255, 114,  86,
/*215: 185, 105, 255, lilac      4 B {coral1} extra   */ 255, 114,  86,
/*216: 255, 140,  60, peach      4 B {coral1} extra   */ 255, 114,  86,
/*217: 255, 185, 125, peachtint  4 B {coral1} extra   */ 255, 114,  86,
/*218: 250, 250, 135, yellowtint 4 B {LightGoldenrod1}*/ 255, 236, 139,
/*219: 152, 255, 179, greentint  4 B {PaleGreen1}     */ 154, 255, 154,
/*220: 160, 180, 255, bluetint   4 B {LightSkyBlue1}  */ 176, 226, 255,
/*221: 220, 170, 255, lilactint  4 B {LightSkyBlue1}ex*/ 176, 226, 255,
/*222: 255, 180, 200, pinktint   4 B {LightPink1}     */ 255, 174, 185,
/*223: 255, 255, 255, white      4 B {gray100}        */ 255, 255, 255,
/*224: 125, 125, 125, gray       4 B {gray49}         */ 125, 125, 125,
/*225: 175, 117,  89, brown      4 B {LightSalmon1}   */ 255, 160, 122,
/*226: 255,   0,   0, red        4 W {red3}           */ 205,   0,   0,
/*227: 250, 100,   0, orange     4 W {OrangeRed3}     */ 205,  55,   0,
/*228: 230, 175,   0, gold       4 W {DarkGoldenrod3} */ 205, 149,  12,
/*229: 240, 230,   0, yellow     4 W {yellow3}        */ 205, 205,   0,
/*230: 170, 210,   0, lime       4 W {green3}         */   0, 205,   0,
/*231:  15, 187,   0, green      4 W {green3}         */   0, 205,   0,
/*232:  50, 200, 110, sea        4 W {SpringGreen3}   */   0, 205, 102,
/*233:   0, 185, 185, cyan       4 W {cyan3}          */   0, 205, 205,
/*234:  19, 117, 214, sky        4 W {DodgerBlue3}    */  24, 116, 205,
/*235:   0,   0, 255, blue       4 W {MediumBlue}     */   0,   0, 205,
/*236: 117,  19, 214, purple     4 W {purple3}        */ 125,  38, 205,
/*237: 214,   0, 207, magenta    4 W {magenta3}       */ 205,   0, 205,
/*238: 255,   0, 110, hotpink    4 W {DeepPink3}      */ 205,  16, 118,
/*239: 230, 120, 130, pink       4 W {salmon3}        */ 205, 112,  84,
/*240: 150, 100, 170, lilac      4 W {salmon3} extra  */ 205, 112,  84,
/*241: 195, 110,   0, peach      4 W {salmon3} extra  */ 205, 112,  84,
/*242: 195, 110,   0, peachtint  4 W {salmon3} extra  */ 205, 112,  84,
/*243: 170, 160,   0, yellowtint 4 W {goldenrod4}     */ 139, 105,  20,
/*244:   7,  93,  15, greentint  4 W {DarkGreen}      */   0, 100,   0,
/*245:  15,  20, 110, bluetint   4 W {MidnightBlue}   */  25,  25, 112,
/*246: 100,  15, 110, lilactint  4 W {MidnightBlue}ext*/  25,  25, 112,
/*247: 160,   0,   0, pinktint   4 W {firebrick4}     */ 139,  26,  26,
/*248:   0,   0,   0, white      4 W {gray0}          */   0,   0,   0,
/*249:  97,  97,  97, gray       4 W {gray38}         */  97,  97,  97,
/*250: 142,  95,  70, brown      4 W {burlywood4}     */ 139, 115,  85,
/*251:   0,   0,   0, unused                          */   0,   0,   0,
/*252:   0,   0,   0, unused                          */   0,   0,   0,
/*253:   0,   0,   0, unused                          */   0,   0,   0,
/*254:   0,   0,   0, unused                          */   0,   0,   0,
/*255: 255, 255, 255  deadwhite                       */ 255, 255, 255
};
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****makegrayxcolor()**********************************************************/
int makegrayxcolor(int icolor)  /*stripped to use mygrayscalepalette 041001*/
{
   XColor  xcolor;

   xcolor.red   = 257*mygrayscalepalette[icolor][0]; /*loaded in MAGECOLR*/
   xcolor.green = 257*mygrayscalepalette[icolor][1];
   xcolor.blue  = 257*mygrayscalepalette[icolor][2];
   xcolor.flags = DoRed|DoGreen|DoBlue;
   if(XAllocColor(dpy,grayscalemap,&xcolor)) 
   {
      ingrayn[icolor] = xcolor.pixel;
      ingrayned[icolor] = 1;
   }
   else 
   {
      ingrayn[icolor] = 0; /*Pixel*/
      ingrayned[icolor] = 0; /*flag*/
   }
   return(ingrayned[icolor]);
}
/*___makegrayxcolor()_________________________________________________________*/

/****makexcolors()*************************************************************/
void makexcolors()
{
  XColor  xcolor;
  int     i,j,ifail,iwin,ipass,itry,icount;
  unsigned long plane_masks[32];
  int ncolors, nplanes;
  int nrequested = 256; /*used for block color assignment*/
  int oldLtest;
  Colormap defaultcolormap; 

  oldLtest = Ltest; /*this subroutine resets its own local Ltest flag */
  Ltest = 0; /*local, reset as needed for testing*/

if(Ltest)
{
   printf("makexcolors enters with Lread_writecolors== %d\n",Lread_writecolors);
   /*which often is possible, but Lread_writecolors = 0; before try  010911*/
}
  Lgrayscalemap = 0; /*on spec  040926*/

  defaultcolormap = DefaultColormap(dpy,DefaultScreen(dpy));
  if(Ltest) {printf("default colormap== %lu\n",defaultcolormap);} /*121108*/

  for(ipass = 1; ipass<=2; ipass++)
  {
    if(ipass == 1)
    {
        /*Lread_writecolors set by finddisplaytype,(140912) passed in common*/
        colormap = XCreateColormap(dpy,RootWindow(dpy,DefaultScreen(dpy)),
                             magevisual,AllocNone);
        if(Ltest) {printf("pass 1 create colormap== %lu\n",colormap);}/*121108*/
        /*041109 NULL interpreted as a pointer, Colormap as an integer*/
        if(colormap != 0 && colormap != defaultcolormap) /*041109 an integer!!*/
        {/*should check for BadAlloc,BadMatch,BadValue,BadWindow ????*/
          grayscalemap = XCreateColormap(dpy,RootWindow(dpy,DefaultScreen(dpy)),
                             magevisual,AllocNone);
          if(grayscalemap != colormap && grayscalemap != defaultcolormap)
               {Lgrayscalemap = 1;}
          else {Lgrayscalemap = 0;}
        }
    }
    else /*ipass == 2*/
    {
        Lread_writecolors = 0; /*don't try to make own map*/
        colormap = DefaultColormap(dpy,DefaultScreen(dpy));
        magevisual = DefaultVisual(dpy,DefaultScreen(dpy));
        if(Ltest) {printf("pass 2 use default colormap== %lu\n",colormap);}/*121108*/
    }
    iwin = 0;
    ifail = 0;
    nplanes = 0; /*pg 198-200, Vol 1 9004*/

    ncolors = nrequested;
    /* XAllocColorCells continuous single array*/

/*010911*/
/*SGI seems to allocate all 256 colors, but then complainss that*/
/*black, white, and yellow are NOT allocated*/
/*however, the full palette demo looks OK --?????*/
/*so try to use the read only colors, which is what linux does anyway*/

Lread_writecolors = 0;  /*010911*/
if(Ltest)
{
   printf("makexcolors arbitarily sets Lread_writecolors== %d\n",Lread_writecolors);
}
/*SO IT SEEMS AS IF I GAVE UP ON GETTING READ_WRITE COLORS !!!! ???? */

    if(Lread_writecolors)
    {/*read_write color possible*/
        while(1)
        {
            j = XAllocColorCells(
                dpy,
                colormap,
                False,
                plane_masks,
                nplanes,
                incolor,    /*MUXM.H EXTERN unsigned long  incolor[256];*/
                ncolors     /*index into incolor[] array*/
            );
            /*printf("makexcolors XAllocColorCells== %d,  ncolors== %d,"
                     " nplanes== %d\n" ,j,ncolors,nplanes);*/
            if(j) break; /*0 failure, try for fewer colors*/
            ncolors--;
            if(ncolors==0) break;
        }
        if(ncolors==nrequested)
        {
            printf("XAllocColorCells succeeded: ncolors == %d\n",ncolors);
            ifail = 0;
            iwin = 1;
        }
        else
        {
           printf("XAllocColorCells failed to alloc read_write colors:"
               " ncolors == only %d of the %d requested\n",ncolors,nrequested);
           Lread_writecolors = 0;
           ifail = 1;
           iwin = 0;
        }
        if(Lread_writecolors)
        {/*store palette values*/
            iwin = 0;
            xcolor.flags = DoRed|DoGreen|DoBlue;
            for(i=0; i<ncolors; i++)
            {
                xcolor.pixel=incolor[i]; /*incolor defined above*/
                xcolor.red   = 257*myfullpalette[i][0]; /*declared above*/
                xcolor.green = 257*myfullpalette[i][1];
                xcolor.blue  = 257*myfullpalette[i][2];
                XStoreColor(dpy,colormap,&xcolor); 
                iwin = iwin + 1;
                incolored[i] = 1;
            }
/*NOTE: should be incolor[WHITEINDEX] == WhitePixel(dpy,DefaultScreen(dpy));*/
            BLACKINDEX =   0; /*special entry full black*/
            WHITEINDEX = 255; /*223 full white in indexed set, level 4 #7*/
            /*255 may not be allocated when doing colors this way*/
            /*010514*/
            if(Ltest) printf("stored %d colors\n",iwin);
        }
    }/*read_write color possible*/
    else if(LLcolor) /*color still possible*/
    {/*try to allocate read only colors  THIS IS WHAT IS DONE !!!! */
      for(itry=1;itry<=2;itry++)
      {/*two tries: 1 with mage palette, 2 with X11 palette*/
        if(Ltest) printf("try %d to allocate read only colors\n",itry);
        icount = 0;
        ifail=0;
        iwin=0;
        xcolor.flags = DoRed|DoGreen|DoBlue;
        /*only need to allocate the exact cells actually used*/
        /*Aug 2001: use 0-250 and 255*/
        for(i=0; i<256; i++)
        {/*scan over all possible entries*/
          if((i>=0 && i<=250 ) || (i==255)) /*general colors + full white*/
          { 
            /*divide down by 256, multiply back up by 257*/
            /*myfullpalette is defined 0--255, divided down by 256 from 65535*/
            icount++;
            if(itry==1)
            {/*THIS SEEMS TO BE SUCCESSFUL, AT LEAST WHEN CHECKED FOR !!!! */
                xcolor.red   = 257*myfullpalette[i][0]; /*declared above*/
                xcolor.green = 257*myfullpalette[i][1];
                xcolor.blue  = 257*myfullpalette[i][2];
            }
            if(itry==2)
            {
                xcolor.red   = 257*myX11palette[i][0]; /*declared above*/
                xcolor.green = 257*myX11palette[i][1];
                xcolor.blue  = 257*myX11palette[i][2];
            }
            if(XAllocColor(dpy,colormap,&xcolor)) 
            {
                iwin=iwin+1;
                incolor[i] = xcolor.pixel;
                /*incolor for black can be == 0 !! */
                incolored[i] = 1;
             /*             
                printf("did match color# incolor[%d]== %ld"
                        ", got RGB: %d, %d, %d,"
                        " orig: %d, %d, %d\n"
                 ,i,incolor[i],xcolor.red,xcolor.green,xcolor.blue
                 ,myfullpalette[i][0],myfullpalette[i][1],myfullpalette[i][2]);
             */
            }
            else 
            {
                ifail = ifail + 1;
                /*incolor for black can be == 0 !! */
                incolor[i] = 0; /*Pixel*/
                incolored[i] = 0; /*flag*/
               /*
                printf("failed to match color# incolor[%d]== %ld"
                ", RGB: %d, %d, %d\n"
                ,i,incolor[i]
                ,myfullpalette[i][0],myfullpalette[i][1],myfullpalette[i][2]);
               */
            }
          }
        }/*scan over all possible entries*/
        if(ifail==0 && Lgrayscalemap)
        {/*invent a color-corrected-toned grayscale map  040926*/
           for(i=0; i<256; i++)
           {/*scan over all possible entries*/
             if((i>=0 && i<=250 ) || (i==255)) /*general colors + full white*/
             {
                makegrayxcolor(i);
             } 
           }/*scan over all possible entries*/
        }/*invent a color-corrected-toned grayscale map*/
        if(itry==1)
        {
           if(Ltest || (iwin != icount)) /*030315*/
           {
              printf("MAGE palette matched color %d times out of %d\n"
                     ,iwin,icount);
           }
        }
        else /*itry==2*/
        {
          printf("X11 palette matched color %d times out of %d\n",iwin,icount);
        }
/*NOTE: should be incolor[WHITEINDEX] == WhitePixel(dpy,DefaultScreen(dpy));*/
        BLACKINDEX =  0;
        WHITEINDEX = 255; /*most palettes keep this as full white*/
      
        if(ifail>0)
        {
            printf("failed to match color %d times\n",ifail);
            if(itry==1)
            {
                /*Free allocated color cells and try again*/
                /*XFreeColors(dpy,colormap,incolor,256,8);*/
            }
        }
        else break;
      }/*two tries: 1 with mage palette, 2 with X11 palette*/
    }/*try to allocate read only colors*/
    else if(Lcolor) /*only gray still possible*/
    {/*try to allocate read only colorcells*/
        ifail = 0;
        iwin = 0;
        icount = 0;
        if(Ltest) printf("try to allocate read only gray color cells\n");
        xcolor.flags = DoRed|DoGreen|DoBlue;
        for(i=0;i<16;i++)
        {   /*divide down by 256, multiply back up by 257*/
            icount++;
            /*xcolor.pixel=incolor[i];*/
            xcolor.red   = 257*mygraypalette[i][0]; /*declared MAGECOLR.h*/
            xcolor.green = 257*mygraypalette[i][1];
            xcolor.blue  = 257*mygraypalette[i][2];
            if(XAllocColor(dpy,colormap,&xcolor)) 
            {
                /* printf("did match gray# %d, got RGB: %d, %d, %d,"
                        " orig: %d, %d, %d\n"
                     ,i,xcolor.red,xcolor.green,xcolor.blue
                 ,myfullpalette[i][0],myfullpalette[i][1],myfullpalette[i][2]);
                */
                iwin = iwin + 1;
                incolor[i] = xcolor.pixel;
                incolored[i] = 1;
            }
            else 
            {
                /*printf("failed to match gray# %d, RGB: %d, %d, %d\n"
                ,i,myfullpalette[i][0],myfullpalette[i][1],myfullpalette[i][2]);
                */
                ifail = ifail + 1;
                incolored[i] = 0;
            }
        }

        BLACKINDEX = 10;
        WHITEINDEX =  5;

        if(ifail>0) 
        {
            printf("failed to match gray %d times\n",ifail);

            BLACKINDEX =  0;
            WHITEINDEX =  1;

        }
        printf("matched gray %d times out of %d\n",iwin,icount);
    }
    if(iwin>0 && !ifail) break;
  }/*ipass*/
  Ltest = oldLtest;
}
/*___makexcolors()____________________________________________________________*/

/****finddisplaytype()*********************************************************/
void  finddisplaytype(void) /*renamed 140912*/   /*<-MUXMINIT.c*/
{
  XVisualInfo    visualtemplate;
  XVisualInfo    *visuallist;
  Visual*        defaultvisual;
  int            j, visualsmatched, ivisual, idefaultvisual;
  int            Ltestcolor, numcolormaps;
  int idummycolor = -1; /*dummy flag, so can keep all possible items in code*/
      /* and still avoid picky compiler messages about not using variables*/
  int idirectcolor = -1; /*flags for these color models at magescreen depth*/
  int itruecolor= -1;
  int ipseudocolor= -1;
  int istaticcolor= -1;
  int igrayscale= -1;
  int istaticgray= -1;
  int firstpseudocolor= -1; /*really want to work with a pseudocolor visual*/

  idummycolor=idummycolor;/*avoid compiler messages about not using variable*/

   Ltestcolor = 0;
/* Ltestcolor = 1;*/
/*discover Visual attributes of this computer */
  defaultvisual = DefaultVisual(dpy,DefaultScreen(dpy));
  visualtemplate.screen = DefaultScreen(dpy);
  visualtemplate.depth = DefaultDepth(dpy,DefaultScreen(dpy));

  magedepth = visualtemplate.depth;
  /*windowbackground = WhitePixel(dpy,DefaultScreen(dpy));*/
  /*windowborder = BlackPixel(dpy,DefaultScreen(dpy));*/
/*NOTE: should be incolor[WHITEINDEX] == WhitePixel(dpy,DefaultScreen(dpy));*/
  /*do this after define palette*/

  /*find all visuals that belong to this screen */
  visuallist = XGetVisualInfo(dpy,VisualScreenMask,
                              &visualtemplate,&visualsmatched);
/*
printf("This screen is # %d, with %d color planes,  and %d total visuals\n"
       ,DefaultScreen(dpy),magedepth,visualsmatched);
*/
if(Ltest || Ltestcolor)
{
  printf("default visual: %p, WhitePixel== %ld, BlackPixel== %ld\n" /*121108*/
        ,defaultvisual,WhitePixel(dpy,DefaultScreen(dpy))
        ,BlackPixel(dpy,DefaultScreen(dpy))  );    
 XListInstalledColormaps(dpy,RootWindow(dpy,DefaultScreen(dpy)),&numcolormaps);
  printf("number of Installed Colormaps == %d\n",numcolormaps);
}

  idirectcolor = -1; /*flags for these color models at magescreen depth*/
  itruecolor = -1;
  ipseudocolor = -1;
  istaticcolor = -1;
  igrayscale = -1;
  istaticgray = -1;
  idefaultvisual = -1;
  firstpseudocolor = -1;

  for(j=0;j<visualsmatched;j++)
  {/*find all visuals of this screen*/
    if(visuallist[j].class==StaticGray)
    {
        sprintf(word,"StaticGray");
        if(visuallist[j].depth>=magedepth) istaticgray = j;
        if(visuallist[j].visual==defaultvisual) idefaultvisual = j;
    }
    if(visuallist[j].class==GrayScale)
    {
        sprintf(word,"GrayScale");
        if(visuallist[j].depth>=magedepth) igrayscale = j;
        if(visuallist[j].visual==defaultvisual) idefaultvisual = j;
    }
    if(visuallist[j].class==StaticColor) 
    {
        sprintf(word,"StaticColor");
        if(visuallist[j].depth>=magedepth) istaticcolor = j;
        if(visuallist[j].visual==defaultvisual) idefaultvisual = j;
    }
    if(visuallist[j].class==PseudoColor) 
    {
        sprintf(word,"PseudoColor");
        if(visuallist[j].depth>=magedepth) ipseudocolor = j;
        if(visuallist[j].visual==defaultvisual) idefaultvisual = j;
        if(firstpseudocolor<0 && visuallist[j].depth>=8) firstpseudocolor = j;
        /*identify first pseudocolor visual that would work for MAGE palatte*/

    }
    if(visuallist[j].class==TrueColor) 
    {
        sprintf(word,"TrueColor");
        if(visuallist[j].depth>=magedepth) itruecolor = j;
        if(visuallist[j].visual==defaultvisual) idefaultvisual = j;
    }
    if(visuallist[j].class==DirectColor) 
    {
        sprintf(word,"DirectColor");
        if(visuallist[j].depth>=magedepth) idirectcolor = j;
        if(visuallist[j].visual==defaultvisual) idefaultvisual = j;
    }
    if(Ltest || Ltestcolor)
     printf("class[%d] %d, %s, depth %d, #entries %d, bits/rgb %d, visual %p\n"
               ,j,visuallist[j].class,word,visuallist[j].depth
               ,visuallist[j].colormap_size
               ,visuallist[j].bits_per_rgb
               ,visuallist[j].visual); /*121108 %p for Visual* */
  }/*find all visuals of this screen*/
  /*set color control flags*/
  if(magedepth >= 8){ Lcolor = 1; LLcolor = 1; LCscrn = 1;}
  if(magedepth < 8) { Lcolor = 1; LLcolor = 0; LCscrn = 0;} /*only gray*/
  if(magedepth < 4) { Lcolor = 0; LLcolor = 0; LCscrn = 0;} /*only B&W*/
  Lread_writecolors = 0; /*can't have complete palette control unless 1 */
  /*work through by priority all visuals that are at default screen depth */
/*961013 SGI won't admit to a directcolor that has only a depth of 8*/
/*Since the default screen has only 8, this leaves MAGE trying to match*/
/*colors with the X-palette - with is pretty pathetic for depth cueing*/
/*So try hunting for a visual that is >= 8, MAGE's preferred depth! */
  if(idirectcolor >= 0)     {idummycolor = idirectcolor; } /*not used*/
  else if(itruecolor >= 0)  {idummycolor = itruecolor; } /*not used*/
  /*else if(ipseudocolor >= 0){ivisual = ipseudocolor; Lread_writecolors = 1;}*/
  if     (ipseudocolor >= 0)
  {
      if(firstpseudocolor>=0) ivisual = firstpseudocolor;
      else                    ivisual = ipseudocolor; 
      Lread_writecolors = 1;
  }

  else if(istaticcolor >= 0) {ivisual = istaticcolor; }
  else if(igrayscale >= 0)   {ivisual = igrayscale;   LLcolor = 0;}
  else if(istaticgray >= 0) {ivisual = istaticgray;  LLcolor = 0; Lcolor = 0;}
  else if(idefaultvisual < 0)
  {
    if(Ltest || Ltestcolor)
    {
      printf("never found anything in full screen depth\n");
      printf("using ivisual=1, expect wierd results or a crash\n");
      printf("but setting color controls to off\n");
    }
      ivisual = 0; /*presume at least 0th visual exists*/
      Lcolor  = 0;
      LLcolor = 0;
  }
  else /*idefaultvisual >= 0*/
  {
    if(Ltest || Ltestcolor)
    {
      printf("never found anything in full screen depth\n");
      printf("using default visual\n");
    }
      ivisual = idefaultvisual;
  }


  magevisual = visuallist[ivisual].visual;
  magedepth = visuallist[ivisual].depth;
  if(Ltest || Ltestcolor)
    printf("using visual #%d, depth== %d\n",ivisual,visuallist[ivisual].depth);

  makexcolors();
/*NOTE: should be incolor[WHITEINDEX] == WhitePixel(dpy,DefaultScreen(dpy));*/
}
/*___finddisplaytype()________________________________________________________*/

/****myGetEntryColor()*********************common name: MAC & PC***********/
void    myGetEntryColor(int icolor) /*returns RGB as rgbColor.red .green .blue*/
{
#ifdef UNDEFINED
    GetEntryColor(MagePalette, icolor,  &rgbColor);
#endif
    rgbColor.red   = 257*myfullpalette[icolor][0];
    rgbColor.green = 257*myfullpalette[icolor][1];
    rgbColor.blue  = 257*myfullpalette[icolor][2];

}
/*___myGetEntryColor()___________________________________________________*/

