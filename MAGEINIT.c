/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/***MAGEINIT.C***************************************************************/
/*MAGE
 * shows KINEMAGEs
 *
 *  MAGE: Attribution Notice:
 * MAGE program: copyright c David C. Richardson, 
 *  1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002,2003,2004,2005
 *                                     all rights reserved
 *            Little River Institute, 5820 Old Stony Way, Durham, NC 27705
 *
 * First use was for the Interactive Graphics Appendix to Protein Science,
 *                                           Journal of the Protein Society.
 * Use and distribution for non-profit educational and research purposes
 * encouraged.  Rewrites for this and other platforms encouraged for similar
 * use and distribution.
 *            David C. Richardson, dcr@kinemage.biochem.duke.edu
 *    Biochemistry Dept., Duke University, Durham, North Carolina 27710, USA
 * Concept and MAGE   Developed by David C. Richardson and Jane S. Richardson
 *        "Kinemage" name invented by Caroline P. Usher
 * Grateful acknowledgement of many years of support from NIH, grant GM15000
 * which included visualization of protein structures and the development of
 * CHAOS: display program written in E&S PS300 function-net language,
 * CHAosIN Fortran program,  etc.
 *
 *   Although this code obviously bears resemblance to many examples of
 *    .c code, one hopes this combination and construction departs from the
 *    examples far enough so that there is no danger of copywrite infringement,
 *    nor blame for failings, going back to earlier work.
 *   On the other hand, there is a real intellectual debt owed.
 *   In particular:
 *     Think C development system, examples, and manual, Symantec Corp. 1989 +
 *            and several timely hints from Think C technical Support!
 *     Apple Inside Macintosh, Addison-Wesley, 1985
 *          (the essentual and eventually preferred ref)
 *     Several suggestions and a look at real code by Warren Robinett
 *     Using the Macintosh Toolbox with C,
 *            F.A.Huxham, D.Burnard,J.Takatsuka 2nd Ed. Sybex,1989
 *     Macintosh C Programming Primer, Vol. II, D. Mark, Addison-Wesley, 1990
 *     Macintosh C Programming Primer, Vol. I 2nd Ed,  Mark & Reed, 
 *            Addison-Wesley,1992.  Particularly for AppleEvents
 *     Roger Sayle, RasMol author, for making rasmol produce kinemage files,
 *         and whose code showed me really how to use Apple Events!
 *     Robert Weiss who began making useful suggestions even before he signed
 *         on to help rewrite MAGE for the PC. (== coauthor for first PC_MAGE)
 *     Borland C++, Borland International, Inc. 1991
 *      (but MAGE tries to be vanilla C as much as possible)
 *     Microsoft Windows Programmer's Reference, for version 3,  1990
 *     UNIX and MOTIF: 
 *     Power Programming Motif, E.F.Johnson & K. Reichard, 2nd Ed. 1993
 *       MIS Press, New York.
 *     OSF/Motif Programmer's Reference, OSF, 1990, Prentice Hall
 *     O'Reilly pub X window system manuals, esp Vol 1,2 (Xlib), 4,5 (Xtlib)
 *     Doug Hoffman for UNIX Makefile magic and Bogus Linnux.
 *     Brian Thomas & Michael Prisant for help getting LINUX going.
 *     Mike Word for real Encapsulated PostScript and many tips and ideas
 *      that helped me rebuild MAGE to be more object oriented.
 *      Also did much of the UNIX MAGE-Probe interactive, interprogram real-time
 *      updates, which is used for MAGE-PREKIN as well.
 *     Brent Presley for porting PCMAGE to 32bit Visual C++ code and helping
 *       reconcile that version of MAGE on all platforms.     971001
 *     Mac MAGE-PREKIN AppleEvent interprogram stuff drew heavily from:
 *     "ReverSever" (PREKIN) "ReverClient" (MAGE), Ultimate Mac Programming, 
 *       Volume I, Copyright 1994, Dave Mark and Donald Olson: "code is free..."
 *     Ian W. Davis for developing MolProbity and the KiNG java project:
 *       (Kinemage Next Generation): insights about mage and kinemages. 2001++
 * Files: various MAGE____.c routines, MAC____.c, MPC____.c, MUX____.c, etc.
 *  &  platform specific libraries like: ANSI & MacTraps from THINK-C library.
 * Mac Files are grouped, esp Main,Draw that call each other during
 *  critical speed use, i.e. for the real time rotation.
 * Files are Refered to by comments. 
 * AMAC... is for mac routines that can't be copied over to PC robustly.
 * MAC...  is for mac routines that can be copied.
 * MAGE... is for routines used by both Mac and PC and UNIX.
 * MPC...  is for PC routines that can be copied.
 * PCW...  is for PC Windows routines that probably can't be copied robustly.
 * UNIX versions are even more confusing, see top of MAGE.H
 *
 *  Coding follows Allman (BSD) indent style (Eric Allman, Berkeley)
 *  With some attempt to lead and trail subroutines so they can be seen easily
 *  and try to stay <=80 col so any editor, printer, or email is robust.
 *
 *Subroutine calls flagged by comment indicating source file of subroutine,
 * or if a C routine, etc. Beware: this is not always accurate! 
 * Some common C functions, like sprintf, are not always flagged.
 *To translate from English: alertstr[256],alertstr2[256],alertstr3[256] used
 * for comments to the reader.  Sometimes word[256] printed to alertstr2[256].
 * AMACMAGE.proj.rsrc and PCWMAGE.RC need translation.
 * check other sprintf calls here in MAGEINIT.C  
 ****************************************************************************/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MAGEINIT.C*****************************************************************/
/*initial routines */

#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"
#include "MAGETABL.h"
#include "MAGESYNC.h"
#include <string.h>
#include "MAGEANGL.h"
#include "MAGEDLOG.h"
#include "MAGEFOO.h"
#include "MAGEOUTR.h" /*070901*/
  
