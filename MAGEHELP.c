/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*   MAGEHELP.C   */
#include "MAGE.h"
#include "MAGEMENU.h"

 static char*  magehelpstdout[] = 
{"\r"
,"COMMANDLINE, NO GUI, postscript output of each animation view:\r"
,"mage filename.kin -postscript\r"
,"produces: filename.kin.1.eps , filename.kin.2.eps , ... for n animate views\r"
,"\r"
,"COMMANDLINE, NO GUI, meager help to standard out:\r"
,"mage -help\r"
,"\r"
,"COMMANDLINE, NO GUI, chronological list of changes to standard out:\r"
,"mage -changes\r"
,"\r"
,"for other help, help menu: use write-to-text-window, or write html file,\r"
,"or individual dialog-box help items on the help menu.\r"
,"\r"
,"STUFF NOT YET in other help: mage.6.57.121211...\r"
,"screen fixed NDIM7 parameter balllist radius= 0.0051, ringlist radius= 0.75\r"
,"works well with @scale 60.0 and suitefit42NDIM7.kin with @zoom 1.0\r"
,"note: list inherits screen from group.\r"
,"\r"
,"END\r"
};

 static char*  magekeywords[] = 
{"\r"
,"keywords start with @\r"
,"alternates sometimes shown, but preferred form is listed first\r"
," @keyword {text characters} are those enclosed in {} \r"
,"See 'Properties' listing for object keyword parameters and point attributes\r"
,"--------text section------\r"
," @  of a real keyword must be first character of a new line\r"
," keywords here are set in by a space so this text can be read in Mage\r"
," @text       starts text section\r"
," @mage #.##    MAGE version (can come before @text), \r"
,"          only keyword recognized WITHIN text section\r"
," @kinemage #   ESSENTIAL keyword to end text and start kinemage input\r"
,"--------------------------\r"
,"--------kinemage input----\r"
," # indicates an integer needed, #.## a floating point number.\r"
," @start   control for reading from a sustained active pipe: start a frame\r"
,"       recognized by pipe handler code\r"
," @frame # frame number of image from a sustained active pipe, override  \r"
," @finish  control for reading from a sustained active pipe: finish a frame\r"
," @done    control for reading from a sustained active pipe, close pipe   \r"
,"\r"
," @mage #.##\r"
," @prekin #.##\r"
," @caption   --->chars to caption until next recognized keyword---\r"
," @text      --->any text appended until next recognized keyword (5_75+)\r"
,"\r"
," ---MAGE object hierarchy: group, subgroup, list, (sets of points) points\r"
," points can be in connected line sets, or specify triangles, spheres, etc.\r"
," @group    {name} [parameters... all on one line...]\r"
," @subgroup {name} [parameters... all on one line...]     (@set)\r"
," @____list {name} [parameters... all on one line...]\r"
,"    some type of list is ESSENTIAL for a kinemage\r"
," @vectorlist   (@vector)\r"
," @labellist    (@label)\r"
," @wordlist     (@word)\r"
," @dotlist      (@dot)\r"
," @balllist     (@ball)\r"
," @spherelist   (@sphere)\r"
," @trianglelist (@triangle)\r"
," @ribbonlist   (@ribbon)\r"
," @arrowlist    (@arrow)\r"
," @marklist     (@mark)\r"
," @ringlist     (@ring)\r"
," @fanlist      (@fan)\r"
,"\r"
,"---MAGE  points---follow @____list line on the next line \r"
,"{pointID} [attributes... separated by spaces, commas, new-lines, etc.] x y z\r"
,"--trailing triple: x,y,z defines scope of a point, x,y,z only required thing\r"
,"----table cells only: 'tablecell' or 'noxyz' can stand for triple\r"
,"Generally, list type determines meaning of all its points,\r"
,"  but point attributes can force different meanings within a list\r"
,"  for advanced, special purpose, complicated kinemages.\r"
,"\r"
," @kinemage #     ends this kinemage input\r"
," @noscale\r"
," @scale #.##\r"
," @compare    side-by-side of sequential animate groups, overrides stereo\r"
,"             stereo is reader's choice: menu and keyboard s key\r"
," @stereoangle #.## + for wall-eye, - for cross-eye, also under menu control\r"
,"             + vs - toggled by keyboard c key\r"
," @onewidth      default is multiwidth, also menu control\r"
," @thinline\r"
," @perspective\r"
," @plotonly\r"
," @flat       @xytranslation\r"
," @pickcenter\r"
," @zclipoff\r"
," @whitebackground   (@whiteback)  (@whitebkg)\r"
," @viewid {string}   @1viewid {string}\r"
," @2viewid {string}\r"
," ...\r"
," @##viewid {string}\r"
," @zoom #.##        @1zoom #.##\r"
," @2zoom #.##\r"
," ...\r"
," @##zoom #.##\r"
," @span #.##        @1span #.##\r"
," @2span #.##\r"
," ...\r"
," @##span #.##\r"
," @center #.## #.## #.##    @1center #.## #.## #.##\r"
," @2center #.## #.## #.##\r"
," ...\r"
," @##center #.## #.## #.##\r"
," @matrix   #.## #.## #.##  #.## #.## #.##  #.## #.## #.##   @1matrix ... \r"
," @2matrix  #.## #.## #.##  #.## #.## #.##  #.## #.## #.##\r"
," ...\r"
," @##matrix #.## #.## #.##  #.## #.## #.##  #.## #.## #.##\r"
," @zslab   #        @1zslab  #    @zclip #    @1zclip #\r"
," @2zslab  #        @2zclip  #\r"
," ...                     ...\r"
," @##zslab #        @##zclip #\r"
," @ztran   #        @1ztran #\r"
," @2ztran  # \r"
," ...\r"
," @##ztran #\r"
," @size # #        @1size # #   : graphics width, height in pixels\r"
," @2size # #\r"
," ...\r"
," @##size # #\r"
,"\r"
," @localrotation #.## #.## #.##  #.## #.## #.##  #.## #.## #.## \r"
,"   a.k.a: @localrotat ...  @localmatrix ...\r"
," @endlocalrotation  @endlocalrot @endlocalmatrix\r"
," @localprecenter  #.## #.## #.##    @localcenter ...  applied before matrix\r"
," @localpostcenter #.## #.## #.##                      applied after  matrix\r"
," @endlocalcenter    @endlocalcen @endlocalprecen @endlocalpostcen\r"
,"\r"
," @gnomon #.## #.## #.##  #.## #.## #.##  #.## #.## #.##  #.## #.## #.##\r"
,"   place after the @group... line: to belong to that group\r"
,"   points on 3 axes and center (ok if each on separate line)\r"
,"   used by Docking routines to track change to a mobile group\r"
,"   MAGE will add to existing gnomon info or create anew if needed\r"
,"\r" 
," @fontsizeinfo   #   @fontsizeinfo  # \r"
," @fontsizelabels #   @fontsizelabel # \r"
," @fontsizewords  #   @fontsizeword  # \r"
," @tablefontsize  #   @fontsizetable # \r"
," @tableblackbkg   black background (and blk bkg palette) for table\r"
," @tablewrap #       number of columns across before wrap line below\r"
," @tablewrap auto-wrap first implemented in Java Mage\r"
," @tablewrapdecades  auto-wrap # columns rounded down to 10's\r"
," @tablecolscale   #.##   scale calculated column width\r"
," @tablemarkradius #.## square marker radius for table selected graphics points\r"
," @tablefancore    #.##    inner radius of table value fan on graphics point\r"
," @tablefanradius  #.##  outer radius of table fan at a graphics point,\r"
,"   this is multiplied by point radius to represent the cell value\r"
,"\r"
," @beginselect    keyword starts selection range, re: bondrot selection\r"
," @endselect      keyword ends selection range, re: bondrot selection\r"
,"\r"
,"(@beginemphasis prototype use only: starts selected range of emphasis)\r"
,"(@endemphasis   prototype use only: ends selected range of emphasis)\r"
,"\r"
," @listcolordominant  @listcolordom usually point color is dominant: key 'l'\r"
," @lens #.##     parameter of lens radius for lists marked \"lens\": key 'e'\r"
,"\r"
," @minutes #.##  timed test of #.## minutes, results automatically written.\r"
," @drawline   enables draw new function only for lines between points\r"
," @drawlabel  enables draw new function only for labels (=pointID) at points\r"
," @drawball   enables draw new function only for balls at points\r"
," (lines, labels, balls are inclusive! See list params answer & result:   )\r"
," (scoring done only on answerlist type: limited to one type as of Sept 02)\r"
," @drawnew    enables all draw new functions, useful for practice, etc. \r"
," (answer list without @minutes allows user to see scored result)\r"
," KINEMAGE menu items enabled to allow users to score and proceed in tests\r"
," @nosavealert NOT warn user about new drawings before doing next kinemage\r"
," @bigfonts  sets toggle (w key) ON\r"
," @shortline #.##    absolute amount to shorten each end of a new draw line\r"
,"\r"
," @comment {xxx yy z} character string just transfered unused to output \r"
," @command {xxx yy z} character string to be used for remote update cmd \r"
," @control  creates a control button Lcontrol, which toggles Lcontrolon \r"
," @title {arbitrary kinemage title} \r"
," @copyright {date and author(s) } \r"
," @mapfile {filename.xmap} optional actual name to use for kin3Dcont\r"
,"   must be before any @group... line to apply to whole kinemage\r"
," @pdbfile {filename.pdb} optional actual name to use for prekin or probe\r"
,"   must be before any @group... line to apply to whole kinemage\r"
," @grouppdbfile {filename.pdb} specific group pdb to use for prekin or probe\r"
,"  must follow @group... line to apply to that group\r"
,"\r"
,"  --- masters ---\r"
," @colorset {name} color   unique name and standard color as char strings\r"
," usage:  @____list {listname} color= {name}   ... anywhere in kinemage\r"
,"\r"
," @master {name} *  forces named master buttons in inputed order\r"
,"  where * is one or more attributes optionally following {name}\r"
,"  \"indent\" \"space\" \"on\" \"off\"  on off   particular for pointmaster\r"
," @pointmaster 'C' {name}  character 'C' associated with master= {name}\r"
,"  usage:  {ptID} 'C' #.## #.## #.##  \r"
,"\r"
,"  --- aspects ---\r"
," @1aspect {name-of-1st}  (necessary to define for point aspect colors)\r"
," @2aspect {name-of-2nd}\r"
," ...\r"
," @26aspect {name-of-26th}\r"
,"   points can have up to 16 color-codes in parentheses (ABC...)\r"
,"   positions invoked as the order of the numbers # of  @#aspect {name}  \r"
,"   but each position can have any one of the 26 capital letter color codes\r"
,"   codes in MAGE-HELP/Make kinemage: Internally generated palette kinemage.\r"
",   Use \"]\" and \"[\" keys to cycle through aspects.\r"
,"\r"
," @nowfind {string} invokes FIND, input on the fly from an appended file\r"
," e.g.append 3 lines to search for string: @start @nowfind {string} @finish \r"
," as outputed by File/Save Special/pointID to file\r"
,"\r"
," @parameter #.##  get up to 10 parameters to play with, e.g. diffractometer\r"
,"\r"
,"@fullrgbpalette\r"
,"{  #}   #,   #,   #, {comment}  matches: File/Save Special/palette to file\r"
,"...\r"
," first # is 0-255 index of entry, then R,G,B 0-255, (comment ignored)\r"
," arbitrary number of entries, 251-254 ignored, entry color name NOT changed\r"
,"\r"
,"-----obselete or violating reader's prerogatives----------\r"
," (@float   #.##)\r"
," (@integer #)\r"
," (@ignore)   allowed kinemage file to also be a, e.g., PS300 CHAOS display file \r"
," (@keepstereo)   valid reader's choice but over-ridden by author's compare \r"
," (@keepthinline) artifact as author's choice, reader can use keyboard 't'\r"
," (@keepperspective)  (@keepperspec) artifact  as author's choice, keyboard 'p'\r"
," (@specialps)    obselete: PostScript output now on menu\r"
," (@projection)   construct line length * 1/cos(angle): now menu choice\r"
," (@constructdot) construct line puts dot instead of a line: now menu choice\r"
," (@multibin (#)) obselete, no effect\r"
," (@zoomonly)     obselete, no effect\r"
," (@sideclip)     obselete, no effect\r"
,"\r"
,"END\r"
};
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MAGESETS.c*/
/*interpretgroupkeyword(),interpretsubgroupkeyword(),interpretlistkeyword()*/
 static char*  mageobjectparameters[] = 
{"\r"
,"--------mage group parameters------\r"
," @group {name} [param...]\r"
,"animate  (*prefix)\r"
,"2animate (%prefix)\r"
,"off\r"
,"dominant (no buttons for member subgroups and lists)\r"
,"nobutton (for this group)\r"
,"recessiveon  (dominant if off, recessive if on)\r"
,"collapsable collapsible  synonyms for recessiveon\r"
,"master= {name} (common master name) (see NOTES)\r"
,"instance= {name} (name of a proceeding group) (see NOTES)\r"
,"clone= {name} (name of a proceeding group) (see NOTES)\r"
,"moview # (number of a Menu VIEW)\r"
,"lens  (applied to member points)\r"
,"float (obselete now always do floating point perspective calc:\r"
,"  --- parameters for table groups ---\r"
,"table  (members put into table, invokes Mage to make a table window)\r"
,"header (members are column headers of table: use labellist or wordlist)\r"
," (each table row is a group, uses labellist,wordlist,fanlist)\r"
," (point coords meaningless in a table: use \"tablecell\" for \"x,y,z\")\r"
," table pick picks graphics point with same pointID\r"
,"\r"
,"   SHOW any 3 of N-D points, select clusters and rearrange them. \r"
,"dimension=N groups show any 3 of N dimensions,  up to 17 as of June06\r"
,"all points of lists given or inheriting dimension \r"
,"   must have same number of coordinates\r"
,"d1={name1} ignored after June06 (axis names for the dimensions) \r"
," use general keyword @dimensions {name1} {name2} ... up to N \r"
,"select     (points in this group can be a color-selected set)\r"
,"wrap=360   (negative coords have 360 added to them)\r"
,"\r"
,"--------mage subgroup parameters------\r"
," @subgroup {name} [param...]\r"
,"off\r"
,"dominant (no buttons for member lists)\r"
,"nobutton (for this subgroup)\r"
,"recessiveon  (dominant if off, recessive if on)\r"
,"collapsable collapsible  synonyms for recessiveon\r"
,"master= {name} (common master name) (see NOTES)\r"
,"instance= {name} (name of a proceeding subgroup) (see NOTES)\r"
,"clone= {name} (name of a proceeding subgroup) (see NOTES)\r"
,"lens  (applied to member points)\r"
,"\r"
,"   SHOW any 3 of N-D points, select clusters and rearrange them. \r"
,"dimension=N subgroups show any 3 of N dimensions,  up to 17 as of June06\r"
,"all points of lists given or inheriting dimension \r"
,"   must have same number of coordinates\r"
,"for names: use general keyword @dimensions {name1} {name2} ... up to N \r"
,"select     (points in this subgroup can be a color-selected set)\r"
,"wrap=360   (negative coords have 360 added to them)\r"
,"\r"
,"--------mage list types------\r"
,"vectorlist   (Point x,y,z -->Line x,y,z -->Line x,y,z ...)\r"
,"labellist    (pointID displayed at x,y,z)\r"
,"wordlist     (<comment> ,inc. new lines, displayed at x,y,z)\r"
,"dotlist      (line width # pixels at x,y,z\r"
,"balllist(    (disk [highlight] [shortened lines], center at x,y,z)\r"
,"spherelist   (stacked disks [highlight] [shortened lines] at x,y,z)\r"
,"trianglelist (filled-in triangle)\r"
,"ribbonlist   (sets of 2 filled-in triangles have same normal)\r"
,"arrowlist    (Tail point x,y,z -->Head x,y,z  with 4 tines ...)\r"
,"marklist     (screen-oriented squares around x,y,z)\r"
,"ringlist     (screen-oriented annulus around x,y,z)\r"
,"fanlist (table member: screen-oriented, weighted feathers)\r"
," (weighted feathers radiate from x,y,z of asociated graphics point)\r"
," (@tablefancore   #.##  inner radius of table value fan on graphics point\r"
," (@tablefanradius #.##  outer radius of table fan at a graphics point,\r"
,"\r"
,"--------mage list parameters------\r"
," @_____list {name} [param...]\r"
,"off\r"
,"nobutton (for this list)\r"
,"\r"
,"answer (list used as answer key with @minutes timed test)\r"
," (answer dominant nobutton off (hide answer key from user))\r"
," (answer must have point types of lines, labels, balls as test asks)\r"
," (answer list can be made with drawnew function in Mage.)\r"
,"    (see @minutes, @drawline, @drawlabel, @drawball ... )\r"
,"result  (list from drawnew of an earlier mage run to be scored vs. answer)\r"
,"\r"
,"color= name  colour= name (mage palette named colors)\r"
,"master= {name} (common master name) (see NOTES)\r"
,"instance= {name} (name of a proceeding list) (see NOTES)\r"
,"clone= {name} (name of a proceeding list) (see NOTES)\r"
,"lens  (applied to member points)\r"
,"detail\r"
,"radius= #  (ball, sphere, mark, ring, arrow-head vane length)\r"
,"angle= # (arrow-head tine angle)\r"
,"width= # (pixel width of lines)\r"
,"static  (dynamic rotations not applied to members of this list)\r"
,"nozclip (applied to points in this list)\r"
,"nohi nohighlight\r"
,"face (for characters in wordlist)\r"
,"font (for characters in wordlist)\r"
,"size (for characters in wordlist)\r"
,"\r"
,"   SHOW any 3 of N-D points, select clusters and rearrange them. \r"
,"dimension=N lists show any 3 of N dimensions,  up to 17 as of June06\r"
,"all points of lists given or inheriting dimension \r"
,"   must have same number of coordinates\r"
,"for names: use general keyword @dimensions {name1} {name2} ... up to N \r"
,"select     (points in this list can be a color-selected set)\r"
,"wrap=360   (negative coords have 360 added to them)\r"
,"\r"
,"--------list parameters for bond rotations--------\r"
,"see @beginselect and @endselect for artificial scope\r"
,"  otherwise scope of bondrot is from first point of list to last point\r"
,"  before bondrot of equal or lower number, or end of file  \r"
," Starting angle is arbitrary as far as MAGE is concerned,\r"
," should be actual dihedral\r"
," Ganged rotations distinguished by listname\r"
,"(phirotation) obselete March 2002    (flags a ganged rotation)\r"
,"bondrot   #.#  (starting angle, one of ganged rotations)\r"
,"0bondrot  #.#  (starting angle, trunk) \r"
,"1bondrot  #.#  (starting angle, first branch)\r"
,"2bondrot  #.#  (starting angle, later branch)\r"
,"3bondrot  #.#  (starting angle, ...)\r"
,"4bondrot  #.#  (starting angle)\r"
,"5bondrot  #.#  (starting angle)\r"
,"6bondrot  #.#  (starting angle)\r"
,"7bondrot  #.#  (starting angle)\r"
,"8bondrot  #.#  (starting angle)\r"
,"9bondrot  #.#  (starting angle)\r"
,"  March 2002: recognize up to 20bondrot levels, accept up to 100 bondrots\r"
,"  (99bondrot prototype for monotonically continuing branch from last n)\r"
,"parameter    (Lbrotoption 1)\r"
,"precession   (Lbrotoption 2)\r"
,"selection    (Lbrotoption 3)\r"
,"xrotation    (Lbrotoption 4)\r"
,"yrotation    (Lbrotoption 5)\r"
,"zrotation    (Lbrotoption 6)\r"
,"xtranslation (Lbrotoption 7)\r"
,"ytranslation (Lbrotoption 8)\r"
,"ztranslation (Lbrotoption 9)\r"
,"samescope2   (2 bondrots have the same scope as first of 2)\r"
,"samescopt3   (3 bondrots have the same scope as first of 3)\r"
,"samescope4   (4 bondrots have the same scope as first of 4)\r"
,"hplot      (bondrot value plotted horizontally)\r"
,"vplot      (bondrot value plotted vertically)\r"
,"zplot      (bondrot value plotted in&out, perpendicular to screen)\r"
,"pattern  c... (MAXBONDPATT==37 characters== 0...A or R,Y,G on UNIX slider)\r"
,"phirotation  (Lbrotoption 10  ganged rotation of same-named bondrot lists)\r"
,"\r"
,"--------mage point attributes------\r"
,"point attributes are usually separated by white space\r"
,"as a matter of style and ease of editing, do separate all by white space\r"
,"but sometimes worth maximal compression to use storage, transmission, etc.\r"
,"thus whitespace as separator and as field delineator a bit tricky.\r"
,"----------points---follow @____list line on the next line: \r"
,"{pointID} [attributes...separated by spaces, commas, new-lines, etc.] x y z\r"
,"{   final } does not require white space before next attribute.\r"
,"--trailing triple: x,y,z defines scope of a point, x,y,z only required item\r"
,"x y z fields delineated by non-numeric whitespace   blank , ; : cr lf tab \r"
,"----table cells only: 'tablecell' or 'noxyz' can stand for triple\r"
,"<comment>   saved and outputted for all point types\r"
,"  written to screen for wordlists, written in cell in table groups\r"
,"  if present, written in lower left screen when label is picked\r"
,"--------single character flags, all except P need trailing white space\r"
," P defines new beginning of Point-->Line-->Line-->...polyline in vectorlist\r"
," P synonyms are p M m     i.e. in Move-->Draw, exception: P ends itself\r"
,"   presumed for first point of a vectorlist.  L for 'Line' not essential.\r"
," L synonyms are l D d \r"
," U (or u) flags point as unpickable, see menu superpick item\r"
," B (or b) ball at this point in a vectorlist\r"
," S (or s) sphere at this point in a vectorlist\r"
," R (or r) screen oriented ring at this point in a vectorlist\r"
," Q (or q) screen oriented square at this point in a vectorlist\r"
," A (or a) arrow on this point of a vectorlist\r"
," T (or t) point is interpreted as part of a triangle in a vectorlist\r"
," X (or x) point is new start of an independent triangle in a trianglelist\r"
,"\r"
," 'C' single quotes around character(s) identifying pointmaster(s),\r"
,"    case sensitive, declare before use earlier in kinemage\r"
,"    to avoid single char master name or master-name conflict:\r"
,"     @pointmaster 'C' {name}   character 'C' associated with master= {name}\r"
,"\r"
,"--------multiple character attributes------- need surrounding white space\r"
," colorname   any of mage palette color names;\r"
," colorname! (exclaimation point) forces use of point color.\r"
,"\r"
," width1  (or thin) for forced one pixel wide lines\r"
," width2  (width#, where #= 1...7)  forced # pixel wide lines\r"
,"  Effective pen width = line width specified by point width or list width\r"
,"  where point width is dominant over list width setting.\r"
,"  When not specified, multi-width mode uses pen width for depth cueing\r"
,"  but some display objects (rings and marks) default to a const.\r"
,"\r"
," r= f    point radius for points that are balls, spheres, rings, etc,\r"
,"   also used for arrow vane length (vane angle is list angle).\r"
,"\r"
," dummy (or DUMMY)  point is NOT used for setting min,max x,y,z extents\r"
," tablecell (or  noxyz  or  NOXYZ)  substitutes for x, y, z triple \r"
," ghost (or GHOST) point is seen but not in special output (like PDB) \r"
,"\r"
,"NOTES:\r"
,"   master:\r"
,"The same master can refer to groups,subgroups,lists, each of which can\r"
,"have multiple masters, but only 32 unique masters total.\r"
,"Multiple masters are restrictive: all have to be ON for object to be ON\r"
,"   instance:\r"
,"Object merely redisplayed at original coordinates, color can be different\r"
,"but inherits masters, etc.\r"
,"   clone:\r"
,"Object actually copied, can have changed coords, etc. new object in output.\r"
,"\r"
,"mage commandline options:\r"
,"COMMANDLINE, NO GUI, postscript output of each animation view:\r"
,"mage filename.kin -postscript\r"
,"produces: filename.kin.1.eps , filename.kin.2.eps , ... for n animate views\r"
,"\r"
,"COMMANDLINE, NO GUI, meager help to standard out:\r"
,"mage -help\r"
,"\r"
,"COMMANDLINE, NO GUI, chronological list of changes to standard out:\r"
,"mage -changes\r"
,"\r"
,"COMMANDLINE options along with GUI:\r"
,"-fontinfo   dumps font possibilities to UNIX console\r"
,"-medium   forces use of meduim  font for menus, dialogs, text: OK 1024x768\r"
,"-large    forces use of largest font: TOO BIG FOR <= 1024x768 SCREEN!\r"
,"-macular == -jeremy   chooses medium or large font based on screen size\r"
,"-small    forces use of small font, the old default (before 031127)\r"
,"\r"
,"\r"
,"Capture the foo (besides game mode)\r"
,"menu:Tools/Kludges/foo in cavity\r"
,"Pick point for initial foo, grow in cavity, surface the foo.\r"
,"Foo will coat a surface, and not puff out into the void.\r"
,"Note: use Show Object Properties: group ignorable (by the foo).\r"
,"For proper VdW size use: prekin -atomradii ...\r"
,"\r"
,"END\r"
};

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 static char*  magehypertext[] = 
{"\r"
,"--------mage hypertext help------\r"
,"    The hypertext capabilities of MAGE text has two interrelated functions.\r"
,"    One is for a system of questions presented to the reader that are each \r"
,"answered in a dialog box that gives the reader access to information from \r"
,"the most recently picked points.\r"
,"    The other is facility to instruct MAGE to move around in the kinemage \r"
,"by jumps to specific kinemages and views, find pointID text strings, turn \r"
,"on and off masters, and reset bond rotations.  Of course, these moves can \r" 
,"be part of a scheme of questions.  However, they confer an enormous power \r"
,"to the reader, particularly because the text, including the hypertext, can \r"
,"be edited during the session!  \r"
#ifndef OLD68KMAC /*defined in MAGE.h*/
,"    For Questions, kinemage number and View number are carried forward from \r"
,"the last time they were set during the linear input of the TEXT window \r"  
,"material.  Thus kinemage and view can be presumed for the go-to-graphics \r"
,"option of a new question.  However, if these are to re-defined for the \r"
,"current question, this must be done before any master-button or FIND \r"
,"controls are specified.  This dependence on the order of controls is \r"
,"NOT hypertext-like and might frustrate the reader/lecturer if they try to \r"
,"jump around in the text.  The author must balance readability and flow \r"
,"of the text with robustness. \r"
,"\r"
,"Format: starts with '*{'  includes [options]   ends with '}*' \r"
,"Questions start with Q: but Mage keeps track of the question number. \r"
,"*{Q: text of question... }* \r"
,"Thus, if the second question in the kinemage is:\r"
,"*{Q: Which is the active Ser?}* \r"
,"Mage shows this in the text, with room for the answer in the underlined space\r"
,"*{Q: 2  Which is the active Ser?}*___________________________ \r"
,"\r"
,"Change instructions to Mage include:\r"
,"*{kinemage #, view #, alloff, master={name} on, find {string} }* \r"
,"abreviations: kinemage kin k, view v, master m, find f\r"
,"'alloff' turns all masters off, masters are individually set 'on' or 'off' \r"
,"\r"
,"Bond rotations can be set (and edited and reset!):\r"
,"*{rot={name 1} #.#, rot={name 2} #.# ...}*  abr: 'rot=','rot ','r ' \r"
,"where 'name 1' == the bondrot identifier string seen on the slider bar\r"
,"and #.# is the new value for that rotation angle\r"
,"If Update by PROBE active, hypertext 'rot' triggers an update.\r"
,"PREKIN after version 5.74 makes hypertext commands\r"
," for rot/mutated residues\r"
,"\r"
#else
,"--old 68K Mac has limited text here--\r"
#endif
,"END\r"
};

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 static char*  magedocking[] = 
{"\r"
,"---mage docking help------Visual based docking and superposition----\r"
,"Tools menu item: 'Docking Scope' sets what ever groups currently 'ON'\r"
,"to be mobile (they must be contiguous in the kinemage data structure) while \r"
,"all other groups become the stabile, reference set.\r"
,"(One can still set up by hand appropriate bond rotation/translation commands\r"
," in the kinemage file itself as once was the only way to do docking.)\r"
,"A set of sliders appears in the far right panel which move the \r"
,"mobile set relative to the reference set.\r"
,"Less precise but more facile: holding down the shift key will restrict \r"
,"drag motions to move only the mobile group.\r"
,"Holding down the ctrl (PC:alt) key does xy scrolling instead of rotations\r"
,"thus, holding down both shift and ctrl keys scrolls the mobile group(s)\r"
,"relative to the reference set.\r"
,"The saved kinemage has the modified coordinates of the mobile set.\r"
,"The slider values are not updated by the mouse docking operations and\r"
,"resetting the sliders does NOT reset the docking changes.   However, MAGE\r"
,"saves the relative changes with each mobile group and outputs\r"
,"a keyword entry that can be used to externally reproduce the docking.\r"
," @gnomon #.## #.## #.##  #.## #.## #.##  #.## #.## #.##  #.## #.## #.##\r"
,"   place after the @group... line: to belong to that group\r"
,"   points on 3 axes and center (ok if each on separate line)\r"
,"   used by Docking routines to track change to a mobile group\r"
,"   MAGE will add to existing gnomon info or create anew if needed\r"
,"(Since a kinemage can be made from either a subset or superset of the \r"
,"information in a PDB coordinate file, there is no general translation from\r"
,"a modified kinemage to a modified PDB file.)\r"
,"\r"
#ifndef OLD68KMAC /*defined in MAGE.h*/
,"   Three point docking onto 3 selected points in the reference group(s)\r"
,"of another 3 points selected in the mobile group(s), is accessed through\r"
,"[Edit/Draw New] which puts up a dock3on3 button in the side panel.\r"
,"Activating dock3on3 empowers the next 3 picked points to be references\r"
,"for the final 3 (6 total) to be moved.  The scheme is the 1st mobile point\r"
,"of a set of 3 is moved onto the first point in the reference set, the 2nd\r"
,"is put onto the line defined by reference points 1 and 2, and the \r"
,"3rd point is swung around to point toward reference point 3.\r"
,"The entire set of points in the mobile groups are moved by these rules.\r"
,"   Mage is oblivious to how these points are actually picked, so they can\r"
,"be picked on anything in the display, and the docking will be done as if \r"
,"they were real points in the reference and mobile groups.\r"
,"\r"
,"Small PROBE contact analysis update requires inventing the appropriate\r"
," command line, also requires the pointIDs of the mobile groups to contain \r"
," PDB information needed by PROBE.  Mage takes the first 15 characters\r"
," of each point's pointID and invents a PDB ATOM record with x,y,z,occ,B\r"
," The essential parts of that 15 character segment are:\r"
," 4 character atom name in positions 0,1,2,3\r"
," 3 char residue name positions 5,6,7\r"
," up to 4 char right-justified integer residue number in 10,11,12,13\r"
," position 14 is a blank.  Any further characters of pointID are ignored.\r"
,"\r"
," Generic command for internal contacts of a docking scope group is )\r"
,"probe -quiet -self all -\r"
," Command for interactions between atoms of that scope and other things\r"
," is not generated automatically, see probe -h or a commandline generated\r"
," for a bond rotation situation.\r"
,"\r"
#else
,"--old 68K Mac has limited text here--\r"
#endif
,"END\r"
};

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 static char*  mageremote[] = 
{"\r"
,"UNIX: two instances of mage can communicate through sockets, either across \r"
,"the web by URL addresses or between two Mages running on the same computer.\r"
#ifndef OLD68KMAC /*defined in MAGE.h*/
,"There are two modes: \r"
,"Colaboratory Mode where identical kinemages are running on remote computers\r"
,"and viewing operations, picks, zooms, views, etc. are sent between so that\r"
,"the two identical kinemages are displayed identically.  Each mage transmits\r"
,"to the other, and a ghost of the others cursor is also displayed.\r"
,"\r"
,"Correlation Mode: where different kinemages showing the same data are shown\r"
,"on two instances of Mage, most likely on the same computer.  Kinemages\r"
,"showing different mappings of the data are correlated by identical pointIDs\r"
,"on corresponding data points.  The view points are in general different\r"
,"and the only information transmitted between the mage instances is the\r"
,"pointID that triggers a search for that exact set of characters in the\r"
,"receiving mage.  \r"
,"     This mode was developed to correlate RNA and DNA backbone\r"
,"angle data plotted according to different combinations of the 6 backbone\r"
,"dihedral angles in sets of 3.  An additional feature sends the Up-Arrow\r"
,"trigger and that mage's current 3 angles to the remote mage to trigger its\r"
,"own simulated Up-Arrow translation of the current mutually picked point's\r"
,"angles to be set in a real-space 3D model with rotatable dihedrals.\r"
,"(Note that the receiving mage gets all 6 angles, 3 from its own data point \r"
,"and 3 from the sending mage's data point.  The sending mage only can set\r"
,"its own 3 angles in its model.  So to get both models into identical\r"
,"conformations, one needs to hit the Up-Arrow in the second mage and send\r"
,"back information to the first mage.)\r"
,"This only works if the real coordinate 3D models are identical kinemages\r"
,"in so far as the order of bond rotations, they will differ in which angles\r"
,"are designated as hplot, vplot, zplot.\r"
,"\r"
,"     Two markers, the usual Mage default, confuse which point in one\r"
,"Mage matches the point in the other (despite that the 1st and 2nd markers\r"
,"are slightly differently shaped objects).  There is a Single_Marker mode\r"
,"selectable under the Tools Menu that shows only the first marker.\r"
,"\r"
,"Setup for two communicating instances of Mage: load the kinemages, then\r"
,"Invoke two times in each Mage: Tools/Remote Update... Dialog Box:\r"
,"First: choose: \"initiate a socket server to receive from any remote mage\"\r"
,"if running both on the same computer: one gets to be 1st, the other, 2nd\r"
,"also can choose \"pointID search\" for correlated kinemages.\r"
,"OK that box, and set up the second mage the same way.\r"
,"Then again: Tools/Remote Update... Dialog Box: \r"
,"choose: \"initiate a socket client to send to a named remote mage\"\r"
,"This brings up a Dialog Box in which to\r"
,"\"Name a computer to which to send mage update stuff\"\r"
,"This is the URL of the computer remote to this instance of mage.\r"
,"When running both mages on the same computer, the URL will work if the \r"
,"computer is hooked to the internet, otherwise name it as \"localhost\"\r"
,"Also, when running on the same computer it works better to set the\r"
,"designated 2nd mage at each step before setting the designated 1st mage.\r"
,"(Mage reports socket port settings in the terminal where mage was invoked.)\r"
,"\r"
#else
,"--old 68K Mac has limited text here--\r"
#endif
,"END\r"
};

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 static char*  magehelp[] = 
{"\r"
,"--------mage general help------\r"
,"helpless\r"
,"END\r"
};

