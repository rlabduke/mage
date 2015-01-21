
#ifdef EXTERNANGL
#undef  EXTERNANGL
#define EXTERNANGL
#else
#define EXTERNANGL extern
#endif
 
/*MAGEANGL.C routines*/
EXTERNANGL void    domeasures(void);
EXTERNANGL void    dosurveillancemeasured(void); /*091212*/
EXTERNANGL char    surveillancestr[256]; /*091212, don't know how to rtn a str*/
EXTERNANGL char    constructionstr[256]; /*140518 AngleBetweenLines*/
EXTERNANGL char    monitorparamstr[256]; /*140915 NucleicAcidParameters*/
EXTERNANGL double    dotproduct( double,double,double,double,double,double);
EXTERNANGL double    angle3pt(   double,double,double,
                                 double,double,double,
                                 double,double,double );
EXTERNANGL double    dihedral4pt(double,double,double,
                                 double,double,double,
                                 double,double,double,
                                 double,double,double );
EXTERNANGL void    crossproduct( double,double,double,
                      double,double,double );
EXTERNANGL void    dodrawline(void);                        
EXTERNANGL void    doeraseline(void);
EXTERNANGL void    doconstructline(void);
EXTERNANGL void    dodock3on3(void); /*020305*/
EXTERNANGL void    doaxisrot(float*,float*,float*
                                         ,float,double,double,double
                                               ,double,double,double);
EXTERNANGL void    threepointdock(float[7][3]);

EXTERNANGL void    reinitialbondrots(void);
EXTERNANGL void    restorebondrots(int, int);
EXTERNANGL void    initrot(void);    /*old MAGEROTN.C */
EXTERNANGL void    resetrot(int);    /*old MAGEROTN.C */
EXTERNANGL void    resetzclip(int);   /*030405 */
EXTERNANGL void    resetzoom(int);    /*030405 */
EXTERNANGL void    setrotview(int);  /*old MAGEROTN.C */
EXTERNANGL void    dodocktran(int,float); /*970412*/
EXTERNANGL void    dodockrot(int,float); /*970412*/
EXTERNANGL void    dogangrotscan(int,float); /*970623*/
EXTERNANGL void    grafbondrotarrow(float direction); /*970917*/
EXTERNANGL int     bondrotoptioner(int i); /*moved here 970917*/  /*981010*/
EXTERNANGL void    bondrotbyoption(int i, float theta); /*001005*/
EXTERNANGL void    dobondrotrouter(int i, float theta); /*970917*/
EXTERNANGL void    dobondrotplotter(void);  /*970918*/
EXTERNANGL void    dobondrotgrapher(void);  /*001002, reworked 031226*/
EXTERNANGL void    dobondrotgrapherNDIMsuitefit(void);  /*121208,130116*/
EXTERNANGL void    bondrotgrapher(int,int[7],float[7]); /*031226*/
EXTERNANGL void    resetgraphedrotamer(int i);  /*001007*/
/*MAGEANGL.C special variables that need to be known by other routines*/
EXTERNANGL int icurrentrotation; /*970917*/
EXTERNANGL int LbondrotHplot,LbondrotVplot,LbondrotZplot; /*970918,030819*/
EXTERNANGL int Lconstructperpendicular;  /*971121*/
EXTERNANGL int Ldrawnewghost; /*011007*/
EXTERNANGL int LPerpendicularToPlane; /*971122*/
EXTERNANGL int LPerpendicularBetweenLines; /*971122*/
EXTERNANGL int LShortestBetweenLinesegments; /*971122*/
EXTERNANGL int LAngleBetweenLines; /*140518*/
EXTERNANGL int LSplitLine; /*971124SplitLine*/
EXTERNANGL int Ldottedline; /*990710 dotted draw line*/
EXTERNANGL int LconstructMultiple; /*971126ARROW*/
EXTERNANGL int L0to360; /*bondrotangles shown 0 - 360 , special for UNIX */
EXTERNANGL int Lsuitefittedstr; /* screen print of suite angles 121211*/
EXTERNANGL char suitefittedstr[256]; /* screen print of suite angles 121211*/

EXTERNANGL int LNucleicAcidParameters; /*140912*/
EXTERNANGL int LParameterlinekinemage; /*140915*/

EXTERNANGL int  maxdeep;   /*080912 pixelated triangle dot z-buffer*/
EXTERNANGL int  izadjust;  /*080912 pixelated triangle dot z-buffer*/
EXTERNANGL long izbinsize; /*080912 pixelated triangle dot z-buffer*/
EXTERNANGL int  izmin,izmax; /*080915 pixelated triangle dot z-clip*/
EXTERNANGL int  izminlimit,izmaxlimit,limitz; /*080915 pixelated  z-clip*/

/*971025perpendicular*/
#define NMATRIX 9
EXTERNANGL double A[NMATRIX+1][NMATRIX+1];
EXTERNANGL double C[NMATRIX+1][NMATRIX+1];

EXTERNANGL void    DoShortestBetweenLinesegments(float[3][7]);  /*971122*/
EXTERNANGL void    DoPerpendicularBetweenLines(float[3][7]);  /*971122*/
EXTERNANGL void    DoPerpendicularToPlane(float[3][7]);  /*971122*/
EXTERNANGL void    DoPerpendicularToLine(float[3][7]);  /*030930*/
EXTERNANGL void    DoAngleBetweenLines(float[3][7]);  /*140518*/
//EXTERNANGL void    DoNucleicAcidParameters(float[3][7]);  /*140912*/
EXTERNANGL void    DoNucleicAcidParameters();  /*dockpointptr[7],141006*/

EXTERNANGL void    PerpPtOnLine(float[3],float[3],float[3],float[3]); /*080916*/
EXTERNANGL void  TriangleParams(float[4][3],float[4][3],float[4][3]); /*080916*/
EXTERNANGL int     InTriangle2(float[3], float[4][3], float[4][3]);  /*080916*/
EXTERNANGL int     InTriangle(float[3][7]);  /*080912*/
EXTERNANGL void    EquationOfPlane(float[3][7]);  /*080912*/
EXTERNANGL int     matrixinvert(int);

/*971025perpendicular*/