static char* magechangesstdout[] =
{"\r"
,"Ancient additions: details,ribbons,triangles,spheres\r"
,"zoomless fixed points, subscripts, line color&width, PS\r"
,"groups,subgroups,lists,points as structures. Docking\r"
,"Mac & SGI checked T/B stereo for Crystal_Eyes\r"
,"UNIX remote updating of kinemage\r"
,"32bit PCMAGE 971001\r"
,"020303 delete last rotation group,dihedrals display,appendout\r"
,"020313z dock3on3,gangedrot\r"
,"NB 020314egam attempt to show only active slider never implemented\r" 
,"020718 scaling revised for simulation viewing\r"
,"020814 unique and robust file names for kin output \r"
,"020816 unique file names extended with user name dialog\r"
,"020830,020902 forced uniquely named files for test results\r"
,"020904,07 timed test lines or labels or balls; scored \r"
,"020908 pickerase for new group during testmode\r"
,"020914,16,17,19 append multiple test output kins, practice mode\r"
,"020929 help to html file\r"
,"021211 commandline append of extra files\r"
,"021216 Compare right eye now works in postscript output\r"
,"021222 possible palette from KiNG HSV base\r"
,"021224 sequential octahedral markers\r"
,"021229 clean up Nextrafile extra file input timing\r"
,"030111 + accumulates animate groups\r"
,"030120 PC rgb 2D output patched *257 and FIND fixup\r"
,"030121 r3d and pov output fixed, PC-XP dragNdrop fixed\r"
,"030127 (!LbondrotHplot&&!LbondrotVplot) protection menu/drawnew\r"
,"030129 Michael Prisant almost cured the static compile\r"
,"030130 skipped for this line of work\r"
,"030131 Michael Prisant cured the static compile with dlstubs\r"
,"030201 dcr messed with Makefile: use static RH8, dynamic 7.2,7.3 \r"
,"030205---030210 schemes for multi-button mice, cleanup docking\r"
,"030216-030218 game,explore internally generated egamickdocking\r"
,"030220 6.23 Show Object Properties, 030223 code clean up\r"
,"030223 egam: capture the foo\r"
,"030225 clean up of 030220, 030305,6 dlog box instability...\r"
,"030308 start to set up for kin3Dcont ...\r"
,"030310 cleanup find pick, kip--> .1.kin \r"
,"030311-15 appendscreentext score from probe run->replacecaptiontext\r"
,"030312-15 NOT output docking scope, @mapfile,  \r"
,"030320 SetsinGrafWindow as flagged mainloop call, bad in old Mac\r"
,"030505 LGUI vs -postscript only NO GUI by commandline\r"
,"030528 multiple pointmasters\r"
,"030605,7 cleaned up help aspects & fanlist, LGUI=1 for all\r" 
,"030626 Postscript cleanup for balls,rings,marks as lists & points\r"
,"030630 foo slimemode, stays close does not stray out into void\r"
,"030709 Lmouse ztran w Lpickcenteron, foo finds initial place\r"
,"030713 Rmouse recentering avoids resetting buttons and sliders\r"
,"030725,801 6.31 commandline -postscript output of animation views\r"
,"030819+ zplot as well as hplot, vplot for 3 nucleic acid angles\r"
,"030823 clean up postscript output: small ball sizes and font sizes\r"
,"030910 postscript ribbon repair, X11 maxtext 64k=32k+32kappend \r"
,"030927 repair bondrot slider housekeeping for docking scope\r"
,"030928 NOE updating from Brian Coggins' program\r"
,"030930 measures+perpendicular distances to line and plane\r"
,"031001 NOE cleanup, destroy topbrot gracefully\r"
,"031002 Qstring to 512 for rotamer hypertext\r"
,"031011,12 rot seg faults and -changes -help cmd line\r"
,"031118 6.33 UNIX: -fontinfo, font sizes: -macular= -jeremy -large -medium\r"
,"031127-031201 dlog repair, postscript centerline clipping, new balls drag\r"
,"031226 remote update search for pointID, Up-Arrow remote, Single_Marker\r"
,"031227 apsects output & many colorsets allowed\r"
,"031228 3rd instance of mage for bondrotation model...\r"
,"040106 commandline socket setups\r"
,"040111 MAGEINPT scaling min/max set greater than historic 9999\r"
,"040208 6.34 output forced pseudo pdb format option\r"
,"040210 probe gets residue num without chainID, see prekin 030925\r"
,"040214,17,18,25,0302 sans serif fonts, resized fonts, limit Button text\r"
,"040306 6.35 hyperhypertext: caption *{name}* invokes  text *{name ...}*\r"
,"040320 avoid measures and markers for auger and probe. x:y:z OK.\r"
,"040402 foo work, inc use listradius when no point radius \r"
,"040409 Lzclipon state honored by postscripter and raster3Drender\r"
,"040502 POVray max intense colors\r"
,"040430--040504 dragline accuracy: matrix now (double), problem is integers?\r"
,"040509-23 resize_CB delay uses XtTimer (MacOSX and Linux/Unix)\r"
,"040605-09,12 6.36 loading time cleaned up, PC highlites fixed, etc.\r"
,"040616 gray scale option\r"
,"040831-0902 clean up ring & mark list radius. Put space in dihedral dhr=_-#\r"
,"040917 white background color vs light direction for spheres and ribbons\r"
,"040918a blk rim to sphere disks and more uniform colors \r"
,"040918b more sphere disks. sphere,ribbon blk bkg colors, blk rim\r"
,"040925 default blackrim on balls,spheres; recognize norim \r"
,"040926 separate grayscale colormap for UNIX/LINUX \r"
," (two-tone gray not implemented)\r"
,"041001 point master ON/OFF problem..., r3d pure black edges\r"
,"041011 menu help strings kept <256, magehelp.html now works OK\r"
,"041026 button font size affects a localname, e.g. master names stay intact\r"
,"041027,31 Viewing Param: screen size, @#size gwidthview,gheightview\r"
,"041108 kludge: separate scrolling window for buttons\r"
,"041109 probe command: only send rotatable sets that are turned ON\r"
,"041112 probe command: includes -nomodeltest for update into nmr models\r"
,"041210 FOO distsq\r"
,"050121 remote update uses model number, -nomodeltest NOT used!\r"
,"050123 writerotated recognizes docking scope mobile group\r"
,"050220 category-master==point-master forced ON when probe updates contacts\r"
,"050407 fix: disable restore dock angles, zero-length draw, Rmouse zclip. \r"
,"       rotated parts B==33.33\r"
,"050706 annotation: \r"
," MAGEDRAW line 529 /*ball flag on point is dominant*/ /*980804*/\r"
," so ball points not subject to pointmaster control, seems wrong headed.\r"
,"\r"
,"050816 FOO seems broken...\r"
,"050916.6.41 ------ \r"
,"050916-22 clean up html help, fix pickedlistoldptr re MSWindows crash\r"
,"050927-8 clean up PC text window notices\r"
,"050929 clean up PC dialog boxes\r"
,"051004-5 FIND findwholeword option, esp. for protein residue numbers\r"
,"051114 more protection from masters > 32, re multicrit.kin\r"
,"051120.6.42 ------ \r"
,"051120 writerotated work...\r"
,"051121 fix bug in view number for appended kinemage views\r"
,"051128 fix masterconfirmer: avoid crash with too many pointmasters\r"
,"051201 deadblack & deadwhite stay that way\r"
,"051214 clip middle of line if both ends would be clipped, kludges option\r"
,"060121.6.43 ------ 7D ------ LINUX and MAC only \r"
,"060121 group dimension=7 reads RNA 7 angle-coord kinemage, reassorts x,y,z \r"
,"060122 info line to mage screen, incl axes' ID\r"
,"060123 display radius of SELECTING zone\r"
,"060129 group parameters: axes names for dimensions\r"
,"060203 SELECT only points that are ON\r"
,"060206 SELECT menu, buttons work\r"
,"060207 SELECT just one, fix write-visible-only, rework output code\r"
,"060208 fix quit flag in MPCMAIN.cpp, ignore B flag on points in balllists\r"
,"060212 space,indent master button only at 1st encounter of each master\r"
,"060523 X flags starting point of independent triangle (to match KiNG)\r"
,"060525 dimension=7 now passed to saved kinemage; visible-only as if mono\r"

,"060604---6.44 ------- distributions of 7D clusters ------ \r"
,"060604 averages and sigma lines for distributions: file/save special...\r"
,"060606 ------------ 7D for MSWindows ------------\r"
,"060611 clean up averages and sigmas output\r"
,"060612-13 7D bug fixes,\r"
,"060613 @#axischoice view item, 36char button names, screen-size view item\r"
,"060614 protect axischoice 1--7 ...\r"
,"060615 improve postscript black edge on balls\r"
,"060619-22... ---6.45 (19 did listboxes)----- nD --- n = 13 ???? ----------\r"
,"060715,26... cont multidim...\r"
,"061012 emphasis taken off menu (can't pick rings), remains internal option\r"
,"061022 prohibit docking scope use of pointless empty trailing sets\r"
,"061023 list wrap inherits from group, e.g. 360 for negative angles.\r"
,"060128,29 MAXNDIM dimension work...\r"
,"061030 SELECTINGcolor by list, UNIX only\r"
,"061111,12,13---6.46--- rework dimensions...(fix output aname size)\r"
,"061114 ---6.47--- NDIM multipoint parallel axes \r"
,"061115 forward slash toggle, pickable multipoint parallel axes points\r"
,"061116 placemarkers on actual 3D coords after ND coord change.\r"
,"061117 parallel axes zoom, tran\r"
,"061118 rearrange parallel axes\r"
,"061119 picked point polyline at front in parallel axes space\r"
,"061120 picked pt on markers button, zoom around center,... \r"
,"061121 scaled NDimension data\r"
,"061123 re bugs in 061121...\r"
,"061124 subvert show object properties (pickshow) for SOURCE,TARGET choice\r"
,"061125 move points by color from source to target\r"
,"061126 internal mean,sigma\r"
,"061128 mean,sigma labels, only ON points with source color\r"
,"       ... groups, etc, do not alloc bboxstructure, use raw Widget ... \r"
,"061129 mean,sigmas in separate lists with same name\r"
,"061201 average, sigma list params, and cleaner logic in dialog boxes \r"
," if {ptIDs} then can drop points with too few coords, ignore extra coords\r"
,"061216 <screen top-left: comment sigmas to only .1>, sigma lines UNPICKABLE\r"
,"061216b list allocations cleaned up, avoids crash on save\r"
,"061222 -dumpprobe : atoms info sent to probe also sent to stderr\r"
,"\r"
," ---  070109 control so pointmaster comes up OFF ????\r"
,"070315 multi-dimension view option to keep centered on picked point\r"
,"070514 extra places (.3) in angle and dihedral measures to screen & bondrot\r"
,"070617 -6.48- noslider to not show that bondrot slider in bondrot window\r"
,"    Tools/finestep change bondrot step size, Tools/Kludges/show all sliders\r"
,"070622 text region and hypertext expression expanded for suitefit \r"
,"070702 -6.50- NOT load just saved kin. Output-only-visible honors OFF lists\r"
,"070705 suitefit: output visible slider angles, output only ON as pdb coord\r"
,"070706  rotation angle output as inputable hypertext\r"
,"070709 hpertext command max size 2048\r"
,"070829 write-rotated H output flag, for suitefitroc.kin\r"
,"070830 wider button panel for longer RNA names\r"
,"070901 suitefit output with specific Base Types in PDB std order \r"
,"070905 -6.51- writerotated dialog for library choice,dropped 071005\r"
,"071004 Nucleotide Library to PDBv3.0 names,  Library dropped 071005\r"
,"071005 list resnumb resname, point number for suitefit alternate base order\r"
,"071113 nbondrot max from 20 to 99\r"
,"071201 picked point to stderr\r"
,"071212 isolated {...} in hypertext ignored\r"
,"071215 suitefitroc report of caption hypertext hits\r"
,"071222 re-allow hydrogens to be in rotated write-out\r"
,"080309 %.3f slider angles to tune suitefitroc_080309.kin\r"
,"080315 MAGELIST.h MAXBONDROT 200 (was 100) for double suitefitroc.kin\r"
,"080405 extra type flag array in MAGEDRAW it(7,ncount), re STYLE...\r"
,"080406 new type: multi-D ellipsoid surface: ellipsoidlist: dummied...\r"
,"080430 compile for Leopard\r"
,"080912 pixelated ribbon, triangle list: area filled with z-buffered dots\r"
,"080915,16 more efficient pixelation...\r"
,"081120 @localeulerradians phi,psi,theta As phi,theta,psi: z,x,z \r"
,"       ref Goldstein 1st pag 109 for Havranek-Baker paper 200811\r"
,"090316 tinkering with MAGE make kinemage Practice Docking Widgets\r"
,"(090402 drawline dashed: never implemented)\r"
,"090419 HelixBearHair.kin Lswitchon xrays from XL to film vs spots on film\r"
,"090626 Try Xlib call for blunt end lines, re: blk line outline\r"
,"090703 try rear/fore point rather than average for blk line outline\r"
,"090912 -6.54- foobar == foo barrier group == SEAL as if Lvoid, NOT surface\r"
,"091127 water Dock builtin\r"
,"091212 monitor last measures instead of only monitor last line\r"
,"100406 dotballlist: makes lists of dotballs from atoms in a dotlist\r"
,"100407 dotballlist work: (also fixed FOO dotball dot index)\r"
,"100818 -6.56- FOO expanded to foo-in-a-box and drop-foo-grid-point-to-atom\r"
,"101225 magekinjas: commas in output, MAGEOUT\r"
,"111002 reworked Makefile for snow leopard OSX 10.6.8... fixed MAGEBALL code\r"
,"111024 dotball dot density 10 -> 16\r"
,"120911,120913 copies of 111024: try compiling on rebuilt moroxite & kyanite\r"
,"121105 Fink compiled OpenMotif on 10.8 sorta works on 10.8, fails on 10.7\r"
,"121105 Makefile rework, etc. ...\r"
,"121105 start to clean code re warnings/errors from new compiler...\r"
,"121107 redo Makefile, clean code re warnings/errors from new compiler...\r"
,"121107 cured Ndimension seg-fault: MAGELIST.h prototype alloc, new compiler\r"
,"121108 cleanup to compiling w/o warnings, runs some tests, fails sometimes\r"
,"121109 debugging... revised Tools/Find flow and default\r"
,"121110 6.57 ... dialogs now seem OK, remote update crashes at 248 calls\r"
,"121111 6.57 version MacOSX Mountain Lion, tune 7D and dotball\r"
,"121113 7D graphics starting selection now gold: re RNA11 demo\r"
,"121202 max,min range for angle data parallel coord\r"
,"       Ndim parallel coords displayed for dotlist as well as balllist\r"
,"121208 pick ndim-param pt: UpArrow sets 3D-coord model, delta=ribose=MESS!\r"
,"121209 ndim-param->3D-coord: match delta to pucker, special suitefit\r"
,"       screen-fixed param UpArrow update of bondrot names specific\r"
,"121210 adjust z depth for screen-fixed param-space: for suitefit42NDIM7\r"
,"       mark-radii detached from zoom (e.g. ringlist), still overall scaled\r"
,"121211 static & screen group param descend to lists, markers rationalized\r"
,"121212 Linvert360yz\r"
,"121213 repaired NDIM (0,360) 0,0,0 offset for all screen sizes\r"
,"121213test test of automator dragNdrop,  look for working directory...\r"
,"121222 aka 121213 mageAutomatorApp.app, working directory further input\r"
,"121224 2D vs 3D marker sizes...\r"
,"130115 update mobile 3D model from pick in active ND parameter space \r"
,"--BUT group centered ,param, (on first point) shows non-centered markers \r" 
,"130116 NDIM pick-->suitefit FRAGILE CODE: use unique names delta-1,delta+ \r"
,"   and requires exact bondrot names in suitefit which are hard-coded into \r"
,"       MAGEANGL.c/dobondrotgrapherNDIMsuitefit() \r"
,"130119 MAGEHELP for bondrots, N DIMensions\r"
,"130303 domeasures permutes its lines,dots; corrected report of picked point\r"
,"130304 lists not inherit STATUS NOBUTTON from (e.g. implicit) subgroup\r"
,"130309 scaled docking suitefit group in 3D active group(s) of NDIM points\r"
,"       group parameter: scaled #.# \r"
,"130309x compiled with only -I/sw/include/ ... -L/sw/lib for motif stuff\r"
,"140518x DrawNew construct 5: dot product angle btw vectors 1,2 and 3,4\r"
,"140912 6.58->measures tracks change of atom positions,  multiple measures\r"
,"       disambiguate monitor=track vs monitor:displaydevice characteristics\r"
,"       construct6 e.g. PO4 -- glycosidic vectors, lashup multi-monitor\r"
,"       Serious work in MAGEANGL.c/doconstructline()...\r"
,"       NEEDED MUXMDLOG.c/DoConstructSixthDialog(), output to stderr \r"
,"140915 6.58 cont'd \r"
,"141006 repeat MAGEANGL/DoNucleicAcidParameters() with dockpointptr's \r"
,"       Lparamtrigger,Lparamtriggeron  ~ Lcontrol,Lcontrolon,Lsockettrigger\r"
,"141021 works dynamically in suitesuite as conf change and paramtrigger\r"
,"141031 NucleicAcidParameters cleanup... define PprojRatio\r"
,"141106 fix ratio, other NucleicAcidParameters\r"
,"141126 fix doconstructline logic, replace param points: float and int parts\r"
,"\r"
,"\r"
,"END\r"
}; /*magechanges()*/

