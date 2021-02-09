/*
 * tcllogs2html.c -- part of logs2html.mod
 *
 * Written by Fedotov Alexander aka Shmupsik aka shurikvz <shurikvz@mail.ru>
 *
 * 2004-2009 year
 *
 * Updated by pseudo to work on eggdrop 1.8 and Tcl 8.6 (Sep 2010)
 */
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

static void get_log_path(char *channel, char **inputpath) {
	if (Tcl_VarEval(interp, "logfile", (char *)NULL) != TCL_OK || tcl_resultempty()) {
		putlog(LOG_MISC, "*", "Error finding logfiles! Please check settings in your config file to be sure statement \"logfile\" present.");
		return;
	}

	int len = strlen(tcl_resultstring()) + 1;
	char *logslist = nmalloc(len);
	strlcpy(logslist, tcl_resultstring(), len);

	char *p = logslist, *q, *r;
	while ((q = strchr(p, '{')) != NULL) {
		p = strchr(++q, '}');
		(*p) = '\0';
		p++; // now points to next chan in list
		r = newsplit(&q);
		// r = channel modes, check if it has +p flag
		if (logmodes(r) & LOG_PUBLIC) {
			r = newsplit(&q);
			// r = channel name, is it our channel?
			if ((*r == '*') || (!rfc_casecmp(channel, r))) {
				r = newsplit(&q);
				// r = log file name
				int len = strlen(r) + 1;
				(*inputpath) = nmalloc(len);
				strlcpy((*inputpath), r, len);
				break;
			}
		}
	}

	nfree(logslist);
	return;
}


static int tcl_addlogs2htmlchan STDVAR
{
	BADARGS(7, 7, " channel output-path main-page-name main-page-title logs-page-prefix logs-page-title");

	l2hchan_t *newchan = nmalloc(sizeof(struct l2hchan_struct));
	if (newchan == NULL) {
		putlog(LOG_MISC, "*", "Can't allocate enough space to add new channel!");
	    Tcl_AppendResult(irp, "Can't allocate enough space to add new channel!", NULL);
		return TCL_ERROR;
	}

	int len;
	newchan->next = NULL;

	newchan->inputpath = NULL;
	get_log_path(argv[1], &(newchan->inputpath));
	if (newchan->inputpath == NULL) {
		putlog(LOG_MISC, "*", "Can't find expression \"logfile\" for channel \'%s\' with mode \'+p\'.", argv[1]);
		Tcl_AppendResult(irp, "Can't find expression \"logfile\" for channel \'", argv[1], "\' with mode \'+p\'.", NULL);
		nfree(newchan);
		return TCL_OK; //Silently ignore this
	}

	len = strlen(argv[1]) + 1;
	newchan->channame = nmalloc(len);
	strlcpy(newchan->channame, argv[1], len);

	len = strlen(argv[2]) + 1;
	newchan->outputpath = nmalloc(len);
	strlcpy(newchan->outputpath, argv[2], len);

	len = strlen(argv[3]) + 1;
	newchan->mainpagename = nmalloc(len);
	strlcpy(newchan->mainpagename, argv[3], len);

	len = strlen(argv[4]) + 1;
	newchan->mainpagetitle = nmalloc(len);
	strlcpy(newchan->mainpagetitle, argv[4], len);

	len = strlen(argv[5]) + 1;
	newchan->logspagename = nmalloc(len);
	strlcpy(newchan->logspagename, argv[5], len);

	len = strlen(argv[6]) + 1;
	newchan->logspagetitle = nmalloc(len);
	strlcpy(newchan->logspagetitle, argv[6], len);

	int count = 0;
	l2hchan_t *chan = logs2htmlchanlist;
	if (chan == NULL) {
		logs2htmlchanlist = newchan;
	} else {
		count++;
		while (chan->next != NULL) {
			chan = chan->next;
			count++;
		}
		chan->next = newchan;
	}
	count++;

	putlog(LOG_MISC, "*", "  logs2html module: added channel '%s'. Total channels added: %d.", newchan->channame, count);
	Tcl_AppendResult(irp, "Channel added: ", argv[1], ".", NULL);

	return TCL_OK;
}

static tcl_cmds my_tcl_cmds[] = {
	{"addlogs2htmlchan",	tcl_addlogs2htmlchan},
	{NULL,					NULL}	/* Mark end. */
};