/*
showing the present colors.  The set of 13 colors that span the hues around a spectral color-wheel are laid out at left.  Gray is the most neutral to use along with a color scale (as, for instance, in the palette).  Colors must be called by name in the kinemage files.  Rotate to see the five levels of color depth-cuing.\r"
,"Choose the 'white backgrd' differently defined there: depth cuing is then by saturation rather than intensity; both 'white' and the default become black; and pinktint, bluetint, etc. become dark rather than pale colors.\r"
*/
/*040612 need more space between colorcode and color name for PC big font*/
/* del x of ~.5 compromise, in Group {labels}*/
 static char*  magepalette[] = 
{"@caption\r"
,"Palette test, \r"
,"@onewidth\r"
,"@viewid {magepalette}\r"
,"@zoom 1.00\r"
,"@zslab 180\r"
,"@ztran 0\r"
,"@center 3.000 -3.000 0.000\r"
,"@matrix 1.00000 -0.00000  0.00000  0.00000  0.99920 -0.03999  0.00000  0.03999  0.99920\r"
,"@zclip 180\r"
,"@group {colors}\r"
,"@vectorlist {deadblack} color=black\r"
,"{blk1}P -8 -16 10 {blk2} 2 -16 10\r"
,"{blk1}P -8 -16 5 {blk2} 2 -16 5\r"
,"{blk1}P -8 -16 0 {blk2} 2 -16 0\r"
,"{blk1}P -8 -16 -5 {blk2} 2 -16 -5\r"
,"{blk1}P -8 -16 -10 {blk2} 2 -16 -10\r"
,"@vectorlist {red} color=red \r"
,"{r1}P -8 -14 10 {r2} 2 -14 10 \r"
,"{r1}P -8 -14 5 {r2} 2 -14 5 \r"
,"{r1}P -8 -14 0 {r2} 2 -14 0 \r"
,"{r1}P -8 -14 -5 {r2} 2 -14 -5 \r"
,"{r1}P -8 -14 -10 {r2} 2 -14 -10 \r"
,"@vectorlist {orange} color=orange\r"
,"{or1}P -8 -12 10 {or2} 2 -12 10\r"
,"{or1}P -8 -12 5 {or2} 2 -12 5\r"
,"{or1}P -8 -12 0 {or2} 2 -12 0\r"
,"{or1}P -8 -12 -5 {or2} 2 -12 -5\r"
,"{or1}P -8 -12 -10 {or2} 2 -12 -10\r"
,"@vectorlist {gold} color=gold\r"
,"{gold1}P -8 -10 10 {gold2} 2 -10 10\r"
,"{gold1}P -8 -10 5 {gold2} 2 -10 5\r"
,"{gold1}P -8 -10 0 {gold2} 2 -10 0\r"
,"{gold1}P -8 -10 -5 {gold2} 2 -10 -5\r"
,"{gold1}P -8 -10 -10 {gold2} 2 -10 -10\r"
,"@vectorlist {yellow} color=yellow\r"
,"{y1}P -8 -8 10 {y2} 2 -8 10 \r"
,"{y1}P -8 -8 5 {y2} 2 -8 5 \r"
,"{y1}P -8 -8 0 {y2} 2 -8 0 \r"
,"{y1}P -8 -8 -5 {y2} 2 -8 -5 \r"
,"{y1}P -8 -8 -10 {y2} 2 -8 -10 \r"
,"@vectorlist {lime} color=lime\r"
,"{l1}P -8 -6 10 {l2} 2 -6 10\r"
,"{l1}P -8 -6 5 {l2} 2 -6 5\r"
,"{l1}P -8 -6 0 {l2} 2 -6 0\r"
,"{l1}P -8 -6 -5 {l2} 2 -6 -5\r"
,"{l1}P -8 -6 -10 {l2} 2 -6 -10\r"
,"@vectorlist {green} color=green\r"
,"{g1}P -8 -4 10 {g2} 2 -4 10\r"
,"{g1}P -8 -4 5 {g2} 2 -4 5\r"
,"{g1}P -8 -4 0 {g2} 2 -4 0\r"
,"{g1}P -8 -4 -5 {g2} 2 -4 -5\r"
,"{g1}P -8 -4 -10 {g2} 2 -4 -10\r"
,"@vectorlist {sea} color=sea\r"
,"{sea1}P -8 -2 10 {sea2} 2 -2 10 \r"
,"{sea1}P -8 -2 5 {sea2} 2 -2 5 \r"
,"{sea1}P -8 -2 0 {sea2} 2 -2 0 \r"
,"{sea1}P -8 -2 -5 {sea2} 2 -2 -5 \r"
,"{sea1}P -8 -2 -10 {sea2} 2 -2 -10 \r"
,"@vectorlist {cyan} color=cyan\r"
,"{c1}P -8 0 10 {c2} 2  0 10 \r"
,"{c1}P -8 0 5 {c2} 2  0 5 \r"
,"{c1}P -8 0 0 {c2} 2  0 0 \r"
,"{c1}P -8 0 -5 {c2} 2  0 -5 \r"
,"{c1}P -8 0 -10 {c2} 2  0 -10 \r"
," @vectorlist {sky} color=sky\r"
,"{sky1}P -8 2 10 {sky2} 2 2 10 \r"
,"{sky1}P -8 2 5 {sky2} 2 2 5 \r"
,"{sky1}P -8 2 0 {sky2} 2 2 0 \r"
,"{sky1}P -8 2 -5 {sky2} 2 2 -5 \r"
,"{sky1}P -8 2 -10 {sky2} 2 2 -10\r" 
,"@vectorlist {blue} color=blue\r"
,"{b1}P -8 4 10 {b2} 2 4 10 \r"
,"{b1}P -8 4 5 {b2} 2 4 5 \r"
,"{b1}P -8 4 0 {b2} 2 4 0 \r"
,"{b1}P -8 4 -5 {b2} 2 4 -5 \r"
,"{b1}P -8 4 -10 {b2} 2 4 -10 \r"
,"@vectorlist {purple} color=purple\r"
,"{pur1}P -8 6 10 {pur2} 2 6 10\r"
,"{pur1}P -8 6 5 {pur2} 2 6 5\r"
,"{pur1}P -8 6 0 {pur2} 2 6 0\r"
,"{pur1}P -8 6 -5 {pur2} 2 6 -5\r"
,"{pur1}P -8 6 -10 {pur2} 2 6 -10\r"
,"@vectorlist {magenta} color= magenta\r"
,"{m1}P -8 8 10 {m2} 2 8 10\r"
,"{m1}P -8 8 5 {m2} 2 8 5\r"
,"{m1}P -8 8 0 {m2} 2 8 0\r"
,"{m1}P -8 8 -5 {m2} 2 8 -5\r"
,"{m1}P -8 8 -10 {m2} 2 8 -10\r"
,"@vectorlist {hotpink} color=hotpink\r"
,"{hotp1}P -8 10 10 {hotp2} 2 10 10\r"
,"{hotp1}P -8 10 5 {hotp2} 2 10 5\r"
,"{hotp1}P -8 10 0 {hotp2} 2 10 0\r"
,"{hotp1}P -8 10 -5 {hotp2} 2 10 -5\r"
,"{hotp1}P -8 10 -10 {hotp2} 2 10 -10\r"

,"@vectorlist {pink} color=pink\r"
,"{pk1}P 6 10 10 {pk2}16 10 10\r"
,"{pk1}P 6 10 5 {pk2}16 10 5\r"
,"{pk1}P 6 10 0 {pk2}16 10 0\r"
,"{pk1}P 6 10 -5 {pk2}16 10 -5\r"
,"{pk1}P 6 10 -10 {pk2}16 10 -10\r"
,"@vectorlist {lilac} color=lilac\r"
,"{lilac1}P 6 8 10 {lilac2} 16 8 10\r"
,"{lilac1}P 6 8 5 {lilac2} 16 8 5\r"
,"{lilac1}P 6 8 0 {lilac2} 16 8 0\r"
,"{lilac1}P 6 8 -5 {lilac2} 16 8 -5\r"
,"{lilac1}P 6 8 -10 {lilac2} 16 8 -10\r"
,"@vectorlist {peach} color=peach\r"
,"{peach1}P 6 6 10 {peach2} 16 6 10\r"
,"{peach1}P 6 6 5 {peach2} 16 6 5\r"
,"{peach1}P 6 6 0 {peach2} 16 6 0\r"
,"{peach1}P 6 6 -5 {peach2} 16 6 -5\r"
,"{peach1}P 6 6 -10 {peach2} 16 6 -10\r"

,"@vectorlist {peachtint} color=peachtint\r"
,"{pcht1}P 6 4 10 {pcht2}16 4 10\r"
,"{pcht1}P 6 4 5 {pcht2}16 4 5\r"
,"{pcht1}P 6 4 0 {pcht2}16 4 0\r"
,"{pcht1}P 6 4 -5 {pcht2}16 4 -5\r"
,"{pcht1}P 6 4 -10 {pcht2}16 4 -10\r"
,"@vectorlist {yellowtint} color=yellowtint\r"
,"{yt3}P 6 2 10 {yt4} 16 2 10\r"
,"{yt3}P 6 2 5 {yt4} 16 2 5\r"
,"{yt3}P 6 2 0 {yt4} 16 2 0\r"
,"{yt3}P 6 2 -5 {yt4} 16 2 -5\r"
,"{yt3}P 6 2 -10 {yt4} 16 2 -10\r"
,"@vectorlist {greentint} color=greentint\r"
,"{gt1}P 6 0 10 {gt2} 16 0 10\r"
,"{gt1}P 6 0 5 {gt2} 16 0 5\r"
,"{gt1}P 6 0 0 {gt2} 16 0 0\r"
,"{gt1}P 6 0 -5 {gt2} 16 0 -5\r"
,"{gt1}P 6 0 -10 {gt2} 16 0 -10\r"
,"@vectorlist {bluetint} color=bluetint\r"
,"{bt1}P 6 -2 10 {bt2} 16 -2 10\r"
,"{bt1}P 6 -2 5 {bt2} 16 -2 5\r"
,"{bt1}P 6 -2 0 {bt2} 16 -2 0\r"
,"{bt1}P 6 -2 -5 {bt2} 16 -2 -5\r"
,"{bt1}P 6 -2 -10 {bt2} 16 -2 -10\r"
,"@vectorlist {lilactint} color=lilactint\r"
,"{lact1}P 6 -4 10 {lact2} 16 -4 10\r"
,"{lact1}P 6 -4 5 {lact2} 16 -4 5\r"
,"{lact1}P 6 -4 0 {lact2} 16 -4 0\r"
,"{lact1}P 6 -4 -5 {lact2} 16 -4 -5\r"
,"{lact1}P 6 -4 -10 {lact2} 16 -4 -10\r"
,"@vectorlist {pinktint} color=pinktint\r"
,"{pkt1}P 6 -6 10 {pkt2} 16 -6 10\r"
,"{pkt1}P 6 -6 5 {pkt2} 16 -6 5\r"
,"{pkt1}P 6 -6 0 {pkt2} 16 -6 0\r"
,"{pkt1}P 6 -6 -5 {pkt2} 16 -6 -5\r"
,"{pkt1}P 6 -6 -10 {pkt2} 16 -6 -10\r"

,"@vectorlist {white} color=white\r"
,"{wht1}P 6 -8 10 {wht2} 16 -8 10\r"
,"{wht1}P 6 -8 5 {wht2} 16 -8 5\r"
,"{wht1}P 6 -8 0 {wht2} 16 -8 0\r"
,"{wht1}P 6 -8 -5 {wht2} 16 -8 -5\r"
,"{wht1}P 6 -8 -10 {wht2} 16 -8 -10\r"
,"@vectorlist {gray} color=gray\r"
,"{gra1}P 6 -10 10 {gra2} 16 -10 10\r"
,"{gra1}P 6 -10 5 {gra2} 16 -10 5\r"
,"{gra1}P 6 -10 0 {gra2} 16 -10 0\r"
,"{gra1}P 6 -10 -5 {gra2} 16 -10 -5\r"
,"{gra1}P 6 -10 -10 {gra2} 16 -10 -10\r"
,"@vectorlist {brown} color=brown\r"
,"{brn1}P 6 -12 10 {brn2} 16 -12 10\r"
,"{brn1}P 6 -12 5 {brn2} 16 -12 5\r"
,"{brn1}P 6 -12 0 {brn2} 16 -12 0\r"
,"{brn1}P 6 -12 -5 {brn2} 16 -12 -5\r"
,"{brn1}P 6 -12 -10 {brn2} 16 -12 -10\r"
,"@vectorlist {invisible} color=invisible\r"
,"{i1}P 6 -14 10 {i2} 16 -14 10\r"
,"{i1}P 6 -14 5 {i2} 16 -14 5\r"
,"{i1}P 6 -14 0 {i2} 16 -14 0\r"
,"{i1}P 6 -14 -5 {i2} 16 -14 -5\r"
,"{i1}P 6 -14 -10 {i2} 16 -14 -10\r"
,"@vectorlist {deadwhite} color=deadwhite \r"
,"{dw1}P 6 -16 10 {dw2} 16 -16 10 \r"
,"{dw1}P 6 -16 5 {dw2} 16 -16 5 \r"
,"{dw1}P 6 -16 0 {dw2} 16 -16 0 \r"
,"{dw1}P 6 -16 -5 {dw2} 16 -16 -5 \r"
,"{dw1}P 6 -16 -10 {dw2} 16 -16 -10 \r"

,"@group dominant {labels}\r"
,"@labellist {colorlabl} color=gray\r"
,"{deadblack} -8.5 -16 10 { } -10 -16 10\r"
,"{red} -8.5 -14 10 {A} -10 -14 10\r"
,"{orange} -8.5 -12 10 {B} -10 -12 10\r"
,"{gold} -8.5 -10 10 {C} -10 -10 10\r"
,"{yellow} -8.5 -8 10 {D} -10 -8 10\r"
,"{lime} -8.5 -6 10 {E} -10 -6 10\r"
,"{green} -8.5 -4 10 {F} -10 -4 10\r"
,"{sea} -8.5 -2 10 {G} -10 -2 10\r"
,"{cyan} -8.5 0 10 {H} -10 0 10\r"
,"{sky} -8.5 2 10 {I} -10 2 10\r"
,"{blue} -8.5 4 10 {J} -10 4 10\r"
,"{purple} -8.5 6 10 {K} -10 6 10\r"
,"{magenta} -8.5 8 10 {L} -10 8 10\r"
,"{hotpink} -8.5 10 10 {M} -10 10 10\r"

,"{pink} 5.5 10 10 {N} 4 10 10\r"
,"{lilac} 5.5 8 10 {O} 4 8 10\r"
,"{peach} 5.5 6 10 {P} 4 6 10\r"

,"{peachtint} 5.5 4 10 {Q} 4 4 10\r"
,"{yellowtint} 5.5 2 10 {R} 4 2 10\r"
,"{greentint} 5.5 0 10 {S} 4 0 10\r"
,"{bluetint} 5.5 -2 10 {T} 4 -2 10\r"
,"{lilactint} 5.5 -4 10 {U} 4 -4 10\r"
,"{pinktint} 5.5  -6 10 {V} 4 -6 10\r"

,"{white} 5.5 -8 10 {W} 4 -8 10\r"
,"{gray} 5.5 -10 10 {X} 4 -10 10\r"
,"{brown} 5.5 -12 10 {Y} 4 -12 10\r"
,"{invisible} 5.5 -14 10 {Z} 4 -14 10\r"
,"{deadwhite} 5.5 -16 10 { } 4 -16 10\r"
,"\r"
,"@group {zero} dominant off \r"
,"@balllist {zero} color= gray\r"
,"{zero} 0 0 0\r"
,"\r"
,"@finish\r"
};

 static char*  magedemo[] = 
{" \r"
#ifndef OLD68KMAC /*defined in MAGE.h*/
,"@text\r"
,"Question example:\r"
,"*{Q: How far does the Sulfur move?}*\r"
,"(Write out and read in again so Input Text parser can recognize question.)\r"
,"\r"
,"@caption\r"
,"Alternate conformations found for Met 52 of file 5PTI (Wlodawer et al., 1984).\r" 
,"Conf. A in gold and conf. B in orange. \r"
,"The two can be compared either by switching between them with 'ANIMATE', \r"
,"or by turning both on at the same time. \r"
,"The change is primarily in the Chi2 angle. \r"
,"N, O atoms are color-coded balls, S atoms are spheres.\r"
,"@viewid {side}\r"
,"@zoom 1.14\r"
,"@zslab 240\r"
,"@center 23.36 17.5 -6.12\r"
,"@matrix \r"
,"-.427847 -.896668 -.113723 .589963 -.181724 -.786715 .684757 -.403687 .60675\r"
,"@2viewid {down cb-cg}\r"
,"@2zoom 1.14\r"
,"@2zslab 360\r"
,"@2center 23.541 17.007 -5.538\r"
,"@2matrix\r"
,"-.358812 -.788558 -.499429 .415252 .344339 -.842019 .835955 -.509516 .203896\r"
,"@pointmaster 'A' {arrow}\r"
,"@1aspect {color1}\r"
,"@2aspect {color2}\r"
,"@3aspect {color3}\r"
,"@4aspect {color4}\r"
,"@group {51-53 mc} \r"
,"@labellist off {label}\r"
,"{ Met 52} 23.975, 16.86, -5.592\r"
,"@wordlist off {title}\r"
,"{title} <Alternate \r"
,"conformations \r"
,"of Met 52> 22.63, 17.94, -4.74\r"
,"@subgroup dominant {main ch}\r"
,"@balllist {n} color= sky radius= .1 <marked with balls>\r"
,"{n met 52} 24.405, 16.565, -4.212\r"
,"{n arg  53} 25.206, 18.931, -5.556\r"
,"@balllist {o} color= red radius= .1\r"
,"{o met 52} 25.254, 17.714, -7.432\r"
,"{o cys  51} 26.397, 15.689, -4.871\r"
,"@vectorlist {main ch} color= gray \r"
,"{ca cys 51} P 25.822, 15.753, -2.493 {c cys 51} L 25.591, 15.998, -3.994 \r"
,"{o cys 51} L 26.397, 15.689, -4.871\r"
,"{c cys 51} P 25.591, 15.998, -3.994 {n met 52} L 24.405, 16.565, -4.212 \r"
,"{ca met 52} L 23.975, 16.86, -5.592\r"
,"{c met 52} L 24.865, 17.909, -6.259\r"
,"{o met 52} L 25.254, 17.714, -7.432\r"
,"{c met 52} P 24.865, 17.909, -6.259 {n arg 53} L 25.206, 18.931, -5.556 \r"
,"{ca arg 53} L 26.111, 19.97, -6.044 \r"

,"@group dominant {conf A} animate\r"
,"@spherelist {S} color= yellow master= {S atoms} radius= .2\r"
,"{sd a met 52} <aspects> (DAFJ) 21.684, 14.673, -6.752\r"
,"@vectorlist {sc a} color= gold master= {side ch}\r"
,"{ca met 52} P 23.975, 16.860, -5.592 {cb met 52} L 22.481, 17.237, -5.692 \r"
,"{cg a met 52} L 21.619, 16.002, -5.527\r"
,"{sd a met 52} L 21.684, 14.673, -6.752\r"
,"{ce a met 52} L 20.896, 15.481, -8.152\r"
,"@arrowlist {A} color= magenta radius= 0.3 angle= 30\r"
,"{sd a met 52} L 'A' 21.684, 14.673, -6.752\r" 
,"{sd b met 52} L 'A' 19.845 ,16.563, -5.694\r"

,"@group dominant {conf B} animate\r"
,"@spherelist {S} color= yellow master= {S atoms} radius= .2\r"
,"{sd b met 52} <aspects> (DAFJ) 19.845 ,16.563, -5.694\r"
,"@vectorlist {sc b} color= orange master= {side ch}\r"
,"{ca met 52} P 23.975, 16.860, -5.592 {cb met 52} L 22.481, 17.237, -5.692\r"
,"{cg b met 52} L 21.536, 16.149, -5.278\r"
,"{sd b met 52} L 19.845 ,16.563, -5.694\r"
,"{ce b met 52} L 19.639, 18.241, -5.038\r"
,"@arrowlist {A} color= magenta radius= 0.3 angle= 30\r"
,"{sd b met 52} L 'A' 19.845 ,16.563, -5.694\r"
,"{sd a met 52} L 'A' 21.684, 14.673, -6.752\r" 
,"@group {frame} dominant \r"
,"@vectorlist {frame} color=blue screen \r"
,"-180 -180 0 \r"
," 180 -180 0 \r"
," 180  180 0 \r"
,"-180  180 0 \r"
,"-180 -180 0 \r"
,"P -10 0 0 \r"
,"   10 0 0 \r"
,"P 0  10 0 \r"
,"  0 -10 0 \r"
,"{center}P 0 0 0 \r"
,"{center}  0 0 0 \r"
,"\r"
#endif /*NOT OLD68KMAC defined in MAGE.h*/

,"@finish \r"
};