/****getmageversion()*****************************************************/
void getmageversion()   /*common version number: MAGEMAIN.C */
{
   char compile[16];
   compile[0] = '\0';
#ifdef UNIX_X11
#ifdef _STATIC_
   sprintf(compile," static");
#else
   sprintf(compile," dynamic");
#endif
#endif

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678
featuring: 17D, FIND whole word, hypercaption, font-sizes meas+perp, h/v/zplot, -postscript, symbols, foo probe-score, mapfile, dock, anim, RGB<-HSV, multi-infiles HTML help, 3Dliteracy tests, Show Rotations IOprekin-pipe, simulations, appendout, dock3on3
123456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678
featuring: bondrot noslider, 17D, FIND whole word, hypercaption font-sizes, meas+perp, h/v/zplot, -postscript foo, probe-score, mapfile, dock, anim, RGB<-HSV multi-infiles, HTML help, 3Dliteracy tests Show Rotations, IOprekin-pipe, simulations, dock3on3
123456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*sprintf(features
   ,"featuring: NDIM->bondrot , 17D, FIND fragments, hypercaption" 
CRLF"font-sizes, meas+perp, h/v/zplot, -postscript"
CRLF"foo, probe-score, mapfile, dock, anim, RGB<-HSV"
CRLF"multi-infiles, HTML help, 3Dliteracy tests"
CRLF"Show Rotations, IOprekin-pipe, simulations, dock3on3"
       );*/
sprintf(features
   ,"RNA/DNAparam,NDIM->bondrot, 17D, FIND fragments, hypercaption"
CRLF"font-sizes, meas+perp, h/v/zplot, -postscript"
CRLF"foo, probe-score, mapfile, dock, anim, RGB<-HSV"
CRLF"multi-infiles, HTML help, 3Dliteracy tests"
CRLF"Show Rotations, IOprekin-pipe, simulations, dock3on3"
       );


/*COMPILE IN CHANGED DATE AND MAGE VERSION*/
         sprintf(temps," dated:  141126 /sw/libXm " ); 
         MAGEVERSION = (float)6.58; /*display format only has 2 decimal places*/
/*CAN TINKER WITH (SO NEEDS TO BE CHECKED) this word character string to put in
  x or other version number modifiers to be displayed on banner */
         sprintf(word," %.2f,%s ",MAGEVERSION,compile);
         
#ifdef EGAM
    sprintf(version,"EGAM version %s, %s",word,temps);
#else
#ifdef __MWERKS__         /*__MWERKS__ compiler specific*/
#if defined(powerc)  
    sprintf(version,"MAGE CW_PPC version %s, %s",word,temps);
#else
    sprintf(version,"MAGE CW_68K version %s, %s",word,temps);
#endif
#else
    sprintf(version,"MAGE version %s, %s",word,temps);
#endif
#endif
#ifdef MACINTOSH
    sprintf(modification,"256 color palette, quit restores original color mode"
    );
#endif
#ifdef PCWINDOWS
    sprintf(modification," PC MAGE for Windows95/98/NT/2000/XP"
    CRLF" written by David C. Richardson and Brent K. Presley");
#endif
#ifdef UNIX_X11
    sprintf(modification," UNIX X11 tuned for Mac OS X 10.8, OpenMotif");/*121109*/
#endif
#ifdef UNIX_PEX
    sprintf(modification," UNIX PEX alpha test");
#endif
}
/*___getmageversion()____________________________________________________*/

