<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<!-- saved from url=(0049)http://csapp.cs.cmu.edu/3e/malloclab-release.html -->
<html>

  <body bgcolor="white">
    <h1>Malloc Lab Release Notes</h1>

<table width="700"><tbody><tr><td>

<h2>9/2/2014</h2>
<ul>
<li> Fixed bug in <kbd>grade-malloclab.pl</kbd> autograder that relied
on <kbd>bash</kbd> syntax that is incompatible with <kbd>sh</kbd>.
<i>Thanks to Prof. Hugh Lauer, Worcester Polytechnic Institute</i>
</li></ul>

<h2>11/30/2010</h2>
<ul>
<li> Fixed a 1-line bug in the <kbd>mm-implicit.c</kbd>
and <kbd>mm-explicit.c</kbd> example programs that improperly tested
the error code returned by <kbd>mem_sbrk</kbd>. Some of the newer
versions of libc <kbd>malloc</kbd> allocate in higher memory addresses
that tickle the bug (Another one of those bugs that we're always
warning the students about...sigh).
<i>Thanks to Prof. Paul Haiduk, West Texas A&amp;M University.</i>

</li><li> The <kbd>mm-implicit.c</kbd> and <kbd>mm-explicit.c</kbd> example
programs are not 64-bit clean. Added the <kbd>-m32</kbd> flag to
Makefile and Makefile-handout to reflect this.

</li></ul>
<h2>4/28/2004</h2>
<ul>
<li> Fixed an off-by-one memory overwrite in the processing for the -f argument (the kind we're always warning the students about...sigh).
<i>Thanks to Prof. John Regehr, University of Utah.</i> 

</li><li> In <kbd>config.h</kbd>, <kbd>#ifndef __CONFIG_</kbd> was 
incorrectly spelled <kbd>#ifndef __CONGIF_</kbd>. 

</li></ul>

<h2>7/14/2003</h2>
<ul>
<li> Minor change to fix the rule that builds the Latex writeup pages.
</li></ul>

<h2>6/3/2002</h2>
<ul>
<li> Initial release.
</li></ul>

</td></tr></tbody></table>


</body></html>
