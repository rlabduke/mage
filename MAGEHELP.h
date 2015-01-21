/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#ifdef  MAGEHELP
#undef  MAGEHELP
#define MAGEHELP
#else
#define MAGEHELP extern
#endif

#ifdef MAGE_ESOTERICA
That will perhaps travel with Mage source masquerading as a header file.

September 2002:

     Avoid making how the scoring is set up, i.e. keep secret the
whole deal of embedded answer keys in the test kinemage itself!
 3D Literacy timed test invoked only from input kinemage:"
 @minutes 5.5"
 @drawline"
 for kinemage timed for 5 minutes and 30 sec."
 At beginning a Dialog box requests a few character for a"
 unique identifier (or short name) for the test taker."
 At end: results of each test automatically written"
 to a uniquely named file with answer as \"new group\"."
 Auto-scoring value is at beginning of kinemage part,"
 and color-coded \"score\" group at kinemage end."
 see results in mage (or wordprocessor)."
SO just say in menuhelp that automatic scoring can be set up.
 Automatic scoring against (hidden) answer list"
 (Can use a completed test as an answer key):"
 @group {answer} dominant nobutton off"
 @vectorlist {answer} answer nobutton off"
 use @drawline with vectorlist"
  @drawlabel  labellist, @drawball  balllist."
 Test mode disables Quit," 
   force progression with KINEMAGE/Score & Next."
 Instructors should try this to really know where answers files are written.
 Between timed kinemages, a Dialog box shows previous score,
 and brief insructions about next test
 (taken from first 255 characters of next caption).
 Untimed practice: scoring set up (answer list provided) but no @minutes"
 Check result with KINEMAGE/Score & Stay."
 Add @drawline,@drawlabel,@drawball,"
 But only one type of answer can be scored."
     Practice mode: new group accummulates your drawing, answer key shows
what it is scored with, score groups successively hold your scored results
as you invoke KINEMAGE/Score & Stay to check your work.  Turn off earlier 
scores to see the latest, turn off new-group to see the color-coded
scoring of the score group, and, of course, turn off the answer key when
you are trying to continue your practice drawing.


March 2002:
     [File/Save Special/Rotated Parts]: invokes a dialog box with option to 
append to the output file to be selected in the following standard file-open 
dialog, instead of always creating a new file.  That file can be written
either in kinemage format or in PDB file format.  Note that Prekin when
setting up rotatable groups arranges the atom list and point ID stuff
such that Mage can recreate viable PDB format for those rotatable atoms.

     [Edit/Show Object Properties]: selected on the last rotatable group,
if set to be deleted, will actually delete that group and recover for
reuse the bondrot sliders and associated internal bond rotation logic.
This only works for the very last rotatable group if there are more than
one, and can only work for the entire group that has the rotation lists.
This feature does allow one to accummulate a set of rotatable sidechains
while trying and deleting new ones in the process.

     Prekin can now produce a kinemage of all one type of aa from a pdb
file superimposed at the origin of their Calphas with Cbeta-ideal up
the y axis and with the yz plane as the mid-plane of tau.
 
     [Edit/Draw New Setup]: has a choice for "measured dihedral labeled" 
such that with both [Edit/Draw New] and [Tools/measures]: selected and both
"Measures" and "Dihedrals" buttons ON, the dihedral value is placed 
in the graphics display at the mid-point of the dihedral mid-line.

     [Edit/Draw New] has a "Balls" button which places a magenta ball 
on the picked point.  This is an easy, although somewhat ugly, way to
mark points for emphasis or get balls into the graphics image for 
later editing.  Magenta seemed the best compromise for either oxygen
or nitrogen atoms, too obviously, Mage really has no built-in 
knowledge about molecules.

     [Edit/Emphasis]: adds a button which when ON sets selected vectors 
to be drawn as wider lines with balls at the ends.  This is a successful
way for Prekin to show rotatable (mutated) residues.  It is otherwise
a rather kludgy prototype for showing selected regions.  A way to select
regions for emphasis, comparisons, superpositioning, etc. is needed, 
but not implemented. 061012 emphasis taken off menu.

     [Edit/Draw New] when Docking scope is active allows 3 on 3 docking.
     Three point docking onto 3 selected points in the reference group(s)
of another 3 points selected in the mobile group(s), is accessed through
[Edit/Draw New] which puts up a dock3on3 button in the side panel.
Activating dock3on3 empowers the next 3 picked points to be references
for the final 3 (6 total) to be moved.  The scheme is the 1st mobile point
of a set of 3 is moved onto the first point in the reference set, the 2nd
is put onto the line defined by reference points 1 and 2, and the 
3rd point is swung around to point toward reference point 3.
The entire set of points in the mobile groups are moved by these rules.
     Mage is oblivious to how these points are actually picked, so they can
be picked on anything in the display, and the docking will be done as if 
they were real points in the reference and mobile groups.

Feb 2002:
     Finally, after many false starts (and denial that it ought to be
done at all) there is a Mickey Mouse auto-animation and auto-rocking
function built into Mage.  It is deliberately buried under 
[Tools/Kludges/Mickey Mouse]: which invokes a dialog box where not only
can various timings and ranges be selected, but the functionality must be
empowered before the "r" key toggles auto-rock, and the Capitol "R" key
toggles auto-animate.  This will allow construction of movie sequences,
but still give the user immediate control of view-point and all mage
devices for changing center, size, etc.  Long animation sequences
using many more groups than there is button space can be shown without
having to keep hitting the animate button or the "a" key.
{The final straw on this camel was placed by Richard Feldmann who
will undoubtedly really use this feature to good effect. 
It joined straws from Homme Hellinga who has been making movies to see things
better shown this way, and Jane who keeps forgetting to keep the image moving
when she is enthusing to someone about the science behind a kinemage.}
{By the way, capital "A" to reverse animation steps was a feature put in
for Charlie Carter.}

     Pickcenter is now set up in single-shot mode.  Activate with the
button on the side panel, pick a point which becomes the new center,
and pickcenter is left unactive.  The [Tools/Find (F)], [Tools/Find Again (G)]
pickcenter, however, remains active until its check box is deselected,
or if a mouse-initiated pick reverts pickcenter to single-shot. 

Years ago:
     Docking has been in and out of Mage at various times.  For many years
it was retained mainly because Len Banazak liked to use it for a class
exercise.  In the late 90's docking was made almost convenient, and
has been slightly polished since then.  recently, there has been a 
dialog box controlling the effective step size invoked by Menu item
[Tools/fine step...] which lets one change both the docking behavior
of slider induced motions and well as the mouse-controlled modes.
Rough docking can thus be done quickly, as well as very fine control 
for really getting bonds to line up exactly.  
     See both [Help Mage/Docking Help] and 
[Help Mage/Help-->text...] "About Mage Docking in text window".

#endif /*MAGE_ESOTERICA*/