/****mageinitialvalues()***************************************************/
void    mageinitialvalues()
{
  int i;

  /*021222 explicite palette generation so can do HSV or orig RGB */
  loadmyfullpalette(2); /*1 for orig, 2 for KiNG HSV base*/
  loadmygrayscalepalette(); /*041001*/

  /*020718 need to initialize matrix for, e.g., appended frames*/
  initrot(); /*MAGEANGL: initializes all view matricies*/
  /*Lview[1] = 1; */ /*declare existance of view 1*/
  /*Lreset[1] = 1;*/ /*declare matrix 1 to be setable*/
  /*cannot use resetrot(1) to initializes working matrix as view 1*/
            a11 = ma[1][1];
            a12 = ma[1][2];
            a13 = ma[1][3];
            a21 = ma[1][4];
            a22 = ma[1][5];
            a23 = ma[1][6];
            a31 = ma[1][7];
            a32 = ma[1][8];
            a33 = ma[1][9];
  framenumber = -1;/*effective previous framenumber==-1: kinemage params reset*/

  /*------------initial dialog allows change--------------------------*/
    MAXNUM = 4000;   /*atoms and points total stored per kinemage*/
    MAXDRAWS = 3500; /*moveto and drawto points at any time*/

  /*------------------------------------------------------------------*/


    MAXWORDCOM = 3500; /*total number of characters comment or wordlist*/
                        /*originally 23500 but anything will do */
    MAXCOM = 9000; /*starting total number all ptID characters*/ /*960511*/
    nwordcom = 1; /*starts at 1, since 0 means no comment or wordlist*/
    ncom = 1; /*starts at 1, since 0 means no ptIDs */ /*960511*/
              /* com[0]=='\0' dummy so run out of room without crashing*/
  /*--set array pointers==NULL so can tell if allocate them-----------*/

    memset(it,0,8);   /* optimized ,080405 was 7*/
    /*for (i = 0; i < 8; ++i)it[i] = NULL; 080405 was 7*/
    itpointptr = NULL;
    linkit = NULL;
    thelink[0] = NULL;
    thelink[1] = NULL;
    com = NULL;
    wordcom = NULL;
  /*------------------------------------------------------------------*/
    
    /*maxValue=10000; 981010*/
    /*minValue = 0;   981010*/
    width = 5;
if(IUX)
{
    GWIDTH = 800;   /* UNIX effective graphics area window width */
    GHEIGHT = 800;  /* graphics window height*/
    GZDEEP = 800;
}
else /*if(IMAC||IPC)*/
{
    GWIDTH = 400;   /* Mac effective graphics area window width */
    GHEIGHT = 400;  /* graphics window height*/
    GZDEEP = 400;
}
    WIDTH =  GWIDTH; /*GWIDTH & GHEIGHT adjusted when make grafWindow*/
    HEIGHT = GHEIGHT;/* WIDTH & HEIGHT reset then also */

    oldmaxwide=GWIDTH;
    CHEIGHT = 80;   /* caption window height */
    MENUHEIGHT = 30;   /* menubar height (UNIX) */
#ifdef UNIX_PEX
    MESSHEIGHT = 30;   /* message place height (UNIX) */
#else
    MESSHEIGHT = 0;   /* message place height (UNIX) */
#endif
    TWIDTH = 580;   /*reasonable width for a text window*/ /*980919 */
    MARGIN =  20;   /*minimal space to right of text, caption windows*/ /*980919*/
    TOPBAR =  42;   /*Space for junk at top, Mac needs 38*/ /*980919*/
    BOTTOMBAR= 6;  /*PC does in fact need space allowance here*/ /*980919*/
    MIDBAR =  32;   /*allows for Caption Bar between text and Caption*/ /*980919*/
    BORDER =   6;
if(IUX)
{
    BWIDTH = 125;   /*200 buttons,slider area width in graphics window*/
    BUTTWINWIDTH = BWIDTH; /*041108*/
    ZWIDTH =  45;   /* 60 for zoom, zclip, ztran scroll bars area in BWIDTH */
    ZSLIDERWIDTH=15; /*constant for zoomzsliders bar thickness UNIX */
    ALERTHEIGHT = 150; /*of individual alertstr panels in dialog box, UNIX*/ 
    FONTSIZE = 20; /*UNIX effective height allowance for menu,dialog text font*/
}
else
{
    BWIDTH = 100;   /* buttons,slider area width in Mac graphics window*/
                    /*on Mac graphics screen: 100 for buttons with labels */
    ZWIDTH =  30;   /* 30 for zoom, zclip, ztran scroll bars */
}
    RWIDTH =   0;   /*variable:  0 initially for bondrotation scroll bars */
    ROTSWIDTH=100;  /*Mac allowance for bondrotation scroll bars, PC resets*/
    ROTWINDWIDTH=250; /*200 constant for bondrotsliders separate UNIX window*/
    ROTSLIDERWIDTH=20; /*constant for bondrotsliders bar thickness UNIX */
    izclip = 200;
        /*std screen box 400x400x400, 0 at ctr, zclip 200 from ctr*/
    /*fzclip = 1.0;*/ /*factor==1.0 for std box, scale if screen box resized */
    fzclip = (float)GZDEEP/(float)400;/*std izclip defined in std screen box*/
    /*fzclip mediates between internal izclip value adjusted for box expansion*/
    /* and external izclip which pretends to be in a 400x400x400 box */
    izlimit = izclip;
    Lzlimitscope   = 0; /*selected lists obey extra zlimited clipping planes*/
    zoom = 1.0;
    Lspanfromzoom = 0; /*010913*/
    iztran = 0;

    Lstereo = 0;
    LTBstereo = 0;  /*970408*/
#ifdef UNIX_X11
    TBYoffset = 25; /*970607:  35 SGI value on IRIS*/
#else
    TBYoffset = 21; /*970603*/ /*Mac value on Old Bill*/
#endif
    defaultangle = 6.0;
    stereoangle = defaultangle;
    ieyedefault = GWIDTH/2;
    ieyewide = ieyedefault;
    ieyeposdefault = 2000;
    ieyeposition = ieyeposdefault;
    Lmiddle = 1;
    Lrotateye = 0; /*ON: rotation around eye position*/
    Lcenterinfo = 0; /*ON: write ptID and dist near center line*/
    Lrecalculate = 0;
    Lmultibins = 0;
    multibins = 1;
    Lkeepstereo = 0;
    Lkeepthinline = 0;
    Scale = 1.0;

    Linvert360yz = 0; /*121212*/
    Linvert360yzon = 0; /*121212*/
    LSCREENpoints = 0; /*121212*/

    nbondrot = 0;  /*initially should be 0, no provision for bond rotation*/
    distuser = 0.0;  /*distance and angle preferences for constructing lines*/
    angleuser = 0.0;
    dihedraluser = 0.0;
    diststored = (float)1.54; /*distance and angles stored for construction*/
    anglestored = (float)109.6;
    dihedralstored = (float)120.0;
    distarrow = (float)0.30; /*distance (length) of tine stored for arrow*/
    anglearrow = (float)30.0; /*angle of tine stored for arrow*/
    dihedralarrow = (float)90.0; /*360/90 == 4 tines on arrowhead*/
    Ldrawunpickable = 0; 
    shortenline = 0.0; /*draw new amount to shorten lines, e.g. H-bonds*/

    ibigfont = 0;  /*ibigtype=40 for PC with clunky old big type font*/
           /* moves the rightside screen messages ibigfont pixels left*/

    Initialized = 0;
    SliderWidth = 0;

    Loriginatctr = 0;
    ixctr = GWIDTH/2;
    iyctr = GHEIGHT/2;
    izctr = GZDEEP/2;

    textHeight = 1;
    captHeight = 1;
    widMax = 1;
    nxyz = 1;
    nmovedraws=0;
    ntotaltext=0;
    
    /*sprintf(draglinestr,"dragline");*/ /*960603*/
    /*condition(draglinestr);*/
    Lkinformat = 1; /*output for rotated parts*/    
    Lrotangleout = 0; /*angles of visible-slider rotated parts 070705*/
    Lformattest = 0; /*MAGE does best it can silently unless this on */
    Lflat = 0; /*ON: mouse does translations rather than rotations*/
    Lscroll = 0; /*xy rotations instead of translation by mouse*/
    Lhorizon = 0; /*ON: mouse only rotates around vertical y axis*/
    Lpickmarkcriteria=0;/*FLAGs: pickmarker extentions in axial directions*/
    if(LGUINO != 1) Lpostscript = 0;
    if(LGUINO != 2 || LGUINO != 3) Lraster3Drender = 0;
    Lspecialloop = 0;
    Lnewlabels = 0;
    Lnewlabelson = 0;
    Lmeasureddihedral = 0;
    Lmeasureddihedralon = 0;
    Lnewballs = 0;
    Lnewballson = 0;
    Lmonitor = 0;  /*last line length*/
    Lprocessing = 0;
    Liveoffscreenbitmap = 0; /*flag for off-screen bitmap */ 
    Lquesanswers = 0; /*flag for questions in text part of kinemage*/

	memset(alertstr,'\0',MAXALERTSTR);/*optimized array initialization*/
	memset(word,'\0',256);    /* optimized array initialization */
	memset(temps,'\0',256);   /* optimized array initialization */
    /* for(n=0; n<256; n++) alertstr[n] = '\0';*/ /*clear alert report*/
    /* for(n=0; n<256; n++) word[n] = '\0';  */ /* clear alert report */
    /* for(n=0; n<256; n++) temps[n] = '\0';  */ /* clear alert report */ 

/*icommax = 0;  960511 */ /*maximum allocated pointID array*/

    firstlistptr = NULL;
    thislistptr = NULL;
    lastlistptr = NULL;
    firstsgrpptr = NULL;
    thissgrpptr = NULL;
    lastsgrpptr = NULL;
    firstgrupptr = NULL;
    thisgrupptr = NULL;
    lastgrupptr = NULL;
    firstmasterptr = NULL;
    lastmasterptr = NULL;
    firstbboxptr = NULL;
    lastbboxptr = NULL;

drawgrupptr = NULL;
measuregrupptr = NULL;
dotballgrupptr = NULL;
animbboxptr = NULL;
anim2bboxptr = NULL;
pickbboxptr = NULL;
picktablebboxptr = NULL;
pickshowbboxptr = NULL;
movepointsbboxptr = NULL; /*061126*/
meansigmasbboxptr = NULL; /*061126*/
pickcolorbboxptr = NULL; 
zclipbboxptr = NULL;
onscreeninfobboxptr = NULL; /*060122*/
drawlabelsbboxptr = NULL;
drawballsbboxptr = NULL;
drawlinebboxptr = NULL;
construct4bboxptr = NULL; /*971122*/
construct5bboxptr = NULL; /*971122*/
construct6bboxptr = NULL; /*140912*/
dock3on3bboxptr = NULL; /*020305*/
draglinebboxptr = NULL;
monitorbboxptr = NULL;
eraselinebboxptr = NULL;
pickerasebboxptr = NULL;
prunebboxptr = NULL;
punchbboxptr = NULL;
undopbboxptr = NULL;

    updateratelimitdefault = 5.0; /*970508 has shift-key over-ride*/
 
#ifdef MACINTOSH
LimportOK = 0; /*remote data import NOT on old Mac [File/data import] 020514 */
#else
LimportOK = 1; /*remote data import OK to try [File/data import] 020514 */
#endif
Lappend = 0; /*new file makes new kinemage unless Lappend flagged*/
Lappendout = 0; /*output to new file unless Lappendout flagged 020303*/
LPDBformat = 0; /*writerotated: 070829 explicit*/
Lhydrogensout = 1; /*output hydrogens to new file 070829*/
LhydrogensSep = 1; /*output hydrogens separately from heavy atoms 070829*/
   /*separate hydrogens after heavyatoms works for 1residue/group mutations*/
LrotoutSORTED = 0; /*070901*/
Lupdate = 0;
Linhibiton = 0; /*inhibits update call to remote program*/
Linhibitbutton = 0;
Ldeletemapcontours = 0;
Ldeleteprobedots = 0;
Lgetmapfilename = 0;
Lignorecaption = 0; /*set to avoid accummulating probe captions*/
update_res_num = 0;
update_file_name[0] = '\0';
update_command_line[0] = '\0';
import_command_line[0] = '\0';
input_command_line[0] = '\0';
Nthisgroupkind = 0; /*030312*/
activebondrotlistptr = NULL;
lensFactor = 7.0;
killradius = 20;
Lfloat = 1; /*1: float perspective, 0: NOT floating point in inner loop*/

tablefontsize  = 14; /*default set before any @tablefontsize !!!!*/
tablecolscale = (float)1.3; /*default set here to be before any @tablescale !!!!*/
Ltablepresent = 0; /*but input must determine if a table is present*/
tablemarkradius = (float)0.0; /*0 invokes screen adusted markers*/ /*010916*/
tablefanradius = (float)0.4;
tablefancore = (float)0.1;
Ltableblackbkg = 0; /*MAGETABL.h 010924*/

Lpipeactive = 0; 
Lpipebufferloaded = 0;
Lstartinpipe = 0; 
Ldonewithpipe = 0;

Linfofontsize   = 0; /*0: no deliberate font size control*/
Llabelsfontsize = 0; /*, resize them with window*/
Lwordsfontsize  = 0;
Ltablefontsize  = 0;
Ldockingscope   = 0;
finestepdefault = (float)0.05; /*fine step size for z centering and docking*/
finestep = finestepdefault;
degreestepdefault = (float)0.25;/*fine step size on bondrot slider ends 070617*/
DEGSTEP = degreestepdefault; /*070617*/

Lscalemarkers = 0; /*000804*/
Lscalefonts = 0; /*010520*/
Lsockets = 0; /*001202*/
Lsocketiteminput = 0; /*001204*/
Lsocketextra = 0; /*031228*/
Lfinger = 0;
Lfingerin = 0;
#ifdef _STDFONTCURSORS_
Lfontcursor = 1;
#else
Lfontcursor = 0;
#endif
sprintf(currentcolorname,"plaid"); /*Macintosh pickcolorDialog*/
currentcolornumber = numberofcolor(currentcolorname);
Lcurrentbkgwhite = 0; /*0 for black, 1 for white*/
currentaspect = 0; /*010603*/
Lnewpalette = 0; /*010604*/

/*010609,011023,020207*/
nextanimatetick=0; nextrocktick=0;
Lautoanimate=0;Lautorock=0;Lnewrock=0;Lmouseactive=0;
Lmickeymouse=1;/*031118 default to empower Mickey Mouse*/
Lmickeymouserun=0;Lpausemickeymouse=0;
animatesteptime=2;rocksteptime=0;
rockmin=-5;rockmax=5;rockcycletime=2;rocknumsteps=20;

L5depths = 1; /*010927*/
L3depths = 0; /*redundant as long as only 2 choices 010927*/
LR3Dsquaredcolor=0;/*old raster3D: 1 for square of fractional color components*/
Lpointcolor = 0; /*Lpickcoloron:changecolor: keeps picking color on points*/
#ifdef UNIX_X11
topbrot = NULL; /*020617 safety*/
brotwinholder = NULL; /*020617 safety*/
brotwindow = NULL; /*020617 safety*/
#endif
for(i=0;i<MAXQCHAR;i++){usernamestr[i]='\0';} /*020816*/
Ltestoutfile = 0; /*flags existance of output file of test results*/
Ltestscorepending = 0; /*protection from non-modal, multi-threaded-like*/
Ltestreportpending = 0;/*behavior of file output and dialog boxes*/
Ltesttimeend = 0; /*user request to end test in a particular kinemage*/
Laccumulate = 0; /*accumulate animate groups  030111*/
search1str[0] = '\0';
search2str[0] = '\0';
searchstatusstr[0] = '\0'; /*FIND 030121*/
Lsearchstatus = 0;
LmouseLdown = 0;
LmouseMdown = 0;
LmouseRdown = 0;
referencegrupptr = NULL; /*egam 030215*/
mobilegrupptr = NULL; /*egam 030215*/
Legamickdocking = 0; /*egam 030216*/
Modeegamickdocking = 0; /*egam 030217*/
Lneedannealmobile = 0; /*egam 030216*/
Legamickchoice = 0; /*egam 030217*/
Negamickshape = 0; /*egam 030217*/
Lfreeegamickmobile = 0; /*egam 030217*/
Legamickfoo = 0; /*egam 030223 capture the foo*/
Legamickfooon = 0; /*compute foo fill */
LegamickrepeatOK = 0; /*egam 030223*/
Legamickrepeat = 0; /*egam 030225*/
ACTIVE_DLOG = 0;
NEXT_DLOG = 0;
Lappendscreentext = 0; /*030311*/
Lreplacecaptiontext = 0; /*030315*/
LneedSetsinGrafWindow = 0; /*030320*/
Lrescalekinemage = 0; /*030329*/
Lredrawvector = 0; /*030329*/
Lfoocavity = 0; /*030330*/
Lfoosealer = 0; /*030411*/
Lfoosealeron = 0; /*030411*/
Lresizeactive = 0; /*040509*/
#ifdef UNIX_X11
   Timerforresize = 0; /*040605 XtIntervalID is some kind of integer????*/
#endif
Linputactive = 0; /*040605*/
/*debug print statements to track input looping 040605*/
mess1[0]='\0';mess2[0]='\0';mess3[0]='\0';mess4[0]='\0';mess5[0]='\0';/*040605*/
nmess1=0;nmess2=0;nmess3=0;nmess4=0;nmess5=0;/*040605*/
Lgrayscale = 0; /*040926*/
Lgrayscalemap = 0; /*040926*/
Lbuttonseparate = 0; /*041108 separate scrolling window for buttons, UNIX */
update_res_model = -1; /*050121 default flag not to send model number*/
Lfindwholeword = 0; /*121109 default to search for string frag vs whole word*/
Lallocerror = 0; /*051114 report allocation error to UNIX command window*/
Lclipmiddle = 0; /*051214 not show line if both ends out of box*/
Lreloadkinemage = 0; /*060121 from ANG[7] rna conformers, later from ND array*/
Lreplacemarkers = 0; /*061116 after reloadkinemage so markers on that 3D point*/
NX = 0; /*index to ANG[7] 060121, to nD dimname 060622*/
NY = 1; /*delta-1,epsilon-1,zeta-1,alpha,beta,gamma,delta*/
NZ = 2; /*  0    ,   1     ,  2   ,  3  , 4  ,  5  ,  6  */
strcpy(NXstr,"DELTA-1");
strcpy(NYstr,"EPSILON-1");
strcpy(NZstr,"ZETA-1");
#ifdef OLDCODE /*060622*/
L7dimensions = 0; /*060121*/
L7dimnames = 0; /*060129*/
#endif
LNDparallelaxes = 0; /*NDIM ball as parallel axes 061114*/
paraAXtranx = 0; /*061117*/
paraAXtrany = 0; /*061117*/
paraAXzoom = 0; /*061117*/
paraAXnum = NULL; /*061118 actual number order of the parallel axes*/
LNDpointsinfo = 0; /*NDIM points info toggle show only one color 061115*/
LNDpickedinfo = 0; /*NDIM points info toggle on-off picked point 061117*/
dimensionptr = NULL; /*060622,061028,061121 renamed*/
LNdimensions = 0; /*060622*/
LNdimnames = 0; /*060622*/
Lonscreeninfo = 0;   /*060122*/
Lonscreeninfoon = 0; /*060122*/
sprintf(onscreeninfostr,"X: %s, Y: %s, Z: %s",NXstr,NYstr,NZstr);
SELECTINGlistbboxptr = NULL; /*060121*/
LSELECTINGlist = 0;   /*060121*/
LSELECTINGliston = 0; /*060121*/
UnSELECTINGbboxptr = NULL; /*060206*/
LUnSELECTING = 0;   /*060206*/
LUnSELECTINGon = 1; /*060206*/
oneSELECTINGbboxptr = NULL; /*060207*/
LoneSELECTING = 0;   /*060207*/
LoneSELECTINGon = 0; /*060207*/
ncolorSELECTING = 3;  /*121113 3 gold, was 7 sea 060121*/
ncolornotSELECTING = 23;  /* white 060121*/
radiusSELECTING = 20;

Laverageout = 0; /*060604 e.g. average distribution of 7D ON*/
NDIMout = 0; /*060604 Number of DIMensions, 3, 7, flags inconsistency */
Npassout = 0; /*060604 tracks passes through points calculating mean & sigma*/
 
Lcentered = 0; /*130115 existence of a group to keep centered, e.g. suitefit*/

sourcegrupptr = NULL; /*061124*/
sourcesgrpptr = NULL; /*061124*/
sourcelistptr = NULL; /*061124*/
targetgrupptr = NULL; /*061124*/
targetsgrpptr = NULL; /*061124*/
targetlistptr = NULL; /*061124*/
meansigmasgrupptr = NULL; /*061126*/
meansigmassgrpptr = NULL; /*061126*/
meansigmaslistptr = NULL; /*061126*/
Ltargeteditactivity = 1;  /*061126*/
Ldumpprobe = 0; /*061222 probe atoms to stderr*/
LNDaxesPickedCtr = 0; /*view change only ND axes stay centered on point 070315*/
ctrpickedpointptr = NULL; /*point last centered on 070315*/
Lallbondrotsliders = 0; /*trumps noslider on vectorlist bondrot line 070617*/
conformerstr[0] = '\0';    /*071215 for suitefitroc*/
ribose3hyperstr[0] = '\0'; /*071215*/
ribose5hyperstr[0] = '\0'; /*071215*/

measured1pointptr = NULL; /*091212 for surveillance*/
measured2pointptr = NULL; /*091212 for surveillance*/
measured3pointptr = NULL; /*091212 for surveillance*/
measured4pointptr = NULL; /*091212 for surveillance*/
 
}
/*___mageinitialvalues()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setMageinitDLOGreport()*************************************************/
void    setMageinitDLOGreport(void)
{                                          /*950110 CRLF */
   sprintf(alertstr, 
          "MAGE: copyright \251 1990-2014 by David C. Richardson"
      CRLF"Little River Institute, 5820 Old Stony Way, Durham NC 27705"
      CRLF"dcr@kinemage.biochem.duke.edu"      
      CRLF"Biochemistry Dept., Duke University, North Carolina 27710, USA"
      CRLF CRLF"MAGE may be freely copied and distributed."
   ); /*copyright update 140518*/
   if(fp != NULL)
   {/*990129 rev 020830*/
      if(Lkintitle) sprintf(word,"CURRENT KINEMAGE: %d - %s"
          ,thiskinenum,kintitlestr);
      else sprintf(word," ");
      if(Lkincopyright) sprintf(temps,"copyright: %s",kincopyrightstr);
      else sprintf(temps," ");
      sprintf(alertstr2,
             "%s"
         CRLF"%s"
         CRLF"CURRENT INPUT FILE: %s"
         CRLF"%s"
         CRLF"%s"
             ,version,modification,NameStr,word,temps);
      alertstr2[255] = '\0'; /*safety*/
   }
   else if(Lkinfile > 1)
   {
      sprintf(alertstr2,
             "%s"
         CRLF"%s"
         CRLF"Showing internally generated text and/or kinemage"
             ,version,modification);
   }   
   else
   {
      sprintf(alertstr2,
             "%s"
         CRLF"%s"
             ,version,modification);
   }   
   sprintf(alertstr3, "%s",features);
}            
/*___setMageinitDLOGreport()________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****clearfocusxyzstr()******************************************************/
void    clearfocusxyzstr()
{
    /*sprintf(focusxyzstr,"\0");*/ /*121108 compiler objects to this string*/
    focusxyzstr[0]='\0'; /*121108 clear focusxyzstr for it to be used */
    condition(focusxyzstr);       /* for x,y,z report on graph screen */
}
/*___clearfocusxyzstr()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****magetimercheck()*********************************************************/
void    magetimercheck()  /*020816*/
{/*timed-test branch of main loop*/
   /*This subroutine is continually as long as Ltestmode==1*/
   /*Sections that control flow by calls that must be called only once*/
   /*must be portected by single-shot logicals*/

   static int   oldtimersec = 0;
   static int   LsingleshotDialog = 0;
   static int   LsingleshotnextORout = 0;
   static long  previousclickcount = 0;
   long currentclickcount = 0;

   if(  Lmagetimerrunning
      &&(  (Lmagetimerset && theclocktickcount() >= magetimeticklimit)
         ||Ltesttimeend ) /*out-of-time or user requested test to end*/
     )
   {/*last time through for this timed kinemage*/
      /*magetimeticklimit set to anticipated tickcount*/
      /*this will miss by one cycle at time of clock roll-over*/
      /*when theclocktickcount very small and before magetimeticklimit recalc*/
      Lmagetimerrunning = 0; /*now past timing a test*/
      Ltesttimeend = 0; /*now past end of timed test*/
      magetimeticklimit = 0; /*one chance per kinemage load*/
      magetimeremaining = 0;
      Lmagetimerset = 0;
      magetimerset(0); /*MPCMAIN: turnoff interval timer*/
      /*writes entire, possibly modified, kinemage, with special name*/
      Ltestcontinue = 0; /*inhibit continuing with current test; 1 at input*/
      Ltestscorepending = 1; /*protection from non-modal, multi-threaded-like*/
      Ltestreportpending = 1;/*behavior of file output and dialog boxes*/
      /*need to report before next kinemage whatever outcome of scoretest()*/
      Ltestscoreavailable = scoretest();  /*MAGESCOR.c*/
      if(Ltestscoreavailable)
      {
         Scorepositiveold = Scorepositive;
         Scorenegativeold = Scorenegative;
         Scoretotalold = Scoretotal;
      }
      else
      {
         Scorepositiveold = 0;
         Scorenegativeold = 0;
         Scoretotalold = 0;
      }
      kinold = thiskinenum;
      Ltestscorepending = 0; /*protection from non-modal, multi-threaded-like*/
        /*which should not be needed if this routine waits for scoretest rtn*/
      writeoutput(17); /*17 for automatic blind output___OUT.C*/
      LsingleshotDialog = 1; /*call dialog just once*/
      redrawvec(); /* nudge program into another event cycle for report dialog*/
   }
   else if(usernamestr[0]!='\0')
   {/*user name present, see if ready to get on with the test*/
     if(    Ltestcontinue      /*could be incoming next before last's report*/
         && !Ltestreportpending  /*controls to finish last ones report*/
         && !LsingleshotDialog
         && !LsingleshotnextORout
       )
     {/*continue: NOT at the end, reset timer and get on with the test*/
       if(Lmagetimerset==0)
       {/*first time through for this timed kinemage*/
          magetimeremaining = magetesttime; /*initialization, decimal minutes*/
          currentclickcount = theclocktickcount(); /*decimal ticks==60th-secs*/
          previousclickcount = currentclickcount; /*beware roll-over*/
          /*ticks == 60*60*minutes*/
          magetimeticklimit=(long)(magetimeremaining*60*60)+currentclickcount;
          Lmagetimerset = 1;
          Lmagetimerrunning = 1;
          magetimermin = (int)magetimeremaining;
          magetimersec = (int)(magetimeremaining*60 - (float)magetimermin*60);
          oldtimersec = magetimersec;
          magetimerset(1); /*MPCMAIN: turn on 1 sec interval timer*/
          redrawvec();
       }
       else
       {/*within inteval, recompute time remaining, beware clock roll-over*/
          currentclickcount = theclocktickcount(); /*call once*/ /*020919*/
          if(currentclickcount < previousclickcount)
          {/*roll over occurred, recompute target clicks for timer end*/
            magetimeticklimit=(long)(magetimeremaining*60*60)+currentclickcount;
          }
          /*The only magetimeremaining recalculation: protected from roll-over*/
          magetimeremaining =
                (float)(magetimeticklimit - currentclickcount)/3600;
          previousclickcount = currentclickcount; /*reset*/
          magetimermin = (int)magetimeremaining;
          magetimersec = (int)(magetimeremaining*60 - (float)magetimermin*60);
       }
       if(oldtimersec != magetimersec)
       {/*seconds have changed*/
          oldtimersec = magetimersec;
          redrawvec();
       }
     }/*continue*/
     else if(!Ltestscorepending && Ltestreportpending && LsingleshotDialog) 
     {/*report results to user and perhaps prepare them for continuing test*/
        if(LnextOK)
        {/*get in the next kin with putative instructions in caption*/
              getnextkinemage(); /*MAGEFILE.c*/
              Ltestnext = 1;
        }
        else {Ltestnext = 0;}
        /*must have saved the previous scores for the report*/
        DofilenameDialog(); /*reports name of file already written MAGEDLOG*/
        LsingleshotDialog = 0; /*just do call once*/
           /*sets Ltestreportpending = 0 */
           /*call this for current score before proceeding to next kinemage*/
        LsingleshotnextORout = 1; /*companion to Ltestreportpending*/
     }
     else if(!Ltestreportpending && LsingleshotnextORout) /*single-shot*/
     {/*ready to go on to next kinemage or quit*/
        if(Ltestnext==0)
        {/*at end of kinemage file, quit*/
           /*turn off all flags so recursive checking will skip*/
           Lpruneactive = 0;
           Ldrawactive = 0;
           Lnewviewactive = 0;
           Lnewstuffactive = 0;
           Lmagetimer = 0; 
           quitMAGE(); /*MACMENU,MPCMAIN,MUXMMENU*/
        }
        LsingleshotnextORout = 0;
     }
   }
   else
   {/*requires a user name*/
      Lmagetimerrunning = 0; /*stop clock to give time for dialog*/
      Lmagetimerset = 0;
      getusername(); /*MAGEMENU though as of 020816, not a menu item*/
         /*calls DousernameDialog()*/
      if(Lcancel==1)
      {/*invalid user name*/
         usernamestr[0]='\0'; /*force null username to repeat dialog*/
         Lcancel = 0;
      }
   }
}
/*___magetimercheck()________________________________________________________*/

