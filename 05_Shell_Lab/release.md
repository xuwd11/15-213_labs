<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<!-- saved from url=(0045)http://csapp.cs.cmu.edu/3e/shlab-release.html -->
<html>

  <body bgcolor="white">
    <h1>Shell Lab Release Notes</h1>

<table width="700"><tbody><tr><td>

<h2>07/28/2003</h2>
<ul>
<li> Fixed a race condition in the reference solution that happened
during the creation of a new job. The race existed between (1) the
parent adding a new job to the job list and (2) the receipt of a
SIGINT or SIGTSTP signal.  The fix is to block SIGINT and SIGTSTP
signals in the parent until after the new job has been added to the
job list. A similar race was already properly handled for SIGCHLD
signals.

<p>
</p></li><li> Also made some minor changes to the reference solution:
<ul>
<li> Changed the command line parser supplied to the students
so that characters surrounded by single quotes are treated as
a single argument. This allows the tiny shell to handle commands
such as 
<pre>tsh&gt; /bin/sh -c 'ps | fgrep mysplit'
</pre>
</li><li> Made a slight change to the formatting of job list output.
</li></ul>
</li></ul>

<h2>12/05/2002</h2>
<ul>
<li> Added a new trace file (trace16.txt) that tests the shell's
ability to handle SIGINT and SIGTSTP signals that do not originate
from the terminal.
</li></ul>

<h2>10/16/2002</h2>
<ul>
<li> Fixed bug in tracefile <kbd>src/trace10.txt</kbd>, 
which told the driver to sleep too long before running the <kbd>fg</kbd> command.

</li><li> Removed assumptions in <kbd>src/Makefile</kbd> and <kbd>grade/Makefile</kbd> that the user had a "." in their Unix path.
</li></ul>

<h2>6/3/2002</h2>
<ul>
<li> Initial release.
</li></ul>


</td></tr></tbody></table>


</body></html>