static char*  egamickdockrefwidget[] = 
{" \r"
,"@text\r"
,"egamick docking reference widget\r"
,"\r"
,"@caption\r"
,"egamick docking reference widget\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U gray -10   1 -10  U gray  10   1 -10  U gray  10  21 -10 \r"
,"  U gray -10  21 -10  U gray -10   1 -10 \r"
,"P U gray -10   1  10  U gray  10   1  10  U gray  10  21  10 \r"
,"  U gray -10  21  10  U gray -10   1  10 \r"
,"P U gray -10   1 -10  U gray -10   1  10 \r"
,"P U gray  10   1 -10  U gray  10   1  10 \r"
,"P U gray  10  21 -10  U gray  10  21  10 \r"
,"P U gray -10  21 -10  U gray -10  21  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"{a1}P -1.000 -1.000 0.000 \r"
,"{a2}-1.000 0.000 1.732 \r"
,"{a3}-1.000 1.000 0.000 \r"
,"{b3} 0.000 1.000 -1.732 \r"
,"{c3} 1.000 1.000 0.000 \r"
,"{b1}P 0.000 -1.000 -1.732 \r"
,"{c1} 1.000 -1.000 0.000 \r"
,"{c2} 1.000 0.000 1.730 \r"
,"{c3} 1.000 1.000 0.000 \r"
,"{a3}P -1.000 1.000 0.000 \r"
,"{a1}-1.000 -1.000 0.000 \r"
,"{c3}P 1.000 1.000 0.000 \r"
,"{c1} 1.000 -1.000 0.000 \r"
,"{a2}P -1.000 0.000 1.732 \r"
,"{c2} 1.000 0.000 1.730 \r"
,"{a1}P -1.000 -1.000 0.000 \r"
,"{c1} 1.000 -1.000 0.000 \r"
,"{a3}P -1.000 1.000 0.000 \r"
,"{c3} 1.000 1.000 0.000 \r"
,"{b3}P 0.000 1.000 -1.732 \r"
,"{b1} 0.000 -1.000 -1.732 \r"
,"{a1}P -1.000 -1.000 0.000 \r"
,"{b1}0.000 -1.000 -1.732 \r"
,"{ctrBall}P 0.000 0.000 0.000 \r"
,"{ctrBall}  0.000 0.000 0.000 \r"
,"\r"
,"@finish \r"
};