/****changecheckrouter()******************************************************/
void changecheckrouter()  /*020902*/
{
   if(  !Lnosavealert 
      &&(Lpruneactive||Ldrawactive||Lnewviewactive||Lnewstuffactive) )
   {/*check dialog sets ireturnflag*/
      DoChangeCheckDialog();
   }
   else
   {
      ireturnflag = 0;
      if(LquitMAGE) quitMAGE(); /*MACMENU,MPCMAIN,MUXMMENU*/
      else if(Lgetnewfile) OpenNewFile(); /*____DLOG.c*/
      else if(Lclosedrawstuff) closedrawstuff(); /*MAGEMENU*/
      else if(Lgetnextkinemage) getnextkinemage(); /*MAGEFILE.C*/
      else if(Lgetkinemagechoice) getkinemagechoice(); /*MAGEUTIL*/
   }
}
/*___changecheckrouter()_____________________________________________________*/

/****allocdisplayarrays()*****************************************************/
void    allocdisplayarrays(int more)
{
/*    int    it[8][MAXDRAWS];   */   /* size = 2*MAXDRAWS*7 9310:7->8 080405*/
/*    long   linkit[MAXDRAWS];  */   /* size = 4*MAXDRAWS */
/*    pointstructptr* itpointptr[MAXDRAWS] */ /* size = 4*MAXDRAWS */

    int  j, ifail, recycle, moretry;
    int   *ittemp[8]; /*was 7 080405*/
    long  *linkittemp;
    pointstructptr* itpointptrtemp = NULL;

    /*more==0 for first call to allocate initial arrays*/
    moretry = more;
    recycle = 1;
    while(recycle==1)
    {/*recycle allocation trials*/
      ifail = 0;
      if( (MAXDRAWS+more) < 0) ifail = 1; /*increase rolls number to negative*/
      if(ifail == 0)
      {/*not failed yet, so do it[]*/
        for(j = 0; j < 8 ; j++) /*080405 now 0 to 7 incl*/
        {/*it[0-7]*/
          if(more==0)
          {
              it[j] = (int *)malloc(sizeof(int)*(long)MAXDRAWS); /*080405int*/
              if(it[j]==NULL) ifail = 1;
          }
          else
          {
             ittemp[j]=(int *)realloc(it[j],sizeof(int)*(long)(MAXDRAWS+more));
             if(ittemp[j]==NULL) ifail = 1;
          }
        }/*it[0-6], it[0-7] 080405*/
      }/*doing it[]*/
      if(ifail == 0)
      {/*not failed yet, so do linkit*/
            if(more==0)
            {
                linkit = (long *)malloc(sizeof(long)*(long)MAXDRAWS);
                if(linkit==NULL) ifail = 1;
            }
            else
            {
                linkittemp = (long *)realloc(linkit,sizeof(long)*(long)(MAXDRAWS+more));
                if(linkittemp==NULL) ifail = 1;
            }
      }/*doing linkit*/
      if(ifail == 0)
      {/*not failed yet, so do itpointptr*/
            if(more==0)
            {
                itpointptr = (pointstructptr*)malloc(
                                              sizeof(pointstructptr)*MAXDRAWS);
                if(itpointptr==NULL) ifail = 1;
            }
            else
            {
                itpointptrtemp = (pointstructptr*)realloc(itpointptr
                                    ,sizeof(pointstructptr)*(MAXDRAWS+more));
                if(itpointptrtemp==NULL) ifail = 1;
            }
      }/*doing itpointptr*/

        if(more>0 && ifail==1)
        {/*failed to do a reallocation, reduce request and try again*/
            more = more/2;
            if(more > 3) recycle = 1;
            else    recycle = 0;
        }
        else
             recycle = 0;
    }/*recycle allocation trials*/

    if(more>0 && ifail==0)
    {/*reassign pointers*/
        for(j=0;j<=7;j++) {it[j] = ittemp[j];} /*080405 now 0-7 incl*/
        linkit = linkittemp;
        itpointptr = itpointptrtemp;
        MAXDRAWS = MAXDRAWS+(long)more; /*increase limit*/
    }

    if(more==0 && ifail)
    {/*failure at first load, report real trouble*/
        sprintf(alertstr,"failure at first load of display arrays");
        sprintf(alertstr2," ");
        sprintf(alertstr3," ");
        DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
    }

    if(Ltest && !Limitmaxdraws) /*970410*/
              /*970104 only report failure in test mode so not cycle on*/
              /*updating graphics window after dialog box*/
    {
        if(more>0 && ifail>0)
            sprintf(alertstr,"+%d reallocation failed, MAXDRAWS remains: %ld"
             ,moretry,MAXDRAWS);
        else
            sprintf(alertstr,"MAXDRAWS== %ld",MAXDRAWS);

        sprintf(alertstr2,
          // "alloc: it[0]==%ld, it[1]==%ld, it[2]==%ld, it[3]==%ld"CRLF
          // "alloc: it[4]==%ld, it[5]==%ld, it[6]==%ld, it[7]==%ld"CRLF
          // "alloc: linkit==%ld, itpointptr==%ld" /*121107 %p for address*/
           "alloc: it[0]==%p, it[1]==%p, it[2]==%p, it[3]==%p"CRLF
           "alloc: it[4]==%p, it[5]==%p, it[6]==%p, it[7]==%p"CRLF
           "alloc: linkit==%p, itpointptr==%p" /*121107 %p for address*/
         ,it[0],it[1],it[2],it[3],it[4],it[5],it[6],it[7],linkit,itpointptr);

        sprintf(alertstr3," ");
        DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
        Limitmaxdraws=1; /*970410*/
            /*reset to 0 in MAGEINPT so each kinemage reports once*/
    }
    if(ifail&&more==0)
    {
        sprintf(alertstr,"allocation of more display space failed");  /*950110*/
        sprintf(alertstr2,"expect truncated image");
#ifdef MACINTOSH
        sprintf(alertstr3
                     ,"Perhaps preferred size too small: Before launching MAGE:"
                          CRLF"one click on MAGE, choose Info under File Menu: "
                          CRLF"change preferred size to, e.g., 2000 ");
#else
        sprintf(alertstr3," ");
#endif
        DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
        /*cleanup();*/
        /*exit(EXIT_FAILURE);*/
    }
}
/*___allocdisplayarrays()___________________________________________________*/

