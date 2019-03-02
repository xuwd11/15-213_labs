<html>
  <head>
    <title>Data Lab Release Notes</title>
  </head>

  <body bgcolor=white>
    <h1>Data Lab Release Notes</h1>

<table width=700><tr><td>

<h2>11/1/2018</h2>
<ul>
  <li>Corrected a header comment in the satAdd puzzle.
  <i>Thanks to Prof. Hugh Lauer, Justin Aquilante, and Nick Krichevsky, WPI.</i>
</ul>

<h2>10/24/2018</h2>
<ul>
  <li>Fixed a build bug in the BDD checker.
</ul>

<h2>10/23/2018</h2>
<ul>
  <li>Added some new puzzles, fixed a typo in the comment for the
  rotateRight puzzle, and fixed a build bug in an earlier version.
<i>Thanks to Prof. Hugh Lauer, WPI, for identifying the build bug.</i>
<i>Thanks to Prof. Bryan Dixon, Cal State Chico, for identifying the rotateRight typo.</i>
</ul>

<h2>5/4/2016</h2>
<ul>
<li>Added the <kbd>-std=gnu89</kbd> flag to the dlc Makefile so that gcc 5 will correctly 
compile dlc (addresses a constraint on inline functions introduced in gcc 5).
<i>Thanks to Prof. Branch Archer, West Texas A&M University.</i>
</ul>

<h2>2/17/2016</h2>
<ul>
<li>Added a note to the README that bison and flex must be installed in order to rebuild dlc.
<i>Thanks to Prof. Michael Ross, Portland Community College.</i>
</ul>

<h2>9/16/2014</h2>
<ul>
<li>Applied patch to the ANSI C grammar for dlc to accomodate stricter type rules in more recent versions of bison. 
<li>Eliminated unneccessary calls to bison and flex in the dlc Makefile. 
<i>Thanks to Prof. Robert Marmorstein, Longwood University.</i>
</ul>

<h2>9/2/2014</h2>
<ul>
<li>Fixed bug in <kbd>grade/grade-datalab.pl</kbd> autograding script.
</ul>

<h2>1/30/2014</h2>
<ul>
<li>Fixed comment in code for logicalShift to indicate that '!'
is an allowed operator.
<i>Thanks to Prof. Aran Clauson of Western Washington University</i>
</ul>

<h2>9/19/2012</h2>
<ul>
<li>Fixed bug in line 147, btest.c: incorrect initial value for NaN.
<i>Thanks to Prof. Cary Gray,  Wheaton College</i>
</ul>

<h2>4/26/2012</h2>
<ul>
<li>Added a clarifying note to the main README file reminding
instructors that, because there are different versions of dynamic libraries,
Linux binaries such as dlc are not necessarily portable
across different Linux platforms. 
<i>Thanks to Prof. Hugh Lauer,  Worcester Polytechnic Institute</i>

<li>Cleaned up some compiler warnings in the
<kbd>isLessOrEqual.c</kbd> and <kbd>isPositive.c</kbd> puzzle solutions
</ul>

<h2>8/22/2011</h2>
<ul>

<li>Modified the "start" rule in Makefile to touch the log.txt file
before starting up the lab daemons, so that an empty scoreboard is
created initially. <i>Thanks to Prof. Godmar Back, Virginia Tech.</i>

</ul>

<h2>5/31/2011</h2>
<ul>
<li>For fun, we've added an optional user-level HTTP-based "Beat the
Prof" contest that replaces the old email-based version. The new
contest is very simple to run, is completely self-contained, 
and does not require root
password.  The only requirement is that you have a user account on a
Linux machine with an IP address.

<li> Corrected a few minor typos in various README files.
</ul>

<h2>1/2/2011</h2>
<b>This is a major update of the Data Lab:</b>
<ul>
<li> Introduced floating-point puzzles. 

<li> Added many new integer puzzles (There are now 73 puzzles total). 

<li> Made significant improvements to <kbd>btest</kbd>. It now does
millions of tests for each puzzle, checking wide swaths around Tmin,
0, denorm-normalized boundary, and inf. Also added support for
floating-point puzzles.

<li> Added support for floating-point puzzles to <kbd>dlc</kbd>.

<li> Added a new autograder called <kbd>driver.pl</kbd> that uses
<kbd>dlc</kbd> and <kbd>btest</kbd> to check for correctness and
conformance to the coding guidelines.

<li> Top-level directory now conforms to the CS:APP convention of putting all source files in the ./src directory.

<li> In <kbd>driver.pl</kbd>, replaced "the cp {f1,f2,..,fn} target" notation,
which some shells don't handle, with the more portable "cp f1 f2 ... fn target" form.

<li> The lab writeup is longer included in the
<kbd>datalab-handout</kbd> directory, to allow instructors greater
flexibility in distributing and updating the writeup while the lab is
being offered.  

</ul>

<h2>8/29/2003</h2>
<ul>
<li> Fixed a minor bug that caused <kbd>btest</kbd> to test the
<kbd>tc2sm</kbd> puzzle with an input of <kbd>Tmin</kbd>, which isn't defined
in sign-magnitude.
</ul>

<h2>1/27/2003</h2>
<ul>
<li> More operator-efficient solution to the <kbd>isPower2.c</kbd>
puzzle. <i>Thanks to Al Davis, Univ of Utah.</i>
<li> The <kbd>selections-all.c</kbd> file now lists all 41 puzzles.
</ul>

<h2>9/26/2002</h2>
<ul>
<li> Now includes a prebuilt Linux/IA32 binary for the <kbd>dlc</kbd> compiler.
<li> Fixes a bug (an uninitialized stack variable) that
caused "<kbd>dlc -Z</kbd>" to crash on some systems.

<li> Contains some new scripts for running an interactive 
<a href="http://www.cs.cmu.edu/afs/cs/academic/class/15213-f02/www/daemons/dlcontest.html">"Beat the Prof" contest </a>,<br> 
where students try to outperform the
instructor's (intentionally non-optimal) solution.
</ul>

<h2>6/3/2002</h2>
<ul>
<li> Initial release.
</ul>


</td></tr></table>
</body>
</html>
