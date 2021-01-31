/*
 * logs2html.h -- part of logs2html.mod
 *
 * Written by Fedotov Alexander aka Gray_Angel aka Shmupsik <shurikvz@mail.ru>
 *
 * 2004-2009 year
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

#ifndef _EGG_MOD_LOGS2HTML_LOGS2HTML_H
#define _EGG_MOD_LOGS2HTML_LOGS2HTML_H

#include "src/lang.h"

#define SEP "/"

#ifndef MAX
#define MAX(a,b) \
	({ typeof (a) _a = (a); \
	   typeof (b) _b = (b); \
	   _a > _b ? _a : _b; })
#endif

#ifndef MIN
#define MIN(a,b) \
	({ typeof (a) _a = (a); \
	   typeof (b) _b = (b); \
	   _a < _b ? _a : _b; })
#endif


typedef struct l2hchan_struct {
	struct l2hchan_struct *next;
	char *channame;			/* Имя канала для которого конвертировать логи */
	char *inputpath;		/* Имя (префикс) данного канала (вычисляется из выражения logfile в конфиге бота) */
	char *outputpath;		/* Куда выводить конвертированные логи */
	char *mainpagename;		/* Имя главной страницы */
	char *mainpagetitle;	/* Заголовок на главной странице */
	char *logspagename;		/* Имя (префикс) страницы переконвертированного логфайла */
	char *logspagetitle;	/* Заголовок страницы логфайла */
} l2hchan_t;

typedef enum {NODE_MIRCCOLOR, NODE_FONTSTYLE, NODE_EMAIL, NODE_URI, NODE_SPECIAL} l2h_node;

typedef struct l2hnode_struct {
	struct l2hnode_struct *next;
	l2h_node node_type;
	int node_so;			// Start of special char
	char *node_data;		// Class fo mirc colors
	bool node_close;
} l2hnode_t;

/*
 * For language files. Lets define max number of chars in month and days of week names
 */
#define MAX_MONTH_LENGTH 15
#define MAX_DAY_LENGTH 6

static int days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define DAYS_IN_MONTH(year, month) (days_in_month[month] + (((month == 1) && ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))) ? 1 : 0))


static int cmd_convertalllogs(struct userrec *u, int idx, char *par);
static int cmd_makemainpage(struct userrec *u, int idx, char *par);
static int cmd_convertlogs(struct userrec *u, int idx, char *par);
static int getdayofweek(int year, int month, int day);
static FILE *openfile(char *newfilename, const char *mode, bool silent);
static void str_write(FILE *file, char *fstr, ... );
static void writefromexfile(FILE *dst_file, char *exfilename);
static void makeindexpage(l2hchan_t *ch, int year);
static void convertfile(l2hchan_t *ch, int year, int month, int day);
static void strip_codes(char *buf_ptr_copy, l2hnode_t **tree);
static void node_append(struct l2hnode_struct **h, struct l2hnode_struct *i);
static void replace_chars(char *buf_ptr_copy, char **realloc_buf);


#endif /* _EGG_MOD_LOGS2HTML_LOGS2HTML_H */