/****allocwordcomarray()*****************************************************/
void    allocwordcomarray(int more)
{
/* char  *wordcom;  pointer to array, for wordcom[MAXWORDCOM], size=MAXWORDCOM*/
/* the comment or wordlist character array  */

    int  ifail, recycle, moretry;
    char *wordcomtemp;

    /*more==0 for first call to allocate initial arrays*/
    moretry = more;
    recycle = 1;
    while(recycle==1)
    {/*recycle allocation trials*/
        ifail = 0;
        if( (MAXWORDCOM+more) < 0) ifail = 1;
        if(ifail==0)
        {
            if(more==0)
            {
                wordcom = (char *)malloc(sizeof(char)*MAXWORDCOM);
                if(wordcom==NULL) ifail = 1;
                else wordcom[0] = '\0'; /*place for zero length strings*/
            }
            else
            {
            wordcomtemp=(char *)realloc(wordcom,sizeof(char)*(MAXWORDCOM+more));
            if(wordcomtemp==NULL) ifail = 1;
            }
        }
        if(more>0 && ifail==1)
        {/*failed to do a reallocation, reduce request and try again*/
            more = more/2;
            if(more > 3) recycle = 1;
            else    recycle = 0;
        }
        else
             recycle = 0;
    }/*recycle allocation trials*/

    if(more>0 && ifail==0)  wordcom = wordcomtemp; /*reassign pointer*/
    if(more>0 && ifail==0) MAXWORDCOM = MAXWORDCOM + more; /*increase limit*/
    if(( Ltest||ifail) && !Limitmaxwordcom) /*970410*/
    {
        if(more>0 && ifail>0)
            sprintf(alertstr,"+%d reallocation failed, MAXWORDCOM remains: %ld"
            ,moretry,MAXWORDCOM);
        else
            sprintf(alertstr,"MAXWORDCOM == %ld",MAXWORDCOM);
        sprintf(alertstr2,"alloc: wordcom==%p",wordcom ); /*121108 char* */
        sprintf(alertstr3," ");
        DoMageDLOGreport(2); /*___DLOG.C*/  /*alertstr,alertstr2,alertstr3*/
        Limitmaxwordcom=1;/*970410*/
            /*reset to 0 in MAGEINPT so each kinemage reports only once*/
    }
    if(ifail&&more==0)
    {
        sprintf(alertstr,"allocation of more comment space failed");  /*950110*/
        sprintf(alertstr2,"expect truncated words");
#ifdef MACINTOSH
        sprintf(alertstr3,"Perhaps preferred size too small: Before launching MAGE:"
                          CRLF"one click on MAGE, choose Info under File Menu: "
                          CRLF"change preferred size to, e.g., 2000 ");
#else
        sprintf(alertstr3," ");
#endif
        DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
        /*cleanup();*/
        /*exit(EXIT_FAILURE);*/
    }
}
/*___allocwordcomarray()____________________________________________________*/