/*,"@balllist {ctrBall} color=white \r"*/

static char*  egamickdockrefctrwidget[] = 
{" \r"
,"@text\r"
,"egamick docking reference widget\r"
,"\r"
,"@caption\r"
,"egamick docking reference widget\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U invisible -10 -10 -10  U invisible  10 -10 -10 U invisible  10  10 -10 \r"
,"  U invisible -10  10 -10  U invisible -10 -10 -10 \r"
,"P U invisible -10 -10  10  U invisible  10 -10  10 U invisible  10  10  10 \r"
,"  U invisible -10  10  10  U invisible -10 -10  10 \r"
,"P U invisible -10 -10 -10  U invisible -10 -10  10 \r"
,"P U invisible  10 -10 -10  U invisible  10 -10  10 \r"
,"P U invisible  10  10 -10  U invisible  10  10  10 \r"
,"P U invisible -10  10 -10  U invisible -10  10  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"{a1}P -1.000 -1.000 0.000 \r"
,"{a2}-1.000 0.000 1.732 \r"
,"{a3}-1.000 1.000 0.000 \r"
,"{b3} 0.000 1.000 -1.732 \r"
,"{c3} 1.000 1.000 0.000 \r"
,"{b1}P 0.000 -1.000 -1.732 \r"
,"{c1} 1.000 -1.000 0.000 \r"
,"{c2} 1.000 0.000 1.730 \r"
,"{c3} 1.000 1.000 0.000 \r"
,"{a3}P -1.000 1.000 0.000 \r"
,"{a1}-1.000 -1.000 0.000 \r"
,"{c3}P 1.000 1.000 0.000 \r"
,"{c1} 1.000 -1.000 0.000 \r"
,"{a2}P -1.000 0.000 1.732 \r"
,"{c2} 1.000 0.000 1.730 \r"
,"{a1}P -1.000 -1.000 0.000 \r"
,"{c1} 1.000 -1.000 0.000 \r"
,"{a3}P -1.000 1.000 0.000 \r"
,"{c3} 1.000 1.000 0.000 \r"
,"{b3}P 0.000 1.000 -1.732 \r"
,"{b1} 0.000 -1.000 -1.732 \r"
,"{a1}P -1.000 -1.000 0.000 \r"
,"{b1}0.000 -1.000 -1.732 \r"
,"{ctrBall}P 0.000 0.000 0.000 \r"
,"{ctrBall}  0.000 0.000 0.000 \r"
,"\r"
,"@finish \r"
};

/*,"@balllist {ctrBall} color=white \r"*/

static char*  egamickdocknextwidget[] = 
{" \r"
,"\r"
,"@group {mobile} dominant \r"
,"@vectorlist {mobile} color=yellowtint \r"
,"{a1}P 11.000 -1.000 0.000 \r"
,"{a2}  11.000  0.000 1.732 \r"
,"{a3}  11.000  1.000 0.000 \r"
,"{b3}  12.000  1.000 -1.732 \r"
,"{c3}  13.000  1.000 0.000 \r"
,"{b1}P 12.000 -1.000 -1.732 \r"
,"{c1}  13.000 -1.000 0.000 \r"
,"{c2}  13.000  0.000 1.730 \r"
,"{c3}  13.000  1.000 0.000 \r"
,"{a3}P 11.000  1.000 0.000 \r"
,"{a1}  11.000 -1.000 0.000 \r"
,"{c3}P 13.000  1.000 0.000 \r"
,"{c1}  13.000 -1.000 0.000 \r"
,"{a2}P 11.000  0.000 1.732 \r"
,"{c2}  13.000  0.000 1.730 \r"
,"{a1}P 11.000 -1.000 0.000 \r"
,"{c1}  13.000 -1.000 0.000 \r"
,"{a3}P 11.000  1.000 0.000 \r"
,"{c3}  13.000  1.000 0.000 \r"
,"{b3}P 12.000  1.000 -1.732 \r"
,"{b1}  12.000 -1.000 -1.732 \r"
,"{a1}P 11.000 -1.000 0.000 \r"
,"{b1}  12.000 -1.000 -1.732 \r"
,"{ctrBall}P 12.000 0.000 0.000 \r"
,"{ctrBall}  12.000 0.000 0.000 \r"
,"\r"
,"@finish \r"
};

/*,"@balllist {ctrBall} color=yellowtint \r"*/

static char*  egamickdockwidget[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"{a1}P 11.000 -1.000 0.000 \r"
,"{a2}  11.000  0.000 1.732 \r"
,"{a3}  11.000  1.000 0.000 \r"
,"{b3}  12.000  1.000 -1.732 \r"
,"{c3}  13.000  1.000 0.000 \r"
,"{b1}P 12.000 -1.000 -1.732 \r"
,"{c1}  13.000 -1.000 0.000 \r"
,"{c2}  13.000  0.000 1.730 \r"
,"{c3}  13.000  1.000 0.000 \r"
,"{a3}P 11.000  1.000 0.000 \r"
,"{a1}  11.000 -1.000 0.000 \r"
,"{c3}P 13.000  1.000 0.000 \r"
,"{c1}  13.000 -1.000 0.000 \r"
,"{a2}P 11.000  0.000 1.732 \r"
,"{c2}  13.000  0.000 1.730 \r"
,"{a1}P 11.000 -1.000 0.000 \r"
,"{c1}  13.000 -1.000 0.000 \r"
,"{a3}P 11.000  1.000 0.000 \r"
,"{c3}  13.000  1.000 0.000 \r"
,"{b3}P 12.000  1.000 -1.732 \r"
,"{b1}  12.000 -1.000 -1.732 \r"
,"{a1}P 11.000 -1.000 0.000 \r"
,"{b1}  12.000 -1.000 -1.732 \r"
,"{ctrBall}P 12.000 0.000 0.000 \r"
,"{ctrBall}  12.000 0.000 0.000 \r"
,"\r"
,"@finish \r"
};

