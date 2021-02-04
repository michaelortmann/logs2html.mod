logs2html.mod v.2.4.3 by Alexander Fedotov aka Shmupsik aka shurikvz
e-mail: shurikvz@mail.ru

This module converts the log files of your eggdrop to the html format. It also creates a page-calendar with links to that created files.

*******************************************************************************
To install this module:
 - edit and copy logs2html.conf file to your eggdrop directory
 - for (*nix) copy logs2html.mod to the source directory of your eggdrop (src/mod), then as usual: make iconfig && make && make install)
 - for windows:
	- copy the contents of "language" directory to your windrop "language" directory
	- copy the contents of "help" directory to your windrop "help" directory
	- copy *.dll (/precompiled) to windrop "modules" directory
 - if you decide to use your own style for pages edit and copy user.css file to your logs output directory
 - put string "source logs2html.conf" into your eggdrop config file and restart your bot

Module has 4 commands, which you can use from dcc chat:
 - convertalllogs			- reconverts all your logs
 - convertlogs [year [month [day]]]	- reconverts your logs
 - makemainpage				- redraws mainpages
 - makeindexpage			- same as ".makemainpage"
*******************************************************************************



*******************************************************************************
You can try to find latest version on http://sourceforge.net/projects/logs2html or http://www.halftone.ru/projects

pseudo's updates are available at http://eggdrop.egg6.net/modules/
*******************************************************************************



*******************************************************************************
Version history:
25.09.10 - version 2.4.3-pseudo
  - updated dependencies to eggdrop 1.8.x
  - removed direct access of interp->result to fix compatibility with Tcl 8.6

28.05.09 - version 2.4.3
  - now correctly output '"' and '&' char to html
  - improved search of uri and email
  - added css classes for join/left, mode change, nick change and kick events
  - added an option to not display these events in the resulting html file


12.05.09 - version 2.4.2
  - fixed error with compilation on eggdrop 1.6.19 with Suzi-patch sp.0009
  - now automaticly reconvert logs for previous year, when the year changed (i.e. on december, 31)


08.05.09 - version 2.4.1
  - fixed display bug with firefox browser


24.04.09 - version 2.4.0
  - now module can convert and show logs for several years (new command added, see '.help logs2html module')
  - the output pages is now in xhtml format (see "known problems" for details)
  - added dont-print-time parameter
  - added month-block-orientation parameter
  - some color-names fixed in default css file


15.10.05 - version 2.3.4
  - added setting in config file, which allow to change value of meta tag <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=..."> in generated HTML files.


04.10.05 - version 2.3.3
  - fixed bug with broken e-mail (strings with broken e-mails were truncated)


 24.09.05 - version 2.3.2
  - added support for <B> and <U> tags
  - fixed bug with [%H:%M:%S] timestamp (I hope.. :-))


 26.07.05 - version 2.3.1
  - bug fixed


 24.07.05 - version 2.3.0
  - page style discriptions moved to CSS file
  - no need for tcl now
  - small bugs fixed


 25.04.05 - version 2.1.0
  - added support for http links and e-mail.


 27.02.05 - version 2.0
  - converting logs for multiply channels
  - possibility to change colors of HTML pages
  - now you can aim number of lines on the one generated HTML page


 16.12.04 - version 1.0
  - initial realise
*******************************************************************************



*******************************************************************************
KNOWN PROBLEMS


*******************************************************************************



P.S. Sorry for my bad english