/****alloccomarray()*****************************************************/
void    alloccomarray(int more)
{
/* char  *com;  pointer to array, for com[MAXCOM], size=MAXCOM*/
/*long  MAXCOM*/  /*970410*/
/* the comment or wordlist character array  */

    int  ifail, recycle, moretry;
    char *comtemp;

    /*more==0 for first call to allocate initial arrays*/
    moretry = more;
    recycle = 1;
    while(recycle==1)
    {/*recycle allocation trials*/
        ifail = 0;
            if(more==0)
            {
                com = (char *)malloc(sizeof(char)*MAXCOM);
                if(com==NULL) ifail = 1;
                com[0] = '\0'; /*place for zero length strings*/
            }
            else
            {
            comtemp=(char *)realloc(com,sizeof(char)*(MAXCOM+more));
            if(comtemp==NULL) ifail = 1;
            }
        if(more>0 && ifail==1)
        {/*failed to do a reallocation, reduce request and try again*/
            more = more/2;
            if(more > 3) recycle = 1;
            else    recycle = 0;
        }
        else recycle = 0;
    }/*recycle allocation trials*/

    if(more>0 && ifail==0)  com = comtemp; /*reassign pointer*/
    if(more>0 && ifail==0) MAXCOM = MAXCOM + more; /*increase limit*/
    if( (Ltest||ifail) && !Limitmaxcom) /*970410*/
    {
        if(more>0 && ifail>0)
            sprintf(alertstr,"+%d reallocation failed, MAXCOM remains: %ld"
            ,moretry,MAXCOM);
        else
            sprintf(alertstr,"MAXCOM == %ld",MAXCOM);
        sprintf(alertstr2,"alloc: com==%p",com ); /*121108 char* , not long*/
        sprintf(alertstr3," "); 
        DoMageDLOGreport(2); /*___DLOG.C*/  /*alertstr,alertstr2,alertstr3*/
        Limitmaxcom = 1; /*970410*/
            /*reset to 0 in MAGEINPT so each kinemage reports once*/
    }
    if(ifail&&more==0)
    {/*initial failure*/
        sprintf(alertstr,"allocation of ptID space failed");  /*950110*/
        sprintf(alertstr2,"expect truncated words");
#ifdef MACINTOSH
        sprintf(alertstr3
                     ,"Perhaps preferred size too small: Before launching MAGE:"
                          CRLF"one click on MAGE, choose Info under File Menu: "
                          CRLF"change preferred size to, e.g., 2000 ");
#else
        sprintf(alertstr3," ");
#endif
        DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
        /*cleanup();*/
        /*exit(EXIT_FAILURE);*/
    }/*initial failure*/
    if(ifail==0 && more==0)
    {/*initial success*/
        com[0] = '\0'; /*use this as dummy if later run out of room*/
                       /*new ptIDs start at com[1]...*/
    }/*initial success*/
}
/*___alloccomarray()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****alloclinkarrays()*******************************************************/
void    alloclinkarrays(void)
{
/*these arrays come in sets that have to be of the same size since they*/
/* are indexed the same way, and subject to random access */

    int  j, ifail;

        ifail = 0;

        for(j = 0; j <= 1 ; j++)
        {
                thelink[j] = (long *)malloc(sizeof(long)*1000L);
                if(thelink[j]==NULL) ifail = ifail+1;
        }

    if(Ltest)
    {
        sprintf(alertstr," ");

        sprintf(alertstr2,"alloc: "
         CRLF"thelink[0]==%ld,thelink[1]==%ld"
         ,(long)thelink[0],(long)thelink[1]);
        sprintf(alertstr3," ");
        DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
    }
    if(ifail)
    {
        sprintf(alertstr,"allocation of link list space failed");
        sprintf(alertstr2," ");
        sprintf(alertstr3," ");
        DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
        /*cleanup();*/
        /*exit(EXIT_FAILURE);*/
    }
}
/*___alloclinkarrays()_______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****cleanup()***************************************************************/
void cleanup(void)  
{
    int i;
    
    if(Ltablepresent > 0) cleanuptable(); /*old table to be destroyed*/
    
    for (i = 0; i < 7; ++i)
    {
        if (it[i] != NULL) free(it[i]);
    }
    if (com != NULL) free(com);
    if (wordcom != NULL) free(wordcom);
    if (itpointptr != NULL) free(itpointptr);
    if (linkit != NULL) free(linkit);
    if (thelink[0] != NULL) free(thelink[0]);
    if (thelink[1] != NULL) free(thelink[1]);

    killkinemage(); /* MAGELIST.C */

    if(nbondrot > 0)
    {/*nbondrot>0*/
       for(i=1; i<=nbondrot; i++)
       {/*loop over bondrots*/
          if(bondrotptr[i] != NULL)
          {
             if(bondrotptr[i]->level > 0 || bondrotptr[i]->level== -1)
             {/*independent rotation or first of a ganged set of rotations*/
                /*only these have sliders*/
                    
                erasebondrotvalue(i);
                /*removes any previous kinemage bond rotation text*/

#ifdef UNIX_MOTIF
                XtDestroyWidget(hgrafbondrot[i]);
                XtDestroyWidget(bondrotvalue[i]);
                /*XtDestroyWidget(bondrotstart[i]);*/ /*950617*/
                /*XtDestroyWidget(bondrotname[i]);*/ /*950617*/
#endif
                    
             }/*independent rotation or first of a ganged set of rotations*/

             destroybondrotliststructure(bondrotptr[i]);
          }

          bondrotptr[i] = NULL;
                
       }/*loop over bondrots*/
    }/*nbondrot>0*/
    if(Lpipeactive)
    {
       doupdatefrompipe(999," "); /*close pipes____PIPE.c*/  /*991216*/
       destroyallpipebuff(); /*____PIPE.c*/  /*991216*/
    }
}
/*___cleanup()______________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****writechangestostdout()***************************************************/
void writechangestostdout() /*031011*/
{
   int more = 1, j=0, Nth=0; 

         while(more)
         {/*loop over all text for this call*/
            for(j=0; j<255; j++) /*save a space for safety '\0' */
            {/*load transfer string*/
               temps[j] =  magechangesstdout[Nth][j];
               if(temps[j] == '\r')
               {/*ASCII Carraige Return*/
                  temps[j] = EOL; /*platform specific End-Of-Line character*/
               }
               if(temps[j] == '\0'){ break;} /*separate end of text piece*/
            }/*load transfer string*/
            /*check for END before writing to stdout*/
            if(temps[0] =='E' && temps[1] =='N' && temps[2] =='D'){more = 0;}
            temps[j+1] = '\0'; /*safety if runs off end of temps str*/
            fprintf(stdout,"%s",temps);
            Nth++; /*local, not static, so will get reset ==0 on new call*/
         }/*loop over all text for this call*/
}
/*___writechangestostdout()__________________________________________________*/

/****writereporttostderr()****************************************************/
void writereporttostderr() /*Lreportstderr flag MUXMMAIN/main()    070901*/
{
   ; /*dummied 071005*/
}
/*___writereporttostderr()____________________________________________________*/