/*,"@balllist {ctrBall} color=yellowtint \r"*/

static char*  egamickdockreftetrahedron[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U gray -10   1 -10  U gray  10   1 -10  U gray  10  21 -10 \r"
,"  U gray -10  21 -10  U gray -10   1 -10 \r"
,"P U gray -10   1  10  U gray  10   1  10  U gray  10  21  10 \r"
,"  U gray -10  21  10  U gray -10   1  10 \r"
,"P U gray -10   1 -10  U gray -10   1  10 \r"
,"P U gray  10   1 -10  U gray  10   1  10 \r"
,"P U gray  10  21 -10  U gray  10  21  10 \r"
,"P U gray -10  21 -10  U gray -10  21  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"P -0.707 -0.707  0.707    0.707  0.707  0.707 \r"
,"P  0.707 -0.707 -0.707   -0.707  0.707 -0.707 \r"
,"P -0.707 -0.707  0.707    0.707 -0.707 -0.707 \r"
,"P -0.707 -0.707  0.707   -0.707  0.707 -0.707 \r"
,"P  0.707  0.707  0.707    0.707 -0.707 -0.707 \r"
,"P  0.707  0.707  0.707   -0.707  0.707 -0.707 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockrefctrtetrahedron[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U invisible -10 -10 -10  U invisible  10 -10 -10 U invisible  10  10 -10 \r"
,"  U invisible -10  10 -10  U invisible -10 -10 -10 \r"
,"P U invisible -10 -10  10  U invisible  10 -10  10 U invisible  10  10  10 \r"
,"  U invisible -10  10  10  U invisible -10 -10  10 \r"
,"P U invisible -10 -10 -10  U invisible -10 -10  10 \r"
,"P U invisible  10 -10 -10  U invisible  10 -10  10 \r"
,"P U invisible  10  10 -10  U invisible  10  10  10 \r"
,"P U invisible -10  10 -10  U invisible -10  10  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"P -0.707 -0.707  0.707    0.707  0.707  0.707 \r"
,"P  0.707 -0.707 -0.707   -0.707  0.707 -0.707 \r"
,"P -0.707 -0.707  0.707    0.707 -0.707 -0.707 \r"
,"P -0.707 -0.707  0.707   -0.707  0.707 -0.707 \r"
,"P  0.707  0.707  0.707    0.707 -0.707 -0.707 \r"
,"P  0.707  0.707  0.707   -0.707  0.707 -0.707 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdocknexttetrahedron[] = 
{" \r"
,"\r"
,"@group {mobile} dominant \r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11.293 -0.707  0.707   12.707  0.707  0.707 \r"
,"P 12.707 -0.707 -0.707   11.293  0.707 -0.707 \r"
,"P 11.293 -0.707  0.707   12.707 -0.707 -0.707 \r"
,"P 11.293 -0.707  0.707   11.293  0.707 -0.707 \r"
,"P 12.707  0.707  0.707   12.707 -0.707 -0.707 \r"
,"P 12.707  0.707  0.707   11.293  0.707 -0.707 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdocktetrahedron[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11.293 -0.707  0.707   12.707  0.707  0.707 \r"
,"P 12.707 -0.707 -0.707   11.293  0.707 -0.707 \r"
,"P 11.293 -0.707  0.707   12.707 -0.707 -0.707 \r"
,"P 11.293 -0.707  0.707   11.293  0.707 -0.707 \r"
,"P 12.707  0.707  0.707   12.707 -0.707 -0.707 \r"
,"P 12.707  0.707  0.707   11.293  0.707 -0.707 \r"
,"\r"
,"@finish \r"
};




static char*  egamickdockrefwater[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U gray -10   1 -10  U gray  10   1 -10  U gray  10  21 -10 \r"
,"  U gray -10  21 -10  U gray -10   1 -10 \r"
,"P U gray -10   1  10  U gray  10   1  10  U gray  10  21  10 \r"
,"  U gray -10  21  10  U gray -10   1  10 \r"
,"P U gray -10   1 -10  U gray -10   1  10 \r"
,"P U gray  10   1 -10  U gray  10   1  10 \r"
,"P U gray  10  21 -10  U gray  10  21  10 \r"
,"P U gray -10  21 -10  U gray -10  21  10 \r"
,"\r"
,"@vectorlist {water}\r"
,"{Hydrogen} P B 'B' r=0.2 white -1 0 0\r"
,"{Oxygen} P B 'B' red r=0.2 0 0 0\r"
,"{Hydrogen} P B 'B' white r=0.2 0.242 -0.970 -0.000\r"
,"{stagger} P B 'S' brown r=0.1 -0.363 1.455 0.000\r"
,"{stagger} P B 'S' brown r=0.1 -0.503 -0.644 1.258\r"
,"{stagger} P B 'S' brown r=0.1 1.500 0.000 0.000\r"
,"{stagger} P B 'S' brown r=0.1 -0.503 -0.644 -1.258\r"
,"{Hydrogen} P S 'A' r=1 white -1 0 0\r"
,"{Oxygen} P S 'A' r=1.4 red 0 0 0\r"
,"{Hydrogen} P S 'A' r=1 white 0.242 -0.970 -0.000\r"
,"{next}P 'L' peach -3 0 0\r"
,"{hydrogen} 'L' peach -1 0 0\r"
,"{Oxygen} 'L' peach 0 0 0\r"
,"{Hydrogen} 'L' peach 0.242 -0.970 -0.000\r"
,"{next} 'L' peach 0.726 -2.911 -0.000\r"
,"{Oxygen}P 'L' peach 0.000 0.000 0.000\r"
,"{construc} 'L' peach 1.006 1.288 2.516\r"
,"{Oxygen}P 'L' peach 0.000 0.000 0.000\r"
,"{construc} 'L' peach 1.006 1.288 -2.516\r"
,"\r"
,"@finish \r"
};




static char*  egamickdockrefctrwater[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U invisible -10 -10 -10  U invisible  10 -10 -10 U invisible  10  10 -10 \r"
,"  U invisible -10  10 -10  U invisible -10 -10 -10 \r"
,"P U invisible -10 -10  10  U invisible  10 -10  10 U invisible  10  10  10 \r"
,"  U invisible -10  10  10  U invisible -10 -10  10 \r"
,"P U invisible -10 -10 -10  U invisible -10 -10  10 \r"
,"P U invisible  10 -10 -10  U invisible  10 -10  10 \r"
,"P U invisible  10  10 -10  U invisible  10  10  10 \r"
,"P U invisible -10  10 -10  U invisible -10  10  10 \r"
,"\r"
,"@vectorlist {water}\r"
,"{Hydrogen} P B 'B' r=0.2 white -1 0 0\r"
,"{Oxygen} P B 'B' red r=0.2 0 0 0\r"
,"{Hydrogen} P B 'B' white r=0.2 0.242 -0.970 -0.000\r"
,"{stagger} P B 'S' brown r=0.1 -0.363 1.455 0.000\r"
,"{stagger} P B 'S' brown r=0.1 -0.503 -0.644 1.258\r"
,"{stagger} P B 'S' brown r=0.1 1.500 0.000 0.000\r"
,"{stagger} P B 'S' brown r=0.1 -0.503 -0.644 -1.258\r"
,"{Hydrogen} P S 'A' r=1 white -1 0 0\r"
,"{Oxygen} P S 'A' r=1.4 red 0 0 0\r"
,"{Hydrogen} P S 'A' r=1 white 0.242 -0.970 -0.000\r"
,"{next}P 'L' peach -3 0 0\r"
,"{hydrogen} 'L' peach -1 0 0\r"
,"{Oxygen} 'L' peach 0 0 0\r"
,"{Hydrogen} 'L' peach 0.242 -0.970 -0.000\r"
,"{next} 'L' peach 0.726 -2.911 -0.000\r"
,"{Oxygen}P 'L' peach 0.000 0.000 0.000\r"
,"{construc} 'L' peach 1.006 1.288 2.516\r"
,"{Oxygen}P 'L' peach 0.000 0.000 0.000\r"
,"{construc} 'L' peach 1.006 1.288 -2.516\r"
,"\r"
,"@finish \r"
};

static char*  egamickdocknextwater[] = 
{" \r"
,"\r"
,"@group {mobile} \r"
,"@vectorlist {water}\r"
,"{Hydrogen} P B 'B' r=0.2 white 9 0 0\r"
,"{Oxygen} P B 'B' red r=0.2 10 0 0\r"
,"{Hydrogen} P B 'B' white r=0.2 10.242 -0.970 -0.000\r"
,"{stagger} P B 'S' brown r=0.1 9.637 1.455 0.000\r"
,"{stagger} P B 'S' brown r=0.1 9.497 -0.644 1.258\r"
,"{stagger} P B 'S' brown r=0.1 11.500 0.000 0.000\r"
,"{stagger} P B 'S' brown r=0.1 9.497 -0.644 -1.258\r"
,"{Hydrogen} P S 'A' r=1 white 9 0 0\r"
,"{Oxygen} P S 'A' r=1.4 red 10 0 0\r"
,"{Hydrogen} P S 'A' r=1 white 10.242 -0.970 -0.000\r"
,"{next}P 'L' peach 7 0 0\r"
,"{hydrogen} 'L' peach 9 0 0\r"
,"{Oxygen} 'L' peach 10 0 0\r"
,"{Hydrogen} 'L' peach 10.242 -0.970 -0.000\r"
,"{next} 'L' peach 10.726 -2.911 -0.000\r"
,"{Oxygen}P 'L' peach 10.000 0.000 0.000\r"
,"{construc} 'L' peach 11.006 1.288 2.516\r"
,"{Oxygen}P 'L' peach 10.000 0.000 0.000\r"
,"{construc} 'L' peach 11.006 1.288 -2.516\r"
,"\r"
,"@finish \r"
};

static char*  egamickdockwater[] = 
{" \r"
,"\r"
,"@vectorlist {water}\r"
,"{Hydrogen} P B 'B' r=0.2 white 9 0 0\r"
,"{Oxygen} P B 'B' red r=0.2 10 0 0\r"
,"{Hydrogen} P B 'B' white r=0.2 10.242 -0.970 -0.000\r"
,"{stagger} P B 'S' brown r=0.1 9.637 1.455 0.000\r"
,"{stagger} P B 'S' brown r=0.1 9.497 -0.644 1.258\r"
,"{stagger} P B 'S' brown r=0.1 11.500 0.000 0.000\r"
,"{stagger} P B 'S' brown r=0.1 9.497 -0.644 -1.258\r"
,"{Hydrogen} P S 'A' r=1 white 9 0 0\r"
,"{Oxygen} P S 'A' r=1.4 red 10 0 0\r"
,"{Hydrogen} P S 'A' r=1 white 10.242 -0.970 -0.000\r"
,"{next}P 'L' peach 7 0 0\r"
,"{hydrogen} 'L' peach 9 0 0\r"
,"{Oxygen} 'L' peach 10 0 0\r"
,"{Hydrogen} 'L' peach 10.242 -0.970 -0.000\r"
,"{next} 'L' peach 10.726 -2.911 -0.000\r"
,"{Oxygen}P 'L' peach 10.000 0.000 0.000\r"
,"{construc} 'L' peach 11.006 1.288 2.516\r"
,"{Oxygen}P 'L' peach 10.000 0.000 0.000\r"
,"{construc} 'L' peach 11.006 1.288 -2.516\r"
,"\r"
,"@finish \r"
};

static char*  egamickdockreferencecube[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U gray -10   1 -10  U gray  10   1 -10  U gray  10  21 -10 \r"
,"  U gray -10  21 -10  U gray -10   1 -10 \r"
,"P U gray -10   1  10  U gray  10   1  10  U gray  10  21  10 \r"
,"  U gray -10  21  10  U gray -10   1  10 \r"
,"P U gray -10   1 -10  U gray -10   1  10 \r"
,"P U gray  10   1 -10  U gray  10   1  10 \r"
,"P U gray  10  21 -10  U gray  10  21  10 \r"
,"P U gray -10  21 -10  U gray -10  21  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"P -1 -1 -1    1 -1 -1    1  1 -1   -1  1 -1   -1 -1 -1 \r"
,"P -1 -1  1    1 -1  1    1  1  1   -1  1  1   -1 -1  1 \r"
,"P -1 -1 -1   -1 -1  1 \r"
,"P  1 -1 -1    1 -1  1 \r"
,"P  1  1 -1    1  1  1 \r"
,"P -1  1 -1   -1  1  1 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockreferencectrcube[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U invisible -10 -10 -10  U invisible  10 -10 -10 U invisible  10  10 -10 \r"
,"  U invisible -10  10 -10  U invisible -10 -10 -10 \r"
,"P U invisible -10 -10  10  U invisible  10 -10  10 U invisible  10  10  10 \r"
,"  U invisible -10  10  10  U invisible -10 -10  10 \r"
,"P U invisible -10 -10 -10  U invisible -10 -10  10 \r"
,"P U invisible  10 -10 -10  U invisible  10 -10  10 \r"
,"P U invisible  10  10 -10  U invisible  10  10  10 \r"
,"P U invisible -10  10 -10  U invisible -10  10  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"P -1 -1 -1    1 -1 -1    1  1 -1   -1  1 -1   -1 -1 -1 \r"
,"P -1 -1  1    1 -1  1    1  1  1   -1  1  1   -1 -1  1 \r"
,"P -1 -1 -1   -1 -1  1 \r"
,"P  1 -1 -1    1 -1  1 \r"
,"P  1  1 -1    1  1  1 \r"
,"P -1  1 -1   -1  1  1 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdocknextcube[] = 
{" \r"
,"@group {mobile} dominant \r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11 -1 -1   13 -1 -1   13  1 -1   11  1 -1   11 -1 -1 \r"
,"P 11 -1  1   13 -1  1   13  1  1   11  1  1   11 -1  1 \r"
,"P 11 -1 -1   11 -1  1 \r"
,"P 13 -1 -1   13 -1  1 \r"
,"P 13  1 -1   13  1  1 \r"
,"P 11  1 -1   11  1  1 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockcube[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11 -1 -1   13 -1 -1   13  1 -1   11  1 -1   11 -1 -1 \r"
,"P 11 -1  1   13 -1  1   13  1  1   11  1  1   11 -1  1 \r"
,"P 11 -1 -1   11 -1  1 \r"
,"P 13 -1 -1   13 -1  1 \r"
,"P 13  1 -1   13  1  1 \r"
,"P 11  1 -1   11  1  1 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdocklion[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11 -1 -1   13 -1 -1   13  1 -1   11  1 -1   11 -1 -1 \r"
,"P 11 -1  1   13 -1  1   13  1  1   11  1  1   11 -1  1 \r"
,"P 11 -1 -1   11 -1  1 \r"
,"P 13 -1 -1   13 -1  1 \r"
,"P 13  1 -1   13  1  1 \r"
,"P 11  1 -1   11  1  1 \r"
,"P 11 -1  3   13 -1  3   13  1  3   11  1  3   11 -1  3 \r"
,"P 11 -1  1   11 -1  3 \r"
,"P 13 -1  1   13 -1  3 \r"
,"P 13  1  1   13  1  3 \r"
,"P 11  1  1   11  1  3 \r"
,"P 11 -1  5   13 -1  5   13  1  5   11  1  5   11 -1  5 \r"
,"P 11 -1  3   11 -1  5 \r"
,"P 13 -1  3   13 -1  5 \r"
,"P 13  1  3   13  1  5 \r"
,"P 11  1  3   11  1  5 \r"
,"P 11 -1  7   13 -1  7   13  1  7   11  1  7   11 -1  7 \r"
,"P 11 -1  5   11 -1  7 \r"
,"P 13 -1  5   13 -1  7 \r"
,"P 13  1  5   13  1  7 \r"
,"P 11  1  5   11  1  7 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockwall[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11 -1 -1   13 -1 -1   13  1 -1   11  1 -1   11 -1 -1 \r"
,"P 11 -1  1   13 -1  1   13  1  1   11  1  1   11 -1  1 \r"
,"P 11 -1 -1   11 -1  1 \r"
,"P 13 -1 -1   13 -1  1 \r"
,"P 13  1 -1   13  1  1 \r"
,"P 11  1 -1   11  1  1 \r"
,"P 11 -1  3   13 -1  3   13  1  3   11  1  3   11 -1  3 \r"
,"P 11 -1  1   11 -1  3 \r"
,"P 13 -1  1   13 -1  3 \r"
,"P 13  1  1   13  1  3 \r"
,"P 11  1  1   11  1  3 \r"
,"P 11 -1  5   13 -1  5   13  1  5   11  1  5   11 -1  5 \r"
,"P 11 -1  3   11 -1  5 \r"
,"P 13 -1  3   13 -1  5 \r"
,"P 13  1  3   13  1  5 \r"
,"P 11  1  3   11  1  5 \r"
,"P 11 -1  7   13 -1  7   13  1  7   11  1  7   11 -1  7 \r"
,"P 11 -1  5   11 -1  7 \r"
,"P 13 -1  5   13 -1  7 \r"
,"P 13  1  5   13  1  7 \r"
,"P 11  1  5   11  1  7 \r"
,"P 15 -1 -1   17 -1 -1   17  1 -1   15  1 -1   15 -1 -1 \r"
,"P 15 -1  1   17 -1  1   17  1  1   15  1  1   15 -1  1 \r"
,"P 15 -1 -1   15 -1  1 \r"
,"P 17 -1 -1   17 -1  1 \r"
,"P 17  1 -1   17  1  1 \r"
,"P 15  1 -1   15  1  1 \r"
,"P 15 -1  3   17 -1  3   17  1  3   15  1  3   15 -1  3 \r"
,"P 15 -1  1   15 -1  3 \r"
,"P 17 -1  1   17 -1  3 \r"
,"P 17  1  1   17  1  3 \r"
,"P 15  1  1   15  1  3 \r"
,"P 15 -1  5   17 -1  5   17  1  5   15  1  5   15 -1  5 \r"
,"P 15 -1  3   15 -1  5 \r"
,"P 17 -1  3   17 -1  5 \r"
,"P 17  1  3   17  1  5 \r"
,"P 15  1  3   15  1  5 \r"
,"P 15 -1  7   17 -1  7   17  1  7   15  1  7   15 -1  7 \r"
,"P 15 -1  5   15 -1  7 \r"
,"P 17 -1  5   17 -1  7 \r"
,"P 17  1  5   17  1  7 \r"
,"P 15  1  5   15  1  7 \r"
,"P 19 -1 -1   21 -1 -1   21  1 -1   19  1 -1   19 -1 -1 \r"
,"P 19 -1  1   21 -1  1   21  1  1   19  1  1   19 -1  1 \r"
,"P 19 -1 -1   19 -1  1 \r"
,"P 21 -1 -1   21 -1  1 \r"
,"P 21  1 -1   21  1  1 \r"
,"P 19  1 -1   19  1  1 \r"
,"P 19 -1  3   21 -1  3   21  1  3   19  1  3   19 -1  3 \r"
,"P 19 -1  1   19 -1  3 \r"
,"P 21 -1  1   21 -1  3 \r"
,"P 21  1  1   21  1  3 \r"
,"P 19  1  1   19  1  3 \r"
,"P 19 -1  5   21 -1  5   21  1  5   19  1  5   19 -1  5 \r"
,"P 19 -1  3   19 -1  5 \r"
,"P 21 -1  3   21 -1  5 \r"
,"P 21  1  3   21  1  5 \r"
,"P 19  1  3   19  1  5 \r"
,"P 19 -1  7   21 -1  7   21  1  7   19  1  7   19 -1  7 \r"
,"P 19 -1  5   19 -1  7 \r"
,"P 21 -1  5   21 -1  7 \r"
,"P 21  1  5   21  1  7 \r"
,"P 19  1  5   19  1  7 \r"
,"P 23 -1 -1   25 -1 -1   25  1 -1   23  1 -1   23 -1 -1 \r"
,"P 23 -1  1   25 -1  1   25  1  1   23  1  1   23 -1  1 \r"
,"P 23 -1 -1   23 -1  1 \r"
,"P 25 -1 -1   25 -1  1 \r"
,"P 25  1 -1   25  1  1 \r"
,"P 23  1 -1   23  1  1 \r"
,"P 23 -1  3   25 -1  3   25  1  3   23  1  3   23 -1  3 \r"
,"P 23 -1  1   23 -1  3 \r"
,"P 25 -1  1   25 -1  3 \r"
,"P 25  1  1   25  1  3 \r"
,"P 23  1  1   23  1  3 \r"
,"P 23 -1  5   25 -1  5   25  1  5   23  1  5   23 -1  5 \r"
,"P 23 -1  3   23 -1  5 \r"
,"P 25 -1  3   25 -1  5 \r"
,"P 25  1  3   25  1  5 \r"
,"P 23  1  3   23  1  5 \r"
,"P 23 -1  7   25 -1  7   25  1  7   23  1  7   23 -1  7 \r"
,"P 23 -1  5   23 -1  7 \r"
,"P 25 -1  5   25 -1  7 \r"
,"P 25  1  5   25  1  7 \r"
,"P 23  1  5   23  1  7 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockknight[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11 -1 -1   13 -1 -1   13  1 -1   11  1 -1   11 -1 -1 \r"
,"P 11 -1  1   13 -1  1   13  1  1   11  1  1   11 -1  1 \r"
,"P 11 -1 -1   11 -1  1 \r"
,"P 13 -1 -1   13 -1  1 \r"
,"P 13  1 -1   13  1  1 \r"
,"P 11  1 -1   11  1  1 \r"
,"P 11 -1  3   13 -1  3   13  1  3   11  1  3   11 -1  3 \r"
,"P 11 -1  1   11 -1  3 \r"
,"P 13 -1  1   13 -1  3 \r"
,"P 13  1  1   13  1  3 \r"
,"P 11  1  1   11  1  3 \r"
,"P 11 -1  5   13 -1  5   13  1  5   11  1  5   11 -1  5 \r"
,"P 11 -1  3   11 -1  5 \r"
,"P 13 -1  3   13 -1  5 \r"
,"P 13  1  3   13  1  5 \r"
,"P 11  1  3   11  1  5 \r"
,"P 11  1  5   13  1  5   13  3  5   11  3  5   11  1  5 \r"
,"P 11  1  3   11  3  3 \r"
,"P 13  1  3   13  3  3 \r"
,"P 13  3  3   13  3  5 \r"
,"P 11  3  3   11  3  5 \r"
,"P 11  3  3   13  3  3 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockknave[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11 -1 -1   13 -1 -1   13  1 -1   11  1 -1   11 -1 -1 \r"
,"P 11 -1  1   13 -1  1   13  1  1   11  1  1   11 -1  1 \r"
,"P 11 -1 -1   11 -1  1 \r"
,"P 13 -1 -1   13 -1  1 \r"
,"P 13  1 -1   13  1  1 \r"
,"P 11  1 -1   11  1  1 \r"
,"P 11 -1  3   13 -1  3   13  1  3   11  1  3   11 -1  3 \r"
,"P 11 -1  1   11 -1  3 \r"
,"P 13 -1  1   13 -1  3 \r"
,"P 13  1  1   13  1  3 \r"
,"P 11  1  1   11  1  3 \r"
,"P 11  1  3   13  1  3   13  3  3   11  3  3   11  1  3 \r"
,"P 11  1  1   11  1  3 \r"
,"P 11  1  1   11  3  1 \r"
,"P 13  3  1   13  1  1 \r"
,"P 13  3  1   13  3  3 \r"
,"P 11  3  1   11  3  3 \r"
,"P 11  3  1   13  3  1 \r"

,"P  9 -1  3   11 -1  3   11  1  3    9  1  3    9 -1  3 \r"
,"P  9 -1  1    9 -1  3 \r"
,"P 11 -1  1   11 -1  3 \r"
,"P  9  1  1   11  1  1 \r"
,"P  9  1  1    9  1  3 \r"
,"P  9 -1  1    9  1  1 \r"
,"P  9 -1  1   11 -1  1 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockcaltrop[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 12.000 1.000 -1.000 \r"
,"11.333 2.333 0.333 \r"
,"10.000 1.000 1.000 \r"
,"P 12.000 -1.000 1.000 \r"
,"13.333 0.333 0.333 \r"
,"12.667 1.667 1.667 \r"
,"11.333 0.333 2.333 \r"
,"12.000 -1.000 1.000 \r"
,"P 12.000 1.000 -1.000 \r"
,"13.333 0.333 0.333 \r"
,"P 11.333 2.333 0.333 \r"
,"12.667 1.667 1.667 \r"
,"P 10.000 1.000 1.000 \r"
,"11.333 0.333 2.333 \r"
,"P 10.667 0.333 -2.333 \r"
,"12.000 1.000 -1.000 \r"
,"10.667 2.333 -0.333 \r"
,"9.333 1.667 -1.667 \r"
,"10.667 0.333 -2.333 \r"
,"P 10.000 1.000 1.000 \r"
,"8.667 0.333 -0.333 \r"
,"10.000 -1.000 -1.000 \r"
,"P 10.667 0.333 -2.333 \r"
,"10.000 -1.000 -1.000 \r"
,"P 10.667 2.333 -0.333 \r"
,"10.000 1.000 1.000 \r"
,"P 9.333 1.667 -1.667 \r"
,"8.667 0.333 -0.333 \r"
,"P 10.000 -1.000 -1.000 \r"
,"10.667 -2.333 0.333 \r"
,"12.000 -1.000 1.000 \r"
,"P 8.667 -0.333 0.333 \r"
,"9.333 -1.667 1.667 \r"
,"10.667 -0.333 2.333 \r"
,"10.000 1.000 1.000 \r"
,"8.667 -0.333 0.333 \r"
,"P 10.000 -1.000 -1.000 \r"
,"8.667 -0.333 0.333 \r"
,"P 10.667 -2.333 0.333 \r"
,"9.333 -1.667 1.667 \r"
,"P 12.000 -1.000 1.000 \r"
,"10.667 -0.333 2.333 \r"
,"P 11.333 -2.333 -0.333 \r"
,"12.667 -1.667 -1.667 \r"
,"11.333 -0.333 -2.333 \r"
,"10.000 -1.000 -1.000 \r"
,"11.333 -2.333 -0.333 \r"
,"P 12.000 -1.000 1.000 \r"
,"13.333 -0.333 -0.333 \r"
,"12.000 1.000 -1.000 \r"
,"P 11.333 -2.333 -0.333 \r"
,"12.000 -1.000 1.000 \r"
,"P 12.667 -1.667 -1.667 \r"
,"13.333 -0.333 -0.333 \r"
,"P 11.333 -0.333 -2.333 \r"
,"12.000 1.000 -1.000 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockrefoctahedron[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P U gray -10   1 -10  U gray  10   1 -10  U gray  10  21 -10 \r"
,"  U gray -10  21 -10  U gray -10   1 -10 \r"
,"P U gray -10   1  10  U gray  10   1  10  U gray  10  21  10 \r"
,"  U gray -10  21  10  U gray -10   1  10 \r"
,"P U gray -10   1 -10  U gray -10   1  10 \r"
,"P U gray  10   1 -10  U gray  10   1  10 \r"
,"P U gray  10  21 -10  U gray  10  21  10 \r"
,"P U gray -10  21 -10  U gray -10  21  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"P -1.0 -1.0 0.0  1.0 -1.0 0.0  1.0  1.0 0.0  -1.0  1.0 0.0  -1.0 -1.0 0.0 \r"
,"P  0.0  0.0  1.414   -1.0 -1.0  0.0 \r"
,"P  0.0  0.0  1.414    1.0 -1.0  0.0 \r"
,"P  0.0  0.0  1.414    1.0  1.0  0.0 \r"
,"P  0.0  0.0  1.414   -1.0  1.0  0.0 \r"
,"P  0.0  0.0 -1.414   -1.0 -1.0  0.0 \r"
,"P  0.0  0.0 -1.414    1.0 -1.0  0.0 \r"
,"P  0.0  0.0 -1.414    1.0  1.0  0.0 \r"
,"P  0.0  0.0 -1.414   -1.0  1.0  0.0 \r"
,"P invisible -10 -10 -10  invisible  -10 -10 -10 \r"
,"P invisible -10 -10  10  invisible  -10 -10  10 \r"
,"P invisible -10  10 -10  invisible  -10  10 -10 \r"
,"P invisible  10 -10 -10  invisible   10 -10 -10 \r"
,"P invisible -10  10  10  invisible  -10  10  10 \r"
,"P invisible  10  10 -10  invisible   10  10 -10 \r"
,"P invisible  10 -10  10  invisible   10 -10  10 \r"
,"P invisible  10  10  10  invisible   10  10  10 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockrefctroctahedron[] = 
{" \r"
,"@text\r"
,"egamick docking reference structure\r"
,"\r"
,"@caption\r"
,"egamick docking reference structure\r"
,"\r"
,"@perspective\r"
,"@zoom 0.75\r"
,"@group {reference} dominant \r"
,"@vectorlist {frame} color=white \r"
,"P invisible -10 -10 -10  invisible  -10 -10 -10 \r"
,"P invisible -10 -10  10  invisible  -10 -10  10 \r"
,"P invisible -10  10 -10  invisible  -10  10 -10 \r"
,"P invisible  10 -10 -10  invisible   10 -10 -10 \r"
,"P invisible -10  10  10  invisible  -10  10  10 \r"
,"P invisible  10  10 -10  invisible   10  10 -10 \r"
,"P invisible  10 -10  10  invisible   10 -10  10 \r"
,"P invisible  10  10  10  invisible   10  10  10 \r"
,"\r"
,"@vectorlist {reference} color=white \r"
,"P -1.0 -1.0 0.0  1.0 -1.0 0.0  1.0  1.0 0.0  -1.0  1.0 0.0  -1.0 -1.0 0.0 \r"
,"P  0.0  0.0  1.414   -1.0 -1.0  0.0 \r"
,"P  0.0  0.0  1.414    1.0 -1.0  0.0 \r"
,"P  0.0  0.0  1.414    1.0  1.0  0.0 \r"
,"P  0.0  0.0  1.414   -1.0  1.0  0.0 \r"
,"P  0.0  0.0 -1.414   -1.0 -1.0  0.0 \r"
,"P  0.0  0.0 -1.414    1.0 -1.0  0.0 \r"
,"P  0.0  0.0 -1.414    1.0  1.0  0.0 \r"
,"P  0.0  0.0 -1.414   -1.0  1.0  0.0 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdocknextoctahedron[] = 
{" \r"
,"\r"
,"@group {mobile} dominant \r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11.0 -1.0  0.0  13.0 -1.0 0.0 13.0  1.0 0.0 11.0  1.0 0.0 11.0 -1.0 0.0 \r"
,"P 12.0  0.0  1.414   11.0 -1.0  0.0 \r"
,"P 12.0  0.0  1.414   13.0 -1.0  0.0 \r"
,"P 12.0  0.0  1.414   13.0  1.0  0.0 \r"
,"P 12.0  0.0  1.414   11.0  1.0  0.0 \r"
,"P 12.0  0.0 -1.414   11.0 -1.0  0.0 \r"
,"P 12.0  0.0 -1.414   13.0 -1.0  0.0 \r"
,"P 12.0  0.0 -1.414   13.0  1.0  0.0 \r"
,"P 12.0  0.0 -1.414   11.0  1.0  0.0 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockoctahedron[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11.0 -1.0  0.0  13.0 -1.0 0.0 13.0  1.0 0.0 11.0  1.0 0.0 11.0 -1.0 0.0 \r"
,"P 12.0  0.0  1.414   11.0 -1.0  0.0 \r"
,"P 12.0  0.0  1.414   13.0 -1.0  0.0 \r"
,"P 12.0  0.0  1.414   13.0  1.0  0.0 \r"
,"P 12.0  0.0  1.414   11.0  1.0  0.0 \r"
,"P 12.0  0.0 -1.414   11.0 -1.0  0.0 \r"
,"P 12.0  0.0 -1.414   13.0 -1.0  0.0 \r"
,"P 12.0  0.0 -1.414   13.0  1.0  0.0 \r"
,"P 12.0  0.0 -1.414   11.0  1.0  0.0 \r"
,"\r"
,"@finish \r"
};

static char*  egamickdockextra[] = 
{" \r"
,"\r"
,"@vectorlist {mobile} color=yellowtint \r"
,"P 11 -1 -3   13 -1 -3   13  1 -3   11  1 -3   11 -1 -3 \r"
,"P 11 -1 -1   13 -1 -1   13  1 -1   11  1 -1   11 -1 -1 \r"
,"P 11 -1 -3   11 -1 -1 \r"
,"P 13 -1 -3   13 -1 -1 \r"
,"P 13  1 -3   13  1 -1 \r"
,"P 11  1 -3   11  1 -1 \r"
,"\r"
,"P 27 -15 -3   29 -15 -3   29 -17 -3   27 -17 -3   27 -15 -3 \r"
,"P 27 -15 -1   29 -15 -1   29 -17 -1   27 -17 -1   27 -15 -1 \r"
,"P 27 -15 -3   27 -15 -1 \r"
,"P 29 -15 -3   29 -15 -1 \r"
,"P 29 -17 -3   29 -17 -1 \r"
,"P 27 -17 -3   27 -17 -1 \r"
,"\r"
,"P 11 -1  1   13 -1  1   13  1  1   11  1  1   11 -1  1 \r"
,"P 11 -1  3   13 -1  3   13  1  3   11  1  3   11 -1  3 \r"
,"P 11 -1  1   11 -1  3 \r"
,"P 13 -1  1   13 -1  3 \r"
,"P 13  1  1   13  1  3 \r"
,"P 11  1  1   11  1  3 \r"
,"\r"
,"P 27 -15  1   29 -15  1   29 -17  1   27 -17  1   27 -15  1 \r"
,"P 27 -15  3   29 -15  3   29 -17  3   27 -17  3   27 -15  3 \r"
,"P 27 -15  1   27 -15  3 \r"
,"P 29 -15  1   29 -15  3 \r"
,"P 29 -17  1   29 -17  3 \r"
,"P 27 -17  1   27 -17  3 \r"
,"\r"
,"P 12 0 ,-2 \r"
,"12  -6 ,-2 \r"
,"19  -8 ,-2 \r"
,"21 -12 ,-2 \r"
,"20 -13 ,-2 \r"
,"20 -15 ,-2 \r"
,"22 -15 ,-2 \r"
,"25 -13 ,-2 \r"
,"23 -10 ,-2 \r"
,"26 -10 ,-2 \r"
,"28 -13 ,-2 \r"
,"28 -16 ,-2 \r"
,"31 -16 ,-2 \r"
,"34 -14 ,-2 \r"
,"34 -11 ,-2 \r"
,"31 -13 ,-2 \r"
,"29 -10 ,-2 \r"
,"29  -8 ,-2 \r"
,"31  -8 ,-2 \r"
,"32  -9 ,-2 \r"
,"36  -7 ,-2 \r"
,"38   0 ,-2 \r"
,"36   5 ,-2 \r"
,"36  -2 ,-2 \r"
,"34  -4 ,-2 \r"
,"32  -4 ,-2 \r"
,"31  -2 ,-2 \r"
,"32   0 ,-2 \r"
,"33  -1 ,-2 \r"
,"34   1 ,-2 \r"
,"33   3 ,-2 \r"
,"30   3 ,-2 \r"
,"26  -3 ,-2 \r"
,"23  -3 ,-2 \r"
,"22  -1 ,-2 \r"
,"23   1 ,-2 \r"
,"24   0 ,-2 \r"
,"25   2 ,-2 \r"
,"24   4 ,-2 \r"
,"22   4 ,-2 \r"
,"20   2 ,-2 \r"
,"20  -5 ,-2 \r"
,"18   0 ,-2 \r"
,"12   0 ,-2 \r"
,"\r"
,"P 12 0 ,2 \r"
,"12  -6 ,2 \r"
,"19  -8 ,2 \r"
,"21 -12 ,2 \r"
,"20 -13 ,2 \r"
,"20 -15 ,2 \r"
,"22 -15 ,2 \r"
,"25 -13 ,2 \r"
,"23 -10 ,2 \r"
,"26 -10 ,2 \r"
,"28 -13 ,2 \r"
,"28 -16 ,2 \r"
,"31 -16 ,2 \r"
,"34 -14 ,2 \r"
,"34 -11 ,2 \r"
,"31 -13 ,2 \r"
,"29 -10 ,2 \r"
,"29  -8 ,2 \r"
,"31  -8 ,2 \r"
,"32  -9 ,2 \r"
,"36  -7 ,2 \r"
,"38   0 ,2 \r"
,"36   5 ,2 \r"
,"36  -2 ,2 \r"
,"34  -4 ,2 \r"
,"32  -4 ,2 \r"
,"31  -2 ,2 \r"
,"32   0 ,2 \r"
,"33  -1 ,2 \r"
,"34   1 ,2 \r"
,"33   3 ,2 \r"
,"30   3 ,2 \r"
,"26  -3 ,2 \r"
,"23  -3 ,2 \r"
,"22  -1 ,2 \r"
,"23   1 ,2 \r"
,"24   0 ,2 \r"
,"25   2 ,2 \r"
,"24   4 ,2 \r"
,"22   4 ,2 \r"
,"20   2 ,2 \r"
,"20  -5 ,2 \r"
,"18   0 ,2 \r"
,"12   0 ,2 \r"
,"P 13 -2 ,2.5 \r"
,"13  -3 ,3 \r"
,"B  pink 13 -3 ,3 \r"
,"14  -4 ,3 \r"
,"P 14 -1 ,2.5 \r"
,"15  -1 ,3 \r"
,"B  pink 15 -1 ,3 \r"
,"16  -2 ,3 \r"
,"\r"
,"P 12   0 ,-2    12   0 ,2 \r"
,"P 12  -6 ,-2    12  -6 ,2 \r"
,"P 19  -8 ,-2    19  -8 ,2 \r"
,"P 21 -12 ,-2    21 -12 ,2 \r"
,"P 20 -13 ,-2    20 -13 ,2 \r"
,"P 20 -15 ,-2    20 -15 ,2 \r"
,"P 22 -15 ,-2    22 -15 ,2 \r"
,"P 25 -13 ,-2    25 -13 ,2 \r"
,"P 23 -10 ,-2    23 -10 ,2 \r"
,"P 26 -10 ,-2    26 -10 ,2 \r"
,"P 28 -13 ,-2    28 -13 ,2 \r"
,"P 28 -16 ,-2    28 -16 ,2 \r"
,"P 31 -16 ,-2    31 -16 ,2 \r"
,"P 34 -14 ,-2    34 -14 ,2 \r"
,"P 34 -11 ,-2    34 -11 ,2 \r"
,"P 31 -13 ,-2    31 -13 ,2 \r" 
,"P 29 -10 ,-2    29 -10 ,2 \r" 
,"P 29  -8 ,-2    29  -8 ,2 \r" 
,"P 31  -8 ,-2    31  -8 ,2 \r" 
,"P 32  -9 ,-2    32  -9 ,2 \r" 
,"P 36  -7 ,-2    36  -7 ,2 \r" 
,"P 38   0 ,-2    38   0 ,2 \r" 
,"P 36   5 ,-2    36   5 ,2 \r" 
,"P 36  -2 ,-2    36  -2 ,2 \r" 
,"P 34  -4 ,-2    34  -4 ,2 \r"
,"P 32  -4 ,-2    32  -4 ,2 \r"
,"P 31  -2 ,-2    31  -2 ,2 \r"
,"P 32   0 ,-2    32   0 ,2 \r"
,"P 33  -1 ,-2    33  -1 ,2 \r"
,"P 34   1 ,-2    34   1 ,2 \r"
,"P 33   3 ,-2    33   3 ,2 \r"
,"P 30   3 ,-2    30   3 ,2 \r"
,"P 26  -3 ,-2    26  -3 ,2 \r"
,"P 23  -3 ,-2    23  -3 ,2 \r"
,"P 22  -1 ,-2    22  -1 ,2 \r"
,"P 23   1 ,-2    23   1 ,2 \r"
,"P 24   0 ,-2    24   0 ,2 \r"
,"P 25   2 ,-2    25   2 ,2 \r"
,"P 24   4 ,-2    24   4 ,2 \r"
,"P 22   4 ,-2    22   4 ,2 \r"
,"P 20   2 ,-2    20   2 ,2 \r"
,"P 20  -5 ,-2    20  -5 ,2 \r"
,"P 18   0 ,-2    18   0 ,2 \r"
,"P 12   0 ,-2    12   0 ,2 \r"
,"\r"
,"P 15  -3     3\r"
," 19.5 -6.5   2\r"
,"23   -6.5   3\r"
,"26   -6.5   4\r"
,"31.5 -6     4\r"
,"33   -6.5   4\r"
,"35   -5.5   4\r"
,"36.5 -2.750 4\r"
,"37    0.167 3\r"
,"36    5     2\r"
,"\r"
,"@finish \r"
};

static char*  egamickdockfoo[] = 
{" \r"
,"\r"
,"@group {foo group} dominant \r"
,"@balllist {foo} color=pink radius= 2.0 nohilite\r" 
,"{FOO} U 0.0 10.0 0.0 \r"
,"\r"
,"@finish \r"
};

static char*  egamickrawfoo[] = 
{" \r"
,"\r"
,"@group {foo} dominant \r"
,"@balllist {foo} color=pink radius= 1.0 nohilite\r" 
,"{FOO} 0.0 0.0 0.0 \r"
,"\r"
,"@finish \r"
}; /*050816*/

/****writehelptostdout()******************************************************/
void writehelptostdout()
{
   int more = 1, j=0, Nth=0; 

         while(more)
         {/*loop over all text for this call*/
            for(j=0; j<255; j++) /*save a space for safety '\0' */
            {/*load transfer string*/
               temps[j] =  magehelpstdout[Nth][j];
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
/*___writehelptostdout()_____________________________________________________*/

/****dohelpintextwin()********************************************************/
void dohelpintextwin(int ihelp)
{
   int more = 0, j=0, Nth=0; 
   if(!Lkinfile) {cleartexts();}
   if(Limittext==0)
   {/*text window not full*/
      more = 1; /*text less than 32768*/
      if(ihelp == 3)
      {/*dump the help menus to the text window*/
         Linternal = 1;
         inserttexts(CRLF);
         inserttexts(CRLF"----------Keyboard Shortcuts----------"CRLF);
         menuaboutsecrets(0);  /*Keyboard Shortcuts*/
         inserttexts(CRLF"----------Input/Output Help----------"CRLF);
         menuaboutwrite(0); /*Output Help*/
         inserttexts(CRLF"----------Screen Capture Help----------"CRLF);
         menuaboutscreenout(0);  /*Screen Output Help*/
         inserttexts(CRLF"----------Editing Help----------"CRLF);
         menuaboutedit(0);  /*Edit Help*/
         inserttexts(CRLF"----------Display Options Help----------"CRLF);
         menuaboutoptions(0);  /*Display Help*/
         inserttexts(CRLF"----------Special Display Help----------"CRLF);
         menuaboutspecial(0);  /*Special Display Help*/
         inserttexts(CRLF"----------Tools Help----------"CRLF);
         menuaboutOther(0); /*Tools Help*/
         inserttexts(CRLF"----------Kludges: More Tools----------"CRLF);
         menuaboutkluges(0);  /*Kludges: More Tools*/
         inserttexts(CRLF"----------Mouse Operations----------"CRLF);
         menuaboutmouse(0);  /*Mouse Operations*/
         inserttexts(CRLF"----Docking and More Mouse Operations----"CRLF);
         menuaboutdocking(0);  /*Mouse Operations*/
         inserttexts(CRLF"----------Window Resizing----------"CRLF);
         menuaboutresize(0);  /*Window Resizing*/
         inserttexts(CRLF"----------Table Help----------"CRLF);
         menuabouttable(0);  /*Table Help*/
         inserttexts(CRLF"----------Bonds Help----------"CRLF);
         menuaboutbonds(0);  /*Bonds Help 130119*/
         inserttexts(CRLF"----------3D Test Help----------"CRLF);
         menuabouttest3D(0);  /*3D Test Help*/
         inserttexts(CRLF"----------NDIM Help----------"CRLF);
         menuaboutNDIM(0);  /*NDIM Help 130119*/
         Linternal = 0; /*restore*/
      }/*dump the help menus to the text window*/
      else
      {/*write text from stored arrays, character-by-character*/

         while(more)
         {/*loop over all text for this call*/
            for(j=0; j<255; j++) /*save a space for safety '\0' */
            {/*load transfer string*/
               if(ihelp == 1)
               {
                  temps[j] =  magekeywords[Nth][j];
               }
               else if(ihelp == 2)
               {
                  temps[j] =  mageobjectparameters[Nth][j];
               }
               else if(ihelp == 4)
               {
                  temps[j] =  magehypertext[Nth][j];
               }
               else if(ihelp == 5)
               {
                  temps[j] =  magedocking[Nth][j];
               }
               else if(ihelp == 6)
               {
                  temps[j] =  mageremote[Nth][j];
               }
               else
               {
                  temps[j] =  magehelp[Nth][j];
               }
               if(temps[j] == '\r')
               {/*ASCII Carraige Return*/
                  temps[j] = EOL; /*platform specific End-Of-Line character*/
               }
               if(temps[j] == '\0'){ break;} /*separate end of text piece*/
            }/*load transfer string*/
            /*check for END before inserttexts() which can reset temps[]*/
            if(temps[0] =='E' && temps[1] =='N' && temps[2] =='D'){more = 0;}
            temps[j+1] = '\0'; /*safety if runs off end of temps str*/
            inserttexts(temps); /*checks and sets Limittext*/
            Nth++; /*local, not static, so will get reset ==0 on new call*/
         }/*loop over all text for this call*/
      }/*write text from stored arrays, character-by-character*/
      /*set Lkinfile so could write out just this text*/
      Lkinfile = 2;
      AdjustMenus();
      sprintf(NameStr,"magetext.txt");
#ifdef MACINTOSH
      /*now do stupid dance to invoke activate window*/
      ShowWindow(captWindow); 
      SelectWindow(captWindow);
      ShowWindow(textWindow); 
      SelectWindow(textWindow);
#endif
   }/*text window not full*/
}
/*___dohelpintextwin()_______________________________________________________*/

/****writehelptohtmlfile()****************************************************/
void writehelptohtmlfile()
{
   int more=0, i=0, j=0, k=0, Nth=0, ihelp=0;
   int nboxes=0, Lbox = 0, n=0, it=0;
   char bigword[512];  /*can expand confused hypertext: <comment> 050915*/

 /*Bryan's DOCTYPE first line  050920 */
  fprintf(fpout,"<!DOCTYPE HTML PUBLIC ");  /*no line feed*/
  fprintf(fpout,"\"-//W3C//DTD HTML 4.01 Transitional//EN\"\n"); /*no gt sign*/
  fprintf(fpout,"     \"http://www.w3.org/TR/html4/loose.dtd\">\n"); /*end gt*/
  fprintf(fpout,"<html>\n");
  fprintf(fpout,"<!--help output written by mage-->\n");
  fprintf(fpout,"<head>\n");
  fprintf(fpout,"   <title>Mage Internal Help</title>\n");
  fprintf(fpout," <style type=\"text/css\">\n");
  fprintf(fpout,"  body {font-family: Verdana, Helvetica, sans-serif&#059;}\n");
  fprintf(fpout,"   p {font-size: 1.0em&#059;}\n"); /*050917 and no h1's ! */
/*050916  p {font-size: 11pt}  scales on Mac but does not scale on PC ???? */
  fprintf(fpout," </style>\n");
  fprintf(fpout,"</head>\n");
  fprintf(fpout,"<body>\n");
  fprintf(fpout,"<h2><a name=\"pt0\">Mage Internal Help:</a></h2>\n");
  fprintf(fpout,"<p>%s</p>\n",version);

   k = 1;  /*over all nboxes, currently 15 130119*/
   nboxes = 15;
  /*begin list of menu help items -------------------------------*/
   fprintf(fpout, "<h3>"); 
  /*menuaboutsecrets();   Keyboard Shortcuts   */
   fprintf(fpout, 
      "<a href=\"#pt%d\">Keyboard Shortcuts</a><br />\n",k++);
  /*menuaboutwrite();     Input/Output Help    */
   fprintf(fpout,
      "<a href=\"#pt%d\">Input/Output Help</a><br />\n",k++);
  /*menuaboutscreenout();      Screen Capture Help   */
   fprintf(fpout,
      "<a href=\"#pt%d\">Screen Capture Help</a><br />\n",k++);
  /*menuaboutedit();      Edit Help            */
   fprintf(fpout,
      "<a href=\"#pt%d\">Editing kinemages</a><br />\n",k++);
  /*menuaboutoptions();   Display Options Help */
   fprintf(fpout,
      "<a href=\"#pt%d\">Fonts, views, and cursors</a><br />\n",k++);
  /*menuaboutspecial();   Special Display Help */
   fprintf(fpout,
      "<a href=\"#pt%d\">Measures, rotations, and updates</a><br />\n",k++);
  /*menuaboutOther();     Tools Help           */
   fprintf(fpout,
      "<a href=\"#pt%d\">Tools Help</a><br />\n",k++);
  /*menuaboutkluges();    Kludges: More Tools  */
   fprintf(fpout,
      "<a href=\"#pt%d\">Kludges and hypertext</a><br />\n",k++);
  /*menuaboutmouse();     Mouse Operations     */
   fprintf(fpout,
      "<a href=\"#pt%d\">Using the mouse</a><br />\n",k++);
  /*menuaboutdocking();   Docking              */
   fprintf(fpout,
      "<a href=\"#pt%d\">Docking</a><br />\n",k++);
  /*menuaboutresize();    Window Resizing      */
   fprintf(fpout,
      "<a href=\"#pt%d\">Resizing windows</a><br />\n",k++);
  /*menuabouttable();     Table Help           */
   fprintf(fpout,
      "<a href=\"#pt%d\">Table Help</a><br />\n",k++);
  /*menuaboutbonds();     Bonds Help   130119        */
   fprintf(fpout,
      "<a href=\"#pt%d\">Bonds Help</a><br />\n",k++);
  /*menuabouttest3D();    3D Test Help         */
   fprintf(fpout,
      "<a href=\"#pt%d\">3D Literacy Test Help</a><br />\n",k++);
  /*menuaboutNDIM();     NDIM Help     130119      */
   fprintf(fpout,
      "<a href=\"#pt%d\">NDIM Help</a><br />\n",k++);
   fprintf(fpout,"<br />\n");
  /*continue with mage internal help sections -------------------------*/
   fprintf(fpout,
      "<a href=\"#pt50\">Kinemage format keywords</a><br />\n");
   fprintf(fpout,
      "<a href=\"#pt60\">Group,Subgroup,List parameters</a><br />\n");
   fprintf(fpout,
      "<a href=\"#pt80\">Hypertext help</a><br />\n");
   fprintf(fpout,
      "<a href=\"#pt90\">Docking help</a><br />\n");
   fprintf(fpout,
      "<a href=\"#pt95\">Remote Pair of Mage Instances help</a><br />\n");
   fprintf(fpout,"</h3>\n");
  /*end of listing of help items -------------------------------------*/

   for(n=1; n<=nboxes; n++)
   {/*loop through all help menu dialog boxes*/
      k = 0; /*for bigword[]*/
      it = 1; /*controls flow in menu-->dlog/html subroutines */
      Lbox = 1; /*to get started into the while loop*/

      while(Lbox)  /*it from 1 to whatever is needed, returns global temps[]*/
      {/*within a box doing it entries*/

         if     (n== 1) {Lbox = menuaboutsecrets(it);}  /*Keyboard Shortcuts*/
         else if(n== 2) {Lbox = menuaboutwrite(it);}    /*Input/Output Help*/
         else if(n== 3) {Lbox = menuaboutscreenout(it);} /*Screen Capture Help*/
         else if(n== 4) {Lbox = menuaboutedit(it);}      /*Editing kinemages*/
         else if(n== 5) {Lbox = menuaboutoptions(it);}/*Fonts,views, & cursors*/
         else if(n== 6) {Lbox = menuaboutspecial(it);}/*Measures,rots,updates*/
         else if(n== 7) {Lbox = menuaboutOther(it);}  /*Tools Help*/
         else if(n== 8) {Lbox = menuaboutkluges(it);} /*Kludges and hypertext*/
         else if(n== 9) {Lbox = menuaboutmouse(it);}  /*Using the mouse*/
         else if(n==10) {Lbox = menuaboutdocking(it);} /*Docking*/
         else if(n==11) {Lbox = menuaboutresize(it);}  /*Resizing windows*/
         else if(n==12) {Lbox = menuabouttable(it);}   /*Table Help*/
         else if(n==13) {Lbox = menuaboutbonds(it);}   /*Bonds Help 130119*/
         else if(n==14) {Lbox = menuabouttest3D(it);}  /*3D Literacy Test Help*/
         else if(n==15) {Lbox = menuaboutNDIM(it);}   /*NDIM Help 130119*/
         else {Lbox = 0;}

         if(Lbox)
         {/*menu dialog box contents*/
            if(it==1) 
            {
               fprintf(fpout,"<h3><a name=\"pt%d\">%s</a>\n",n,word);
               fprintf(fpout,"<a href=\"#pt0\">^TOP^</a></h3>\n");
            }
            fprintf(fpout,"<p>");
            k = 0;
            for(i=0; i<=255; i++) 
            {
               bigword[k] = temps[i];
               if(bigword[k] == '\r' ||bigword[k] == '\n'||bigword[k] == '\0')
               {
                  bigword[k] = '\0';
                  fprintf(fpout,"%s",bigword); /*CRLF may be in body of string*/
                  fprintf(fpout,"<br />\n");
                  if(temps[i] == '\0') break; /*test real string for end*/
                  k = 0;
               }
               else if(bigword[k] == '<')
               {/*expand to avoid confusion with hypertext tag*/
                  bigword[k++] = '&';
                  bigword[k++] = 'l';
                  bigword[k++] = 't';
                  bigword[k++] = ';';
               }
               else if(bigword[k] == '>')
               {/*expand to avoid confusion with hypertext tag*/
                  bigword[k++] = '&';
                  bigword[k++] = 'g';
                  bigword[k++] = 't';
                  bigword[k++] = ';';
               }
               else
               {
                  k++;
               }
            }
            fprintf(fpout,"</p>\n");
            it++;
         }/*menu dialog box contents*/
      }/*within a box doing it entries*/
   }/*loop through all help menu dialog boxes*/

   for(ihelp = 1; ihelp <= 6; ihelp++)
   {/*loop over help sections*/
      if(ihelp==1){fprintf(fpout,"<h3><a name=\"pt50\">keywords</a>\n");}
      if(ihelp==2){fprintf(fpout,"<h3><a name=\"pt60\">parameters</a>\n");}
      if(ihelp==3){continue;}
      if(ihelp==4){fprintf(fpout,"<h3><a name=\"pt80\">hypertext</a>\n");}
      if(ihelp==5){fprintf(fpout,"<h3><a name=\"pt90\">docking</a>\n");}
      if(ihelp==6){fprintf(fpout,"<h3><a name=\"pt95\">remote</a>\n");}
      fprintf(fpout,"<a href=\"#pt0\">^TOP^</a></h3>\n");
      Nth = 0;
      k = 0;  /*for bigword[]  050915*/
      more = 1;
      fprintf(fpout,"<p>"); 
      while(more)
      {/*loop over all text for this call*/
         k = 0;  /*for bigword[]  050915*/
         for(j=0; j<255; j++) /*save a space for safety '\0' */
         {/*load transfer string*/
            if(ihelp == 1)
            {
               temps[j] =  magekeywords[Nth][j];
            }
            else if(ihelp == 2)
            {
               temps[j] =  mageobjectparameters[Nth][j];
            }
            else if(ihelp == 4)
            {
               temps[j] =  magehypertext[Nth][j];
            }
            else if(ihelp == 5)
            {
               temps[j] =  magedocking[Nth][j];
            }
            else if(ihelp == 6)
            {
               temps[j] =  mageremote[Nth][j];
            }
            if(temps[j] == '\r')
            {/*ASCII Carraige Return*/
               temps[j] = EOL; /*platform specific End-Of-Line character*/
            }
            bigword[k] = temps[j]; /*transfer to expandable charstr*/
            if(bigword[k] == '<')
            {/*expand to avoid confusion with hypertext flag*/
               bigword[k++] = '&';
               bigword[k++] = 'l';
               bigword[k++] = 't';
               bigword[k++] = ';';
            }
            else if(bigword[k] == '>')
            {/*expand to avoid confusion with hypertext flag*/
               bigword[k++] = '&';
               bigword[k++] = 'g';
               bigword[k++] = 't';
               bigword[k++] = ';';
            }
            else
            {
               k++;
            }
            if(temps[j] == '\0'){ break;} /*separate end of text piece*/
         }/*load transfer string*/
         /*check for END before fprintf()*/
         if(temps[0] =='E' && temps[1] =='N' && temps[2] =='D'){more = 0;}
         temps[j] = '\0'; /*safety if runs off end of temps str*/
         bigword[k] = '\0';
         fprintf(fpout,"<br />%s",bigword);/*use expandable str 050915*/
         Nth++; 
      }/*loop over all text for this call*/
      fprintf(fpout,"<br />\n</p>"); 
   }/*loop over help sections*/
   fprintf(fpout,"<h3><a href=\"#pt0\">^TOP^</a></h3>\n");

   fprintf(fpout,"</body>\n");
   fprintf(fpout,"</html>\n");
   fclose(fpout);
   fpout = NULL;
}
/*___writehelptohtmlfile()___________________________________________________*/

/****getinternalkinemage()****************************************************/
char getinternalkinemage(int mode)
{
/* #define MAKEPALETTE 1   in MAGEDLOG/createinternalkinemage(int mode) */
/* #define MAKEEXAMPLE 2    */
/* #define MAKEDOCKCUBE 3   */
/* #define MAKEDOCKWIDGET 4 */
/* #define MAKEDOCKMULTI 5  */
/* #define DOCKPLATONICS 5  */
/* #define DOCKTETRAMERS 6  */

   static int Nth=0; /*counts record lines*/
   static int jth=0; /*counts char of each record line*/
   static char check[]={'@','f','i','n','i','s','h'};
   static int iend=0,Lfinish=0;
   char creturn = ' ';
   
   if(mode == 1)
   {
      creturn =  magepalette[Nth][jth];
   }
   else if(mode == 2)
   {
      creturn =  magedemo[Nth][jth];
   }
   else if(mode == 3)
   {
      if(Legamickfoo)
         {creturn =  egamickdockreferencecube[Nth][jth];}  /*egam 030217*/
      else
         {creturn =  egamickdockreferencectrcube[Nth][jth];}
   }
   else if(mode == 4)
   {
      if(Legamickfoo)
         {creturn =  egamickdockrefwidget[Nth][jth];} /*egam 030216*/
      else
         {creturn =  egamickdockrefctrwidget[Nth][jth];}
   }
   else if(mode == 5) /*DOCKPLATONICS 5*/
   {
      if(Legamickfoo)
      {
         if(Negamickshape==0 || Negamickshape==4)
            { creturn =  egamickdockrefwidget[Nth][jth]; }
         else if(Negamickshape==1)
            { creturn =  egamickdockreftetrahedron[Nth][jth]; }
         else if(Negamickshape==2)
            { creturn =  egamickdockreferencecube[Nth][jth]; }
         else if(Negamickshape==3)
            { creturn =  egamickdockrefoctahedron[Nth][jth]; }
         else if(Negamickshape==5)
            { creturn =  egamickdockrefwater[Nth][jth]; }
      }
      else
      {
         if(Negamickshape==0 || Negamickshape==4)
            { creturn =  egamickdockrefctrwidget[Nth][jth]; }
         else if(Negamickshape==1)
            { creturn =  egamickdockrefctrtetrahedron[Nth][jth]; }
         else if(Negamickshape==2)
            { creturn =  egamickdockreferencectrcube[Nth][jth]; }
         else if(Negamickshape==3)
            { creturn =  egamickdockrefctroctahedron[Nth][jth]; }
         else if(Negamickshape==5)
            { creturn =  egamickdockrefctrwater[Nth][jth]; }
      }
   }
   else if(mode == 6) /*DOCKTETRAMERS 6*/
   {
      { creturn =  egamickdockreferencecube[Nth][jth]; }
   }
   else if(mode == 11) 
      { creturn =  egamickdocknexttetrahedron[Nth][jth]; }
   else if(mode == 12)
      { creturn =  egamickdocknextcube[Nth][jth]; }
   else if(mode == 13)
      { creturn =  egamickdocknextoctahedron[Nth][jth]; }
   else if(mode == 14) 
      { creturn =  egamickdocknextwidget[Nth][jth]; }

   else if(mode == 21) 
      { creturn =  egamickdocktetrahedron[Nth][jth]; }
   else if(mode == 22) 
      { creturn =  egamickdockcube[Nth][jth]; }
   else if(mode == 23) 
      { creturn =  egamickdockoctahedron[Nth][jth]; }
   else if(mode == 24) 
      { creturn =  egamickdockwidget[Nth][jth]; }
   else if(mode == 25) /*091127*/
      { creturn =  egamickdockwater[Nth][jth]; } /*091127*/
   
   else if(mode == 31) 
      { creturn =  egamickdockfoo[Nth][jth]; }
   else if(mode == 32) 
      { creturn =  egamickrawfoo[Nth][jth]; }

   else if(mode == 41) 
      { creturn =  egamickdocklion[Nth][jth]; }
   else if(mode == 42) 
      { creturn =  egamickdockcube[Nth][jth]; }
   else if(mode == 43) 
      { creturn =  egamickdockknight[Nth][jth]; }
   else if(mode == 44) 
      { creturn =  egamickdockknave[Nth][jth]; }
   else if(mode == 45) 
      { creturn =  egamickdockcaltrop[Nth][jth]; }
   else if(mode == 46) 
      { creturn =  egamickdockextra[Nth][jth]; }
   else if(mode == 47) 
      { creturn =  egamickdockwall[Nth][jth]; }
   
   jth++; /*set for next character on a line*/
          /*set here, so can reset to 0 if at EOL*/
   /*mickey mouse stuff to reset Nth indicator at end of kinemage*/
   /*  so another one could be done this same run*/
   /*current char number is jth-1 */
   if( ((jth-1) < 7) && (creturn  == check[jth-1]) )
   {/*must get perfect match of first 7 char of line, 0--6*/
      if(iend == 6) 
      {/*6th char==h, @finish recognized, reset line counter*/
       /*but mage parser needs a white space to trigger keyword*/
       /*recognization*/
       Lfinish = 1;
      }
      else
      {
         iend++;
      }
   }
   
   if(Lfinish==1 && creturn==' ')
   {
      /*white space lets MAGEINPT/GetStuffFromFile recognize keyword @finish*/
      /*so will not call getinternalkinemage() again this pass*/
      Nth = 0; /*ready for next kinemage*/
      jth = 0; /*ready for next kinemage*/
      Lfinish = 0;
      creturn = EOF; /*030320*/
   }
   if(creturn == '\r')
   {/*ASCII Carriage Return used in stored kinemage to end a line*/
      jth = 0; /*ready to begin next line*/
      iend = 0;
      if(Lfinish)
      {/*white space for sure*/
         Nth = 0; /*ready for next kinemage*/
         Lfinish = 0;  /*GetStuffFromFile() should not call again*/
         creturn = EOF; /*030320*/
      }
      else
      {
         Nth++; /*next line*/
      }
   }
   Lkinfile = 3; /*020830*/
   return(creturn);
}
/*___getinternalkinemage()___________________________________________________*/


