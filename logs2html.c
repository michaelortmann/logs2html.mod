/*
 * logs2html.c -- part of logs2html.mod
 *
 * Written by Fedotov Alexander aka Shmupsik aka shurikvz <shurikvz@mail.ru>
 *
 * 2004-2009 year
 *
 * Patch and some changes to v.2.3.4 by skiidoo (2007)
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


#define MODULE_NAME "logs2html"
#define MAKING_LOGS2HTML
#include "src/mod/module.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <math.h>
#include <regex.h>
#include "logs2html.h"
#include "language.h"

#undef global

#define MODULE_MAJOR 2
#define MODULE_MINOR 5
#define MODULE_SUBMINOR 0
#define MODULE_BUILD "0"

static Function *global = NULL;

static int shtime;
static int keep_all_logs;
static char logfile_suffix[21];

static int lines_per_page;
static int start_year;
static int month_block_orientation;
static int dont_print_time;
static int dont_print_join;
static int dont_print_left;
static int dont_print_modechange;
static int dont_print_nickchange;
static int dont_print_kick;
static int dont_print_else;

static int month_cols_count = 3;
static int month_rows_count = 4;

static char mainpage_top_filename[257]		= "\0";
static char mainpage_bottom_filename[257]	= "\0";
static char logspage_top_filename[257]		= "\0";
static char logspage_bottom_filename[257]	= "\0";
static char userstyle_filename[257]			= "\0";
static char encoding_string[31]				= "\0";

static l2hchan_t *logs2htmlchanlist = NULL;		// Here we keep list of our logs

/* for language file */
static char month_names[12][MAX_MONTH_LENGTH];
static char days_names[7][MAX_DAY_LENGTH];

static regex_t re_uri, re_uritrunc, re_email;

static void logs2html_hook_5minutely(void);
static void logs2html_hook_hourly(void);
static void logs2html_hook_rehash(void);
static void logs2html_hook_pre_rehash(void);
static void event_addlogs2htmlchan(void);

#include "fileoperations.c"
#include "htmloperations.c"
#include "tcllogs2html.c"


/*
 * Code starts here
 */

/*
 * getdayofweek()
 * Input:
 *  year	- год
 *  month	- месяц
 *  day		- день
 *
 * Output:
 *  0 - Понедельник, 1 - Вторник, 2 - Среда и.т.д.
 *
 * Description:
 *  Функция вычисляет на какой день недели приходится переданная дата
 */
static int getdayofweek(int year, int month, int day)
{
	long int d1, d2, d3;

	long int tdays = year * 12 + month - 3;
	month = tdays % 12;
	year = (tdays - month) / 12;

	d1 = ((146097 * (year - (year % 100)) / 100) - ((146097 * (year - (year % 100)) / 100) % 4)) / 4;
	d2 = ((1461 * (year % 100)) - ((1461 * (year % 100)) % 4)) / 4;
	d3 = ((153 * month + 2) - ((153 * month + 2) % 5)) / 5 + day + 1721119;

	return (d1 + d2 + d3) % 7;
} /* getdayofweek() */
/****************************************************************************/


/****************************************************************************/
static void makeindexpage(l2hchan_t *ch, int year) {
	/* Compute how many rows we need */
	if (month_cols_count < 0) { month_cols_count = 3; }
	if (month_cols_count > 12) { month_cols_count = 12; }
	month_rows_count = (int)ceil(12.0 / (double)month_cols_count);

	time_t t = time(NULL);
	struct tm tblock = *localtime(&t);

	FILE *file;
	// calculate maximum possible filenamelength
	int filenamelength = strlen(ch->outputpath) + strlen(SEP) + MAX(strlen(ch->mainpagename), strlen(ch->logspagename) + strlen("00") + strlen("00") + strlen("_pg000000")) + strlen("0000") + strlen(".html") + 1;
	char *filename = nmalloc(filenamelength);

	egg_snprintf(filename, filenamelength, "%s%sdefault.css", ch->outputpath, SEP);
	if ((file = openfile(filename, "wb", false)) == NULL) {
		nfree(filename);
		return;
	}
	WriteCSSFile(file);
	fclose(file);

	if (tblock.tm_year + 1900 == year) {
		egg_snprintf(filename, filenamelength, "%s%s%s.html", ch->outputpath, SEP, ch->mainpagename);
	} else {
		egg_snprintf(filename, filenamelength, "%s%s%s%d.html", ch->outputpath, SEP, ch->mainpagename, year);
	}
	if ((file = openfile(filename, "wb", false)) == NULL) {
		nfree(filename);
		return;
	}

	str_write(file, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
	str_write(file, "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	str_write(file, "<head>\n");
	if (strlen(encoding_string) > 0) {str_write(file, "\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\" />\n", encoding_string); }
	str_write(file, "\t<meta name=\"title\" content=\"%s\" />\n", ch->mainpagetitle);
	str_write(file, "\t<meta name=\"description\" content=\"%s\" />\n", ch->mainpagetitle);
	str_write(file, "\t<meta name=\"generator\" content=\"logs2html module for Eggdrop v.%d.%d.%d%s\" />\n", MODULE_MAJOR, MODULE_MINOR, MODULE_SUBMINOR, MODULE_BUILD);
	str_write(file, "\t<meta name=\"author\" content=\"Fedotov Alexander aka shurikvz aka shmupsik\" />\n");
	str_write(file, "\t<meta name=\"keywords\" content=\"logs2html, logs, logging, channel, irc, bot, eggdrop, windrop, mirc, module, convert\" />\n");
	str_write(file, "\t<meta name=\"robots\" content= \"index,all\" />\n");
	str_write(file, "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"default.css\" />\n");
	if (strlen(userstyle_filename) > 0) str_write(file, "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"user.css\" />\n");
	str_write(file, "\t<title>%s %d %s</title>\n", ch->mainpagetitle, year, LOGS2HTML_YEAR);
	str_write(file, "\t<!--[if IE]>\n\t<style type=\"text/css\">\n\t.l2hMainPage #leftsidebar { padding-top: 30px; }\n\t.l2hMainPage #mainblock { zoom: 1; padding-top: 15px; }\n\t/* the above proprietary zoom property gives IE the hasLayout it may need to avoid several bugs */\n\t</style>\n\t<![endif]-->\n");
	str_write(file, "</head>\n\n");

	str_write(file, "<body class=\"l2hMainPage\">\n");
	str_write(file, "\t<div id=\"container\">\n");

	if (strlen(mainpage_top_filename) > 0) {
		str_write(file, "\t\t<div id=\"mp_top\">\n");
		writefromexfile(file, mainpage_top_filename);
		str_write(file, "\n\t\t</div>\n");
	}

	str_write(file, "\t\t<div id=\"header\">\n<h2>%s<br />%d %s</h2>\n\t\t</div>\n", ch->mainpagetitle, year, LOGS2HTML_YEAR);
	str_write(file, "\t\t<div id=\"leftsidebar\">\n");

	int loopyear;
	for(loopyear = start_year; loopyear <= tblock.tm_year + 1900; loopyear++) {
		str_write(file, "\t\t\t<div class=\"indexlink\">");
		if (loopyear == year) {
			str_write(file, "<a href=\"JavaScript:void(0);\">%d</a>", loopyear);
		} else {
			if (tblock.tm_year + 1900 == loopyear) {
				egg_snprintf(filename, filenamelength, "%s.html", ch->mainpagename);
			} else {
				egg_snprintf(filename, filenamelength, "%s%d.html", ch->mainpagename, loopyear);
			}
			str_write(file, "<a href=\"%s\">%d</a>", filename, loopyear);
		}
		str_write(file, "</div>\n");
	}
	str_write(file, "\t\t</div>\n\n");
	str_write(file, "\t\t<div id=\"mainblock\">\n");
	str_write(file, "\t\t\t<table id=\"monthtable\" width=\"100%%\" border=\"0\">\n");

	int i, j, k;
	str_write(file, "\t\t\t\t<colgroup>\n");
	for(k = 0; k < month_cols_count; k++) {
		str_write(file, "\t\t\t\t\t<col width=\"%d%%\" />\n", (int)(100 / month_cols_count));
	}
	str_write(file, "\t\t\t\t</colgroup>\n");

	for(i = 0; i < month_rows_count; i++) {
		str_write(file, "\t\t\t\t<tr>\n");
		for(j = 0; j < month_cols_count; j++) {
			int month = i * month_cols_count + j;
			int row, col;

			str_write(file, "\t\t\t\t\t<td>\n\t\t\t\t\t\t<table class=\"monthblock\" width=\"100%%\">\n");
			str_write(file, "\t\t\t\t\t\t\t<colgroup>\n");
			for(k = 0; k < 7; k++) {
				str_write(file, "\t\t\t\t\t\t\t\t<col width=\"%d%%\" />\n", (int)(100 / 7));
			}
			str_write(file, "\t\t\t\t\t\t\t</colgroup>\n");

			if ((month >= 0) && (month <= 11)) {

				if (month_block_orientation != 0) {
					/*
					 * Horisontal block orientation
					 * Each table has: 1 row: month name, 1 row: day of week names, 6 rows: days of month
					 *                 7 columns: days of month.
					 * Total we must create 8x7 table
					 */
					str_write(file, "\t\t\t\t\t\t\t<tr>\n\t\t\t\t\t\t\t\t<th class=\"monthname\" colspan=\"7\">%s</th>\n\t\t\t\t\t\t\t</tr>\n", month_names[month]);
					str_write(file, "\t\t\t\t\t\t\t<tr>\n\t\t\t\t\t\t\t\t<td class=\"dayname\">%s</td>\n\t\t\t\t\t\t\t\t<td class=\"dayname\">%s</td>\n\t\t\t\t\t\t\t\t<td class=\"dayname\">%s</td>\n\t\t\t\t\t\t\t\t<td class=\"dayname\">%s</td>\n\t\t\t\t\t\t\t\t<td class=\"dayname\">%s</td>\n\t\t\t\t\t\t\t\t<td class=\"dayname\">%s</td>\n\t\t\t\t\t\t\t\t<td class=\"weekend\">%s</td>\n\t\t\t\t\t\t\t</tr>\n",
							 days_names[0], days_names[1], days_names[2], days_names[3], days_names[4], days_names[5], days_names[6]);
					for(row = 0; row < 6; row++) {
						str_write(file, "\t\t\t\t\t\t\t<tr class=\"%s\">\n", (row % 2 == 0) ? "odd" : "even");
						for(col = 0; col < 7; col++) {
							int day = row * 7 + (col + 1) - getdayofweek(year, month + 1, 1);
							if ((day >= 1) && (day <= DAYS_IN_MONTH(year, month))) {
								egg_snprintf(filename, filenamelength, "%s%s%s%d%02d%02d_pg%d.html", ch->outputpath, SEP, ch->logspagename, year, month + 1, day, 1);
								//Lets check if file exist and we can paste link to it. If not exist we try to make it.
								if (!file_readable(filename)) {
									convertfile(ch, year, month + 1, day);
								}
								if (file_readable(filename)) {
									/* let write without full path */
									egg_snprintf(filename, filenamelength, "%s%d%02d%02d_pg%d.html", ch->logspagename, year, month + 1, day, 1);
									str_write(file, "\t\t\t\t\t\t\t\t<td class=\"day\"><a href=\"%s\">%d</a></td>\n", filename, day);
								} else {
									str_write(file, "\t\t\t\t\t\t\t\t<td class=\"day\">%d</td>\n", day);
								}
							} else {
								str_write(file, "\t\t\t\t\t\t\t\t<td class=\"emptycell\">&nbsp;</td>\n");
							}
						}
						str_write(file, "\t\t\t\t\t\t\t</tr>\n");
					}
				} else {
					/*
					 * Vertical block orientation
					 * Each table has: 1 row: month name, 7 rows: (day of week + days of month) names,
					 *                 1 column: day of week names, 6 columns: days of month.
					 * Total we must create 8x7 table
					 */
					str_write(file, "\t\t\t\t\t\t\t<tr>\n\t\t\t\t\t\t\t\t<th class=\"monthname\" colspan=\"7\">%s</th>\n\t\t\t\t\t\t\t</tr>\n", month_names[month]);
					for(row = 0; row < 7; row++) {
						str_write(file, "\t\t\t\t\t\t\t<tr class=\"%s\">\n", (row % 2 == 0) ? "odd" : "even");
						for(col = 0; col < 7; col++) {
							/* First column - day names. */
							if (col == 0) {
								if (row == 6) {
									str_write(file, "\t\t\t\t\t\t\t\t<td class=\"weekend\">%s</td>\n", days_names[row]);
								} else {
									str_write(file, "\t\t\t\t\t\t\t\t<td class=\"dayname\">%s</td>\n", days_names[row]);
								}
								continue;
							}
							int day = (col - 1) * 7 + (row + 1) - getdayofweek(year, month + 1, 1);
							if ((day >= 1) && (day <= DAYS_IN_MONTH(year, month))) {
								egg_snprintf(filename, filenamelength, "%s%s%s%d%02d%02d_pg%d.html", ch->outputpath, SEP, ch->logspagename, year, month + 1, day, 1);
								//Lets check if file exist and we can paste link to it. If not exist we try to make it.
								if (!file_readable(filename)) {
									convertfile(ch, year, month + 1, day);
								}
								if (file_readable(filename)) {
									/* let write without full path */
									egg_snprintf(filename, filenamelength, "%s%d%02d%02d_pg%d.html", ch->logspagename, year, month + 1, day, 1);
									str_write(file, "\t\t\t\t\t\t\t\t<td class=\"day\"><a href=\"%s\">%d</a></td>\n", filename, day);
								} else {
									str_write(file, "\t\t\t\t\t\t\t\t<td class=\"day\">%d</td>\n", day);
								}
							} else {
								str_write(file, "\t\t\t\t\t\t\t\t<td class=\"emptycell\">&nbsp;</td>\n");
							}
						}
						str_write(file, "\t\t\t\t\t\t\t</tr>\n");
					}
				}
			} else {
				/* No more months :) Write empty month block */
				for(row = 0; row < 8; row++) {
					str_write(file, "\t\t\t\t\t\t\t<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");
				}
			}
			str_write(file, "\t\t\t\t\t\t</table>\n\t\t\t\t\t</td>\n");
		}
		str_write(file, "\t\t\t\t</tr>\n");
	}

	str_write(file, "\t\t\t</table>\n");
	str_write(file, "\t\t</div>\n\n");

	str_write(file, "\t\t<br class=\"clearfloat\" />\n");
	str_write(file, "\t\t<div id=\"footer\">\n<p>Generated by logs2html module for eggdrop v.%d.%d.%d%s<br />", MODULE_MAJOR, MODULE_MINOR, MODULE_SUBMINOR, MODULE_BUILD);
	str_write(file, "Find latest version at <a href=\"http://sourceforge.net/projects/logs2html\">http://sourceforge.net/projects/logs2html</a> or <a href=\"http://www.halftone.ru/projects\">http://www.halftone.ru</a><br />");
	str_write(file, "Fork at <a href=\"http://github.com/ac100-ru/logs2html.mod\">http://github.com/ac100-ru/logs2html.mod</a></p>\n\t\t</div>\n");

	if (strlen(mainpage_bottom_filename) > 0) {
		str_write(file, "\t\t<div id=\"mp_bottom\">\n");
		writefromexfile(file, mainpage_bottom_filename);
		str_write(file, "\n\t\t</div>\n");
	}

	str_write(file, "\t</div>\n");
	str_write(file, "</body>\n");
	str_write(file, "</html>\n");

	fclose(file);
	nfree(filename);

	return;
} /* makeindexpage() */
/****************************************************************************/


/****************************************************************************/
/*
 * function void convertfile(l2hchan_t *ch, int year, int month, int day)
 *
 * Input:
 *
 *
 *
 * Output:
 *
 *
 * Description:
 *   переводит файлы из текстового вида в формат HTML
 */
static void convertfile(l2hchan_t *ch, int year, int month, int day) {

	FILE *file, *src_file;
	char buffer[LOGLINELEN], data[LOGLINELEN], ct[81], stamp[33];
	char *buf_ptr;

	struct tm tblock;
	tblock.tm_year	= year - 1900;
	tblock.tm_mon	= month - 1;
	tblock.tm_mday	= day;
	tblock.tm_wday	= getdayofweek(year, month, day);
	tblock.tm_hour	= 0;
	tblock.tm_min	= 0;
	tblock.tm_sec	= 1;
	if (!logfile_suffix[0])
		egg_strftime(ct, 12, ".%d%b%Y", &tblock);
	else
		egg_strftime(ct, 80, logfile_suffix, &tblock);
	ct[80] = '\0';

	// calculate maximum possible filenamelength
	int	filenamelength = MAX(strlen(ch->outputpath), strlen(ch->inputpath)) + MAX(strlen(SEP) + strlen(ch->logspagename) + strlen("0000") + strlen("00") + strlen("00") + strlen("_pg000000") + strlen(".html"), strlen(ct)) + 1;
	char *filename = nmalloc(filenamelength);
	egg_snprintf(filename, filenamelength, "%s%s", ch->inputpath, ct);
	if ((src_file = openfile(filename, "r", true)) == NULL) {
		nfree(filename);
		return;
	}

	if (lines_per_page < 0) lines_per_page = 0;

	int lines_count, tsl;
	int pages_count = 0;
	while(!feof(src_file)) {
		lines_count = lines_per_page;
		pages_count++;

		egg_snprintf(filename, filenamelength, "%s%s%s%d%02d%02d_pg%d.html", ch->outputpath, SEP, ch->logspagename, year, month, day, pages_count);
		if ((file = openfile(filename, "wb", false)) == NULL) {
			fclose(src_file);
			putlog(LOG_MISC, "*", "logs2html: Error occurred on converting %d page of file \"%s\"!", pages_count, filename);
			nfree(filename);
			return;
		}

		str_write(file, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
		str_write(file, "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
		str_write(file, "<head>\n");
		if (strlen(encoding_string) > 0) {str_write(file, "\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\" />\n", encoding_string); }
		str_write(file, "\t<meta name=\"title\" content=\"%s\" />\n", ch->logspagetitle);
		str_write(file, "\t<meta name=\"description\" content=\"%s\" />\n", ch->logspagetitle);
		str_write(file, "\t<meta name=\"generator\" content=\"logs2html module for Eggdrop v.%d.%d.%d%s\" />\n", MODULE_MAJOR, MODULE_MINOR, MODULE_SUBMINOR, MODULE_BUILD);
		str_write(file, "\t<meta name=\"author\" content=\"Fedotov Alexander aka shurikvz aka shmupsik\" />\n");
		str_write(file, "\t<meta name=\"keywords\" content=\"logs2html, logs, logging, channel, irc, bot, eggdrop, windrop, mirc, module, convert\" />\n");
		str_write(file, "\t<meta name=\"robots\" content= \"index,all\" />\n");
		str_write(file, "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"default.css\" />\n");
		if (strlen(userstyle_filename) > 0) str_write(file, "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"user.css\" />\n");
		str_write(file, "\t<title>%s. %d/%d/%d.</title>\n", ch->logspagetitle, day, month, year);
		str_write(file, "\t<!--[if IE]>\n\t<style type=\"text/css\">\n\t.l2hLogPage #mainblock { zoom: 1; padding-top: 15px; }\n\t/* the above proprietary zoom property gives IE the hasLayout it may need to avoid several bugs */\n\t</style>\n\t<![endif]-->\n");
		str_write(file, "</head>\n\n");

		str_write(file, "<body class=\"l2hLogPage\">\n");
		str_write(file, "\t<div id=\"container\">\n");

		if (strlen(logspage_top_filename) > 0) {
			str_write(file, "\t\t<div id=\"lp_top\">\n");
			writefromexfile(file, logspage_top_filename);
			str_write(file, "\n\t\t</div>\n");
		}

		str_write(file, "\t\t<div id=\"header\">\n", ch->mainpagetitle, year, LOGS2HTML_YEAR);
		str_write(file, "\t\t\t<a name=\"top\" id=\"top\"></a>\n");

		str_write(file, "\t\t</div>\n");
		str_write(file, "\t\t<div id=\"mainblock\">\n");

		time_t t = time(NULL);
		tblock = *localtime(&t);
		if (tblock.tm_year + 1900 == year) {
			str_write(file, "\t\t\t<div id=\"navtop\"><a href=\"%s.html\">%s</a></div>\n", ch->mainpagename, LOGS2HTML_MAINPAGE);
		} else {
			str_write(file, "\t\t\t<div id=\"navtop\"><a href=\"%s%d.html\">%s</a></div>\n", ch->mainpagename, year, LOGS2HTML_MAINPAGE);
		}

		while ((lines_count > 0) || (lines_per_page == 0)) {

			buf_ptr = fgets(buffer, sizeof buffer, src_file);
			if (buf_ptr == NULL) break;

			remove_crlf(&buf_ptr);
			if (!buf_ptr[0]) continue;

			/* if timestamp exist cut time from string */
			data[0] = '\0';
			strcat(data, "\t\t\t<div>&nbsp;");
			if (shtime) {
				egg_strftime(stamp, sizeof(stamp) - 1, LOG_TS, &tblock); /* Print dummy time */
				tsl = strlen(stamp);
				if (dont_print_time == 0) {
					strcat(data, "<span class=\"time\">");
					strncat(data, buf_ptr, tsl);
					strcat(data, "</span>&nbsp;");
				}
				buf_ptr += (++tsl);
			}
			if (strncmp(buf_ptr, "--- ", 4) == 0) {  /* we don't really need this string I think */
				continue;
			}

			bool IsElseClass = false;
			if (strncmp(buf_ptr, "Action: ", 8) == 0) {  /* command: /me */
				str_write(file, "%s", data);
				buf_ptr += 7;
				str_write(file, "<span class=\"action\">&nbsp;***");
			} else { /* nick */
				char *p = strstr(buf_ptr, "> ");
				if ((p != NULL) && (strncmp(buf_ptr, "<", 1) == 0)) {
					str_write(file, "%s", data);
					buf_ptr++;
					data[0] = '\0';
					strncat(data, buf_ptr, p - buf_ptr);
					str_write(file, "<span class=\"nick\">&lt;%s&gt;</span><span class=\"text\">&nbsp;", data);
					buf_ptr += (p - buf_ptr + 1);
				} else {
					IsElseClass = true;
					if (strstr(buf_ptr, " joined ") != NULL) {
						if (dont_print_join != 0) {continue; }
						str_write(file, "%s", data);
						str_write(file, "<span class=\"else_j\">&nbsp;");
					} else if (strstr(buf_ptr, " left irc: ") != NULL) {
						if (dont_print_left != 0) {continue; }
						str_write(file, "%s", data);
						str_write(file, "<span class=\"else_li\">&nbsp;");
					} else if (strstr(buf_ptr, " left ") != NULL) {
						if (dont_print_left != 0) {continue; }
						str_write(file, "%s", data);
						str_write(file, "<span class=\"else_l\">&nbsp;");
					} else if (strstr(buf_ptr, "Nick change: ") != NULL) {
						if (dont_print_nickchange != 0) {continue; }
						str_write(file, "%s", data);
						str_write(file, "<span class=\"else_n\">&nbsp;");
					} else if (strstr(buf_ptr, ": mode change '") != NULL) {
						if (dont_print_modechange != 0) {continue; }
						str_write(file, "%s", data);
						str_write(file, "<span class=\"else_m\">&nbsp;");
					} else if (strstr(buf_ptr, " kicked from ") != NULL) {
						if (dont_print_kick != 0) {continue; }
						str_write(file, "%s", data);
						str_write(file, "<span class=\"else_k\">&nbsp;");
					} else {
						if (dont_print_else != 0) {continue; }
						str_write(file, "%s", data);
						str_write(file, "<span class=\"else\">&nbsp;");
					}
				}
			}

			lines_count--;

			char *realloc_buf = NULL;
			replace_chars(buf_ptr, &realloc_buf);
			// We have special chars that we replaced. Now point buf_ptr to the realloc_buf,
			// strip_codes() will now work not with 'buffer' but with 'realloc_buf'
			if (realloc_buf != NULL) {
				buf_ptr = realloc_buf;
			}

			l2hnode_t *tree = NULL;
			strip_codes(buf_ptr, &tree);
			l2hnode_t *ptree, *qtree; ptree = qtree = tree;

			data[0] = '\0';
			int last_tag = 0;
			while (tree != NULL) {
				if (tree->node_so != last_tag) {
					data[0] = '\0';
					strncat(data, buf_ptr + last_tag, tree->node_so - last_tag); last_tag = tree->node_so;
					str_write(file, "%s", data);
				}

				switch (tree->node_type) {
					case NODE_MIRCCOLOR:
						{
							if (tree->node_close) {
								str_write(file, "</span>");
							}
							if (tree->node_data != NULL) {
								str_write(file, "<span class=\"%s\">",  tree->node_data);
							}
						}
						break;
					case NODE_FONTSTYLE:
						{
							if (tree->node_close) {
								str_write(file, "</span>");
							} else {
								str_write(file, "<span style=\"%s\">", tree->node_data);
							}
						}
						break;
					case NODE_EMAIL:
						{
							if (!IsElseClass) { //Don't print emails on join/left, bans etc. Because client mask is look like email.
								if (tree->node_close) {
									str_write(file, "</a>");
								} else {
									str_write(file, "<a href=\"mailto:%s\">", tree->node_data);
								}
							}
						}
						break;
					case NODE_URI:
						{
							if (tree->node_close) {
								str_write(file, "</a>");
							} else {
								str_write(file, "<a href=\"%s\">", tree->node_data);
							}
						}
						break;
					case NODE_SPECIAL:
						{
							str_write(file, "%s", tree->node_data);
						}
						break;
				}

				tree = tree->next;
			}
			str_write(file, "%s</span>", buf_ptr + last_tag);

			str_write(file, "</div>\n");

			// clear memory
			while (qtree != NULL) {
				qtree = ptree->next;
				if (ptree->node_data != NULL) { nfree(ptree->node_data); }
				nfree(ptree);
				ptree = qtree;
			}
			if (realloc_buf != NULL)  {
				nfree(realloc_buf);
			}
		}
		fclose(file);
	}
	fclose(src_file);

	/*
	 * Now, if log is splited to some number of lines (lines_per_page) we write links to other pages, and also final close tags for html files.
	 */
	int i, j;
	for (i = 1; i <= pages_count; i++) {
		egg_snprintf(filename, filenamelength, "%s%s%s%d%02d%02d_pg%d.html", ch->outputpath, SEP, ch->logspagename, year, month, day, i);
		if ((file = openfile(filename, "ab", false)) == NULL) {
			putlog(LOG_MISC, "*", "logs2html: Error occurred on converting %d page of file \"%s\"!", i, filename);
			nfree(filename);
			return;
		}

		str_write(file, "\t\t</div>\n");
		if (pages_count > 1) {
			str_write(file, "\t\t<div id=\"navbottom\">");
			if (i == 1)	{
				str_write(file, LOGS2HTML_BACK);
			} else {
				egg_snprintf(filename, filenamelength, "%s%d%02d%02d_pg%d.html", ch->logspagename, year, month, day, i-1);
				str_write(file, "<span>&larr;</span>&nbsp;<a href=\"%s\">%s</a>", filename, LOGS2HTML_BACK);
			}
			str_write(file, "&nbsp;");
			if (i == pages_count)	{
				str_write(file, LOGS2HTML_NEXT);
			} else {
				egg_snprintf(filename, filenamelength, "%s%d%02d%02d_pg%d.html", ch->logspagename, year, month, day, i+1);
				str_write(file, "<a href=\"%s\">%s</a>&nbsp;<span>&rarr;</span>", filename, LOGS2HTML_NEXT);
			}
			str_write(file, "<br />");
			for (j = 1; j <= pages_count; j++) {
				egg_snprintf(filename, filenamelength, "%s%d%02d%02d_pg%d.html", ch->logspagename, year, month, day, j);
				if (j != i)	{
					str_write(file, "<span><a href=\"%s\">%d</a></span>", filename, j);
				} else {
					str_write(file, "<span id=\"navbottom_currentpg\">%d</span>", j);
				}
			}
			str_write(file, "</div>\n");
		}
		str_write(file, "\t\t<div id=\"totoplink\"><a href=\"#top\">%s</a></div>\n", LOGS2HTML_UP);

		str_write(file, "\t\t<br class=\"clearfloat\" />\n");
		str_write(file, "\t\t<div id=\"footer\">\n<p>Generated by logs2html module for eggdrop v.%d.%d.%d%s<br />", MODULE_MAJOR, MODULE_MINOR, MODULE_SUBMINOR, MODULE_BUILD);
		str_write(file, "Find latest version at <a href=\"http://sourceforge.net/projects/logs2html\">http://sourceforge.net/projects/logs2html</a> or <a href=\"http://www.halftone.ru/projects\">http://www.halftone.ru</a><br />");
		str_write(file, "Fork at <a href=\"http://github.com/ac100-ru/logs2html.mod\">http://github.com/ac100-ru/logs2html.mod</a></p>\n\t\t</div>\n");

		if (strlen(logspage_bottom_filename) > 0) {
			str_write(file, "\t\t<div id=\"lp_bottom\">\n");
			writefromexfile(file, logspage_bottom_filename);
			str_write(file, "\n\t\t</div>\n");
		}

		str_write(file, "\t</div>\n");
		str_write(file, "</body>\n");
		str_write(file, "</html>\n");

		fclose(file);
	}

	nfree(filename);

	return;
} /* convertfile() */
/****************************************************************************/


/****************************************************************************/
static void strip_codes(char *buf_ptr_copy, l2hnode_t **string_tree) {
			/*
			 * void strip_mirc_codes(int flags, char *text)
			 * copied from src/dcc.c and modified
			 *
			 * Copyright (C) 1997 Robey Pointer
			 * Copyright (C) 1999 - 2006 Eggheads Development Team
			 */
			char *dd = buf_ptr_copy;
			char *start = buf_ptr_copy;
			bool t_0x02_opened, t_0x03_opened, t_0x07_opened, t_0x1f_opened;
			t_0x02_opened = t_0x03_opened = t_0x07_opened = t_0x1f_opened = false;

			l2hnode_t *temp_tree = NULL;
			l2hnode_t *newnode;

			char fg_color[3], bg_color[3];
			bg_color[0] = '\0';
			while (*buf_ptr_copy) {
				switch (*buf_ptr_copy) {
					case 0xf:                  /* close all open tags */
						{
							buf_ptr_copy++;
							if (t_0x02_opened) {
								newnode = nmalloc(sizeof(struct l2hnode_struct));
								newnode->next		= NULL;
								newnode->node_type	= NODE_FONTSTYLE;
								newnode->node_so	= dd - start;
								newnode->node_close	= true;
								newnode->node_data = NULL;
								node_append(&temp_tree, newnode);
								t_0x02_opened = false;
							}
							if (t_0x07_opened) {
								newnode = nmalloc(sizeof(struct l2hnode_struct));
								newnode->next		= NULL;
								newnode->node_type	= NODE_FONTSTYLE;
								newnode->node_so	= dd - start;
								newnode->node_close	= true;
								newnode->node_data = NULL;
								node_append(&temp_tree, newnode);
								t_0x07_opened = false;
							}
							if (t_0x1f_opened) {
								newnode = nmalloc(sizeof(struct l2hnode_struct));
								newnode->next		= NULL;
								newnode->node_type	= NODE_FONTSTYLE;
								newnode->node_so	= dd - start;
								newnode->node_close	= true;
								newnode->node_data = NULL;
								node_append(&temp_tree, newnode);
								t_0x1f_opened = false;
							}
							if (t_0x03_opened) {
								newnode = nmalloc(sizeof(struct l2hnode_struct));
								newnode->next = NULL;
								newnode->node_type	= NODE_MIRCCOLOR;
								newnode->node_so	= dd - start;
								newnode->node_data	= NULL;
								newnode->node_close	= true;
								node_append(&temp_tree, newnode);
								t_0x03_opened = false;
								bg_color[0] = '\0';
							}
							continue;
						}
						break;
					case 3:                    /* mIRC colors? */
						{
							buf_ptr_copy++;           /* Skip over the ^C */
							fg_color[0] = '\0';
							if (egg_isdigit(*buf_ptr_copy)) { /* Is the first char a number? */
								fg_color[0] = *buf_ptr_copy;
								fg_color[1] = '\0';
								buf_ptr_copy ++;            /* Skip over the first digit */
								if (egg_isdigit(*buf_ptr_copy)) {     /* Is this a double digit number? */
										fg_color[1] = *buf_ptr_copy;
										fg_color[2] = '\0';
										buf_ptr_copy++;
								}
								// Even if we don't have background color in bg_color we have previous background, and we'll use it
								if (*buf_ptr_copy == ',') {   /* Do we have a background color next? */
									buf_ptr_copy++;           /* Skip over the , */
									if (egg_isdigit(*buf_ptr_copy)) {
										bg_color[0] = *buf_ptr_copy;
										bg_color[1] = '\0';
										buf_ptr_copy ++;            /* Skip over the first digit */
									}
									if (egg_isdigit(*buf_ptr_copy)) {     /* Is this a double digit number? */
										bg_color[1] = *buf_ptr_copy;
										bg_color[2] = '\0';
										buf_ptr_copy++;            /* Is it a double digit? */
									}
								}

								if ((strlen(fg_color) > 0) && ((atoi(fg_color) < 0) || (atoi(fg_color) > 15)))	{
									fg_color[0] = '\0';
								}
								if (((strlen(bg_color) > 0) && ((atoi(bg_color) < 0) || (atoi(bg_color) > 15))) || (strlen(fg_color) == 0)) {
									bg_color[0] = '\0';
								}

								if (strlen(fg_color) > 0) {
									newnode = nmalloc(sizeof(struct l2hnode_struct));
									newnode->next		= NULL;
									newnode->node_type	= NODE_MIRCCOLOR;
									newnode->node_so	= dd - start;
									newnode->node_close	= t_0x03_opened;
									if (strlen(bg_color) > 0) {
										newnode->node_data = nmalloc(6);
										egg_snprintf(newnode->node_data, 6, "c%02.2d%02.2d", atoi(fg_color), atoi(bg_color));
									} else {
										newnode->node_data = nmalloc(4);
										egg_snprintf(newnode->node_data, 4, "f%02.2d", atoi(fg_color));
									}
									node_append(&temp_tree, newnode);
									t_0x03_opened = true;
								}
							} else { /* Single ^C char - it was a close tag */
								newnode = nmalloc(sizeof(struct l2hnode_struct));
								newnode->next = NULL;
								newnode->node_type	= NODE_MIRCCOLOR;
								newnode->node_so	= dd - start;
								newnode->node_data	= NULL;
								newnode->node_close	= t_0x03_opened;
								node_append(&temp_tree, newnode);
								t_0x03_opened = false;
								bg_color[0] = '\0';
							}
							continue;
						}
						break;
					case 2:                    /* Bold text */
						{
							buf_ptr_copy++;
							newnode = nmalloc(sizeof(struct l2hnode_struct));
							newnode->next		= NULL;
							newnode->node_type	= NODE_FONTSTYLE;
							newnode->node_so	= dd - start;
							newnode->node_close	= t_0x02_opened;
							if (t_0x02_opened) {
								newnode->node_data = NULL;
							} else {
								newnode->node_data = nmalloc(strlen("font-weight:bold") + 1);
								strlcpy(newnode->node_data, "font-weight:bold", strlen("font-weight:bold") + 1);
							}
							t_0x02_opened = !t_0x02_opened;
							node_append(&temp_tree, newnode);
							continue;
						}
						break;
					case 7:					/* Italic text */
						{
							buf_ptr_copy++;
							newnode = nmalloc(sizeof(struct l2hnode_struct));
							newnode->next		= NULL;
							newnode->node_type	= NODE_FONTSTYLE;
							newnode->node_so	= dd - start;
							newnode->node_close	= t_0x07_opened;
							if (t_0x07_opened) {
								newnode->node_data = NULL;
							} else {
								newnode->node_data = nmalloc(strlen("font-style:italic") + 1);
								strlcpy(newnode->node_data, "font-style:italic", strlen("font-style:italic") + 1);
							}
							t_0x07_opened = !t_0x07_opened;
							node_append(&temp_tree, newnode);
							continue;
						}
						break;
					case 0x16:                 /* Reverse video */
						{
							buf_ptr_copy++;
							continue;
						}
						break;
					case 0x1f:                 /* Underlined text */
						{
							buf_ptr_copy++;
							newnode = nmalloc(sizeof(struct l2hnode_struct));
							newnode->next		= NULL;
							newnode->node_type	= NODE_FONTSTYLE;
							newnode->node_so	= dd - start;
							newnode->node_close	= t_0x1f_opened;
							if (t_0x1f_opened) {
								newnode->node_data = NULL;
							} else {
								newnode->node_data = nmalloc(strlen("text-decoration:underline") + 1);
								strlcpy(newnode->node_data, "text-decoration:underline", strlen("text-decoration:underline") + 1);
							}
							t_0x1f_opened = !t_0x1f_opened;
							node_append(&temp_tree, newnode);
							continue;
						}
						break;
					case 033:
						{
							buf_ptr_copy++;
							if (*buf_ptr_copy == '[') {
								buf_ptr_copy++;
								while ((*buf_ptr_copy == ';') || egg_isdigit(*buf_ptr_copy)) {
									buf_ptr_copy++;
								}
								if (*buf_ptr_copy) {
									buf_ptr_copy++;             /* also kill the following char */
								}
							}
							continue;
						}
						break;
				}
				*dd++ = *buf_ptr_copy++;            /* Move on to the next char */
			}
			*dd = 0;
			/*
			 * strip_mirc_codes() ends here.
			 */

			//At that point we have temp_tree which points to the linked list of mirc codes and special chars (bold, italic) positions.
			// Now close any unclose tag

			if (t_0x02_opened) {
				newnode = nmalloc(sizeof(struct l2hnode_struct));
				newnode->next		= NULL;
				newnode->node_type	= NODE_FONTSTYLE;
				newnode->node_so	= dd - start;
				newnode->node_close	= true;
				newnode->node_data = NULL;
				node_append(&temp_tree, newnode);
				t_0x02_opened = false;
			}
			if (t_0x07_opened) {
				newnode = nmalloc(sizeof(struct l2hnode_struct));
				newnode->next		= NULL;
				newnode->node_type	= NODE_FONTSTYLE;
				newnode->node_so	= dd - start;
				newnode->node_close	= true;
				newnode->node_data = NULL;
				node_append(&temp_tree, newnode);
				t_0x07_opened = false;
			}
			if (t_0x1f_opened) {
				newnode = nmalloc(sizeof(struct l2hnode_struct));
				newnode->next		= NULL;
				newnode->node_type	= NODE_FONTSTYLE;
				newnode->node_so	= dd - start;
				newnode->node_close	= true;
				newnode->node_data = NULL;
				node_append(&temp_tree, newnode);
				t_0x1f_opened = false;
			}
			if (t_0x03_opened) {
				newnode = nmalloc(sizeof(struct l2hnode_struct));
				newnode->next = NULL;
				newnode->node_type	= NODE_MIRCCOLOR;
				newnode->node_so	= dd - start;
				newnode->node_data	= NULL;
				newnode->node_close	= true;
				node_append(&temp_tree, newnode);
				t_0x03_opened = false;
				bg_color[0] = '\0';
			}


			// rewind to the start of the string
			dd = start;

			int status_uri, status_email;
			regmatch_t pmatch_uri, pmatch_email;

			do {
				status_uri = regexec(&re_uri, dd, (size_t) 1, &pmatch_uri, 0);
				status_email = regexec(&re_email, dd, (size_t) 1, &pmatch_email, 0);

				int method;
				if ((status_email == 0) && (status_uri == 0)) { // We have uri & email in string
					if (pmatch_uri.rm_so == pmatch_email.rm_so) { // I think that it never must be so, but if it is - better lets write it as simple text
						method = 3;
					} else	if (pmatch_uri.rm_so > pmatch_email.rm_so) {
						method = 1;
					} else {
						method = 2;
					}
				} else if ((status_email == 0) && (status_uri != 0)) { // We have only email in string
					method = 1;
				} else if ((status_email != 0) && (status_uri == 0)) { // We have only uri in string
					method = 2;
				} else { // No uri or email in string, simply assign temp_tree to result
					method = 3;
				}

				l2hnode_t *temp;
				if (method == 1) {
					while ((temp_tree != NULL) && (temp_tree->node_so < pmatch_email.rm_so + (dd - start))) {
						temp = temp_tree;
						temp_tree = temp_tree->next;
						temp->next = NULL;
						node_append(string_tree, temp);
					}

					newnode = nmalloc(sizeof(struct l2hnode_struct));
					newnode->next = NULL;
					newnode->node_type = NODE_EMAIL;
					newnode->node_so = pmatch_email.rm_so + (dd - start);
					newnode->node_data = nmalloc(pmatch_email.rm_eo - pmatch_email.rm_so + 1);
					newnode->node_data[0] = '\0';
					strncat(newnode->node_data, dd + pmatch_email.rm_so, pmatch_email.rm_eo - pmatch_email.rm_so);
					newnode->node_close = false;
					node_append(string_tree, newnode);

					while ((temp_tree != NULL) && ((temp_tree->node_so < pmatch_email.rm_eo + (dd - start)) || ((temp_tree->node_so == pmatch_email.rm_eo + (dd - start)) && (temp_tree->node_close == true)))) {
						temp = temp_tree;
						temp_tree = temp_tree->next;
						temp->next = NULL;
						node_append(string_tree, temp);
					}

					newnode = nmalloc(sizeof(struct l2hnode_struct));
					newnode->next = NULL;
					newnode->node_type = NODE_EMAIL;
					newnode->node_so = pmatch_email.rm_eo + (dd - start);
					newnode->node_data = NULL;
					newnode->node_close = true;
					node_append(string_tree, newnode);
					dd += pmatch_email.rm_eo;
					continue;
				} else if (method == 2) {
					while ((temp_tree != NULL) && (temp_tree->node_so < pmatch_uri.rm_so + (dd - start))) {
						temp = temp_tree;
						temp_tree = temp_tree->next;
						temp->next = NULL;
						node_append(string_tree, temp);
					}

					newnode = nmalloc(sizeof(struct l2hnode_struct));
					newnode->next = NULL;
					newnode->node_type = NODE_URI;
					newnode->node_so = pmatch_uri.rm_so + (dd - start);
					// if uri begins with protocol name - simply output our uri, else we must add protocol name to our uri
					if (regexec(&re_uritrunc, dd + pmatch_uri.rm_so, (size_t) 0, NULL, 0) == 0) {
						newnode->node_data = nmalloc(pmatch_uri.rm_eo - pmatch_uri.rm_so + 1);
						newnode->node_data[0] = '\0';
					} else {
						// if uri look like 'ftp.eggheads.org' we add 'ftp://' protocol, else let it be 'http://'
						if (strncmp(dd + pmatch_uri.rm_so, "ftp.", 4) == 0) {
							newnode->node_data = nmalloc(strlen("ftp://") + pmatch_uri.rm_eo - pmatch_uri.rm_so + 1);
							strcpy(newnode->node_data, "ftp://");
						} else {
							newnode->node_data = nmalloc(strlen("http://") + pmatch_uri.rm_eo - pmatch_uri.rm_so + 1);
							strcpy(newnode->node_data, "http://");
						}
					}
					strncat(newnode->node_data, dd + pmatch_uri.rm_so, pmatch_uri.rm_eo - pmatch_uri.rm_so);
					newnode->node_close = false;
					node_append(string_tree, newnode);

					while ((temp_tree != NULL) && ((temp_tree->node_so < pmatch_uri.rm_eo + (dd - start)) || ((temp_tree->node_so == pmatch_uri.rm_eo + (dd - start)) && (temp_tree->node_close == true)))) {
						temp = temp_tree;
						temp_tree = temp_tree->next;
						temp->next = NULL;
						node_append(string_tree, temp);
					}

					newnode = nmalloc(sizeof(struct l2hnode_struct));
					newnode->next = NULL;
					newnode->node_type = NODE_URI;
					newnode->node_so = pmatch_uri.rm_eo + (dd - start);
					newnode->node_data = NULL;
					newnode->node_close = true;
					node_append(string_tree, newnode);
					dd += pmatch_uri.rm_eo;
					continue;
				} else if (method == 3) {
					while (temp_tree != NULL) {
						temp = temp_tree;
						temp_tree = temp_tree->next;
						temp->next = NULL;
						node_append(string_tree, temp);
					}
					break;
				}

			} while ((status_email == 0) || (status_uri == 0));


	return;
}
/****************************************************************************/


/*
 * int list_append(struct list_type **h, struct list_type *i)
 * copied from src/userent.c and modified
 *
 * Copyright (C) 1997 Robey Pointer
 * Copyright (C) 1999 - 2006 Eggheads Development Team
 */
static void node_append(struct l2hnode_struct **h, struct l2hnode_struct *i)
{
  for (; *h; h = &((*h)->next));
  *h = i;
  return;
}
/*
 * list_append() ends here.
 */


/****************************************************************************/
static void replace_chars(char *buf_ptr_copy, char **realloc_buf)
{
	// Loop through string if there are chars, which we can safely replace with their xhtml analog,
	// if there are - count number of bytes we need our string to grow
	int realloc_size = 0;
	char *dd = buf_ptr_copy;

	while (*dd) {
		switch (*dd) {
			case 0x3c:		/* < */
				{
					realloc_size += strlen("&lt;") - strlen("<");
				}
				break;
			case 0x3e:		/* > */
				{
					realloc_size += strlen("&gt;") - strlen(">");
				}
				break;
			case 0x22:		/* " */
				{
					realloc_size += strlen("&quot;") - strlen("""");
				}
				break;
			case 0x26:		/* & */
				{
					realloc_size += strlen("&amp;") - strlen("&");
				}
				break;
		}
		dd++;            /* Move on to the next char */
	}

	if (realloc_size > 0) {
		(*realloc_buf) = nmalloc(strlen(buf_ptr_copy) + realloc_size + 1);
		dd = (*realloc_buf); *dd = '\0';

		while (*buf_ptr_copy) {
			switch (*buf_ptr_copy) {
				case 0x3c:		/* < */
					{
						strcat(dd, "&lt;");
						buf_ptr_copy++; dd += strlen("&lt;");
						continue;
					}
					break;
				case 0x3e:		/* > */
					{
						strcat(dd, "&gt;");
						buf_ptr_copy++; dd += strlen("&gt;");
						continue;
					}
					break;
				case 0x22:		/* " */
					{
						strcat(dd, "&quot;");
						buf_ptr_copy++; dd += strlen("&quot;");
						continue;
					}
					break;
				case 0x26:		/* & */
					{
						strcat(dd, "&amp;");
						buf_ptr_copy++; dd += strlen("&amp;");
						continue;
					}
					break;
			}
			*dd++ = *buf_ptr_copy++;            /* Move on to the next char */
			*dd = '\0';                         /* for strncat(), cause string must be null terminated or we'll get buffer overflow */
		}
		*dd = 0;
	}

	return;
}
/****************************************************************************/


/****************************************************************************/
/*
 * function int cmd_makemainpage(struct userrec *u, int idx, char *par)
 *
 * Input:
 *
 *
 *
 * Output:
 *
 *
 * Description:
 *
 */
static int cmd_makemainpage(struct userrec *u, int idx, char *par) {

	time_t start, finish;
	time(&start);
	putlog(LOG_CMDS, "*", "#%s# begin creation of index pages.", dcc[idx].nick);

	time_t t = time(NULL);
	struct tm tblock = *localtime(&t);

	l2hchan_t *p = logs2htmlchanlist;
	while (p != NULL) {
		int i;

		start_year = MIN(MAX(start_year, 2000), tblock.tm_year + 1900);

		for (i = start_year; i <= tblock.tm_year + 1900; i++) {
			dprintf(idx, " Creating index page for channel '%s' for %d year.\n", p->channame, i);
			makeindexpage(p, i);
		}
		p = p->next;
	}

	putlog(LOG_CMDS, "*", "#%s# creation of index pages finished.", dcc[idx].nick);
	time(&finish);
	double elapsed_time = difftime(finish, start);
	if (elapsed_time == 0.0) {
		putlog(LOG_CMDS, "*", "#%s# It takes < 1 second.", dcc[idx].nick);
	}
	else {
		putlog(LOG_CMDS, "*", "#%s# It takes %f second%s.", dcc[idx].nick, elapsed_time, (elapsed_time != 1.0) ? "s" : "");
	}

	return 0;
} /* cmd_makemainpage() */
/****************************************************************************/


/****************************************************************************/
/*
 * function int convertalllogs(struct userrec *u, int idx, char *par)
 *
 * Input:
 *
 *
 *
 * Output:
 *
 *
 * Description:
 *
 */
static int cmd_convertalllogs(struct userrec *u, int idx, char *par) {

	return cmd_convertlogs(u, idx, "all");
} /* convertalllogs() */
/****************************************************************************/


/****************************************************************************/
static int cmd_convertlogs(struct userrec *u, int idx, char *par) {

	time_t start, finish;
	time(&start);
	putlog(LOG_CMDS, "*", "#%s# begin creation of logfiles.", dcc[idx].nick);

	time_t t = time(NULL);
	struct tm tblock = *localtime(&t);

	l2hchan_t *p = logs2htmlchanlist;

	if (!par[0]) {

		//no parameter. Convert log for all channels for current day.
		while (p != NULL) {
			dprintf(idx, " Converting log for channel '%s' for current day.\n", p->channame);
			convertfile(p, tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday);
			dprintf(idx, " Creating index page for channel '%s' for %d year.\n", p->channame, tblock.tm_year + 1900);
			makeindexpage(p, tblock.tm_year + 1900);
			p = p->next;
		}

	} else {

		int i, j, k;
		start_year = MIN(MAX(start_year, 2000), tblock.tm_year + 1900);
		char *part;

		part = newsplit(&par);
		if (!egg_strncasecmp(part, "all", 3)) { // "all". Convert logs for all channels for all years.

			while (p != NULL) {
				for (i = start_year; i <= tblock.tm_year + 1900; i++) {
					dprintf(idx, " Creating logs for channel '%s' for %d year.\n", p->channame, i);
					for (j = 0; j < 12; j++) {
						for (k = 1; k <= DAYS_IN_MONTH(i, j); k++) {
							convertfile(p, i, j + 1, k);
						}
					}
					dprintf(idx, " Creating index page for channel '%s' for %d year.\n", p->channame, i);
					makeindexpage(p, i);
				}
				p = p->next;
			}
		} else { // Check for year, month, day

			int year, month, day;
			year = atoi(part);
			if ((year < start_year) || (year > tblock.tm_year + 1900)) { // Check if parameter is valid year
				dprintf(idx, " Invalid parameter. 'year' must be between %d and %d.\n", start_year, tblock.tm_year + 1900);
				return 1;
			}
			part = newsplit(&par);
			if (!part[0]) { // Is there second parameter?

				while (p != NULL) {
					dprintf(idx, " Creating logs for channel '%s' for %d year.\n", p->channame, year);
					for (j = 0; j < 12; j++) {
						for (k = 1; k <= DAYS_IN_MONTH(year, j); k++) {
							convertfile(p, year, j + 1, k);
						}
					}
					dprintf(idx, " Creating index page for channel '%s' for %d year.\n", p->channame, year);
					makeindexpage(p, year);
					p = p->next;
				}

			} else { // No second parameter. We have only year

				month = atoi(part);
				if ((month < 1) || (month > 12)) { // Check if parameter is valid month
					dprintf(idx, " Invalid parameter. 'month' must be between %d and %d.\n", 1, 12);
					return 1;
				}
				part = newsplit(&par);
				if (!part[0]) { // Is there third parameter?

					while (p != NULL) {
						dprintf(idx, " Creating logs for channel '%s' for %d year, for %d month.\n", p->channame, year, month);
						for (k = 1; k <= DAYS_IN_MONTH(year, month - 1); k++) {
							convertfile(p, year, month, k);
						}
						dprintf(idx, " Creating index page for channel '%s' for %d year.\n", p->channame, year);
						makeindexpage(p, year);
						p = p->next;
					}

				} else { // No third parameter, we have year and month

					day = atoi(part);
					if ((day < 1) || (day > DAYS_IN_MONTH(year, month - 1))) { // Check if parameter is valid day
						dprintf(idx, " Invalid parameter. 'day' must be between %d and %d.\n", 1, DAYS_IN_MONTH(year, month - 1));
						return 1;
					}

					while (p != NULL) {
						dprintf(idx, " Creating log for channel '%s' for %d year, for %d month, for %d day.\n", p->channame, year, month, day);
						convertfile(p, year, month, day);
						dprintf(idx, " Creating index page for channel '%s' for %d year.\n", p->channame, year);
						makeindexpage(p, year);
						p = p->next;
					}

				}

			}
		}

	}

	putlog(LOG_CMDS, "*", "#%s# creation of logfiles finished.", dcc[idx].nick);
	time(&finish);
	double elapsed_time = difftime(finish, start);
	if (elapsed_time == 0.0) {
		putlog(LOG_CMDS, "*", "#%s# It takes < 1 second.", dcc[idx].nick);
	}
	else {
		putlog(LOG_CMDS, "*", "#%s# It takes %f second%s.", dcc[idx].nick, elapsed_time, (elapsed_time != 1.0) ? "s" : "");
	}
	return 0;
} /* convertlogs() */
/****************************************************************************/


/****************************************************************************/
static void initialize(void)
{
	strlcpy(month_names[0],	LOGS2HTML_JANUARY,		MAX_MONTH_LENGTH);
	strlcpy(month_names[1],	LOGS2HTML_FEBRIARY,		MAX_MONTH_LENGTH);
	strlcpy(month_names[2],	LOGS2HTML_MARCH,		MAX_MONTH_LENGTH);
	strlcpy(month_names[3],	LOGS2HTML_APRIL,		MAX_MONTH_LENGTH);
	strlcpy(month_names[4],	LOGS2HTML_MAY,			MAX_MONTH_LENGTH);
	strlcpy(month_names[5],	LOGS2HTML_JUNE,			MAX_MONTH_LENGTH);
	strlcpy(month_names[6],	LOGS2HTML_JULY,			MAX_MONTH_LENGTH);
	strlcpy(month_names[7],	LOGS2HTML_AUGUST,		MAX_MONTH_LENGTH);
	strlcpy(month_names[8],	LOGS2HTML_SEPTEMBER,	MAX_MONTH_LENGTH);
	strlcpy(month_names[9],	LOGS2HTML_OCTOBER,		MAX_MONTH_LENGTH);
	strlcpy(month_names[10],LOGS2HTML_NOVEMBER,		MAX_MONTH_LENGTH);
	strlcpy(month_names[11],LOGS2HTML_DECEMBER,		MAX_MONTH_LENGTH);
	strlcpy(days_names[0],	LOGS2HTML_MONDAY,		MAX_DAY_LENGTH);
	strlcpy(days_names[1],	LOGS2HTML_TUESDAY,		MAX_DAY_LENGTH);
	strlcpy(days_names[2],	LOGS2HTML_WEDNESDAY,	MAX_DAY_LENGTH);
	strlcpy(days_names[3],	LOGS2HTML_THURSDAY,		MAX_DAY_LENGTH);
	strlcpy(days_names[4],	LOGS2HTML_FRIDAY,		MAX_DAY_LENGTH);
	strlcpy(days_names[5],	LOGS2HTML_SATURDAY,		MAX_DAY_LENGTH);
	strlcpy(days_names[6],	LOGS2HTML_SUNDAY,		MAX_DAY_LENGTH);

	// Precompile our regular expressions
    if (regcomp(&re_uri, "((file|gopher|news|nntp|telnet|http|ftp|https|ftps|sftp)://|www\\.|ftp\\.)+(([a-zA-Z0-9\\._-]+\\.[a-zA-Z]{2,6})|([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}))(/(([a-zA-Z0-9%_\\./=?-]|\\&amp;)*([a-zA-Z0-9%_\\/=?-]|\\&amp;))*)?", REG_EXTENDED|REG_ICASE) != 0) {
		putlog(LOG_CMDS, "*", "logs2html: Error while making regular expression for uri.");
    }
    if (regcomp(&re_uritrunc, "^((file|gopher|news|nntp|telnet|http|ftp|https|ftps|sftp)://){1}", REG_EXTENDED|REG_ICASE|REG_NOSUB) != 0) {
		putlog(LOG_CMDS, "*", "logs2html: Error while making regular expression for truncate uri.");
    }
	if (regcomp(&re_email, "([a-zA-Z]([a-zA-Z0-9_\\-\\.]*[a-zA-Z0-9])*)@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([a-zA-Z0-9\\-]+\\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)", REG_EXTENDED|REG_ICASE) != 0) {
		putlog(LOG_CMDS, "*", "logs2html: Error while making regular expression for email.");
    }

	return;
}
/****************************************************************************/


/****************************************************************************/
static void finalize(void)
{
	l2hchan_t *p, *q;

	p = q = logs2htmlchanlist;
	while (q != NULL) {
		q = p->next;
		if (p->channame != NULL)		nfree(p->channame);
		if (p->inputpath != NULL)		nfree(p->inputpath);
		if (p->outputpath != NULL)		nfree(p->outputpath);
		if (p->mainpagename != NULL)	nfree(p->mainpagename);
		if (p->mainpagetitle != NULL)	nfree(p->mainpagetitle);
		if (p->logspagename != NULL)	nfree(p->logspagename);
		if (p->logspagetitle != NULL)	nfree(p->logspagetitle);
		nfree(p);
		p = q;
	}
	logs2htmlchanlist = p = q = NULL;

	regfree(&re_email);
	regfree(&re_uritrunc);
	regfree(&re_uri);

	return;
}
/****************************************************************************/


/****************************************************************************/
static void logs2html_hook_5minutely(void)
{
	time_t t = time(NULL);
	struct tm tblock = *localtime(&t);
	tblock.tm_hour	= 0;
	tblock.tm_min	= 0;
	tblock.tm_sec	= 1;

	l2hchan_t *p = logs2htmlchanlist;
	while (p != NULL) {
		convertfile(p, tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday);
		makeindexpage(p, tblock.tm_year + 1900);
		p = p->next;
	}

	return;
}
/****************************************************************************/


/****************************************************************************/
static void logs2html_hook_hourly(void)
{
	time_t t = time(NULL);

	struct tm tnow = *localtime(&t);

	t -= (time_t)(60);

	struct tm tblock = *localtime(&t);
	tblock.tm_hour	= 0;
	tblock.tm_min	= 0;
	tblock.tm_sec	= 1;

	//Let's convert log for previous day, because last 5 minutes of that log was not converted
	//by logs2html_hook_5minutely(void) procedure.
	if (tnow.tm_mday != tblock.tm_mday) {
		l2hchan_t *p = logs2htmlchanlist;
		while (p != NULL) {
			convertfile(p, tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday);
			makeindexpage(p, tblock.tm_year + 1900);
			p = p->next;
		}
	}

	//Let's convert logs for previous year, because name of the index page change from prefix.html to prefixPASTYEAR.html
	if (tnow.tm_year != tblock.tm_year) {
		int j, k;

		l2hchan_t *p = logs2htmlchanlist;
		while (p != NULL) {
			for (j = 0; j < 12; j++) {
				for (k = 1; k <= DAYS_IN_MONTH(tblock.tm_year + 1900, j); k++) {
					convertfile(p, tblock.tm_year + 1900, j + 1, k);
				}
			}
			makeindexpage(p, tblock.tm_year + 1900);
			p = p->next;
		}
	}

	return;
}
/****************************************************************************/


/****************************************************************************/
static void logs2html_hook_pre_rehash(void)
{
	finalize();

	return;
}
/****************************************************************************/


/****************************************************************************/
static void logs2html_hook_rehash(void)
{
	initialize();

	return;
}
/****************************************************************************/


static void event_addlogs2htmlchan(void)
{
	check_tcl_event("addlogs2htmlchan");

	return;
}


/*
 * Calculate the memory we keep allocated.
 */
static int logs2html_expmem()
{
	int size = 0;
	l2hchan_t *p = logs2htmlchanlist;
	while (p != NULL) {
		size += sizeof(struct l2hchan_struct);
		if (p->channame != NULL)		size += strlen(p->channame) + 1;
		if (p->inputpath != NULL)		size += strlen(p->inputpath) + 1;
		if (p->outputpath != NULL)		size += strlen(p->outputpath) + 1;
		if (p->mainpagename != NULL)	size += strlen(p->mainpagename) + 1;
		if (p->mainpagetitle != NULL)	size += strlen(p->mainpagetitle) + 1;
		if (p->logspagename != NULL)	size += strlen(p->logspagename) + 1;
		if (p->logspagetitle != NULL)	size += strlen(p->logspagetitle) + 1;
		p = p->next;
	}

	size += MAX_MONTH_LENGTH * 12;
	size += MAX_DAY_LENGTH * 7;

	return size;
}

/* A report on the module status.
 *
 * details is either 0 or 1:
 *    0 - `.status'
 *    1 - `.status all'  or  `.module logs2html'
 */
static void logs2html_report(int idx, int details)
{
	if (logs2htmlchanlist != NULL) {
		l2hchan_t *p = logs2htmlchanlist;

		dprintf(idx, "    logs2html channels list:\n");
		while (p != NULL) {
			dprintf(idx, "     Channel: '%s'.\n", p->channame);
			dprintf(idx, "      path to logfiles: '%s'\n", p->inputpath);
			dprintf(idx, "      path to generated html pages: '%s'.\n", p->outputpath);
			p = p->next;
		}
	}

	if (details) {
		int size = logs2html_expmem();
		dprintf(idx, "    Using %d byte%s of memory\n", size, (size != 1) ? "s" : "");
	}

	return;
}

static cmd_t mydcc[] = {
	{"convertalllogs",	"n",	cmd_convertalllogs,	NULL},
	{"makemainpage",	"n",	cmd_makemainpage,	NULL},
	{"makeindexpage",	"n",	cmd_makemainpage,	NULL},
	{"convertlogs",		"n",	cmd_convertlogs,	NULL},
	{NULL,				NULL,	NULL,				NULL}	/* Mark end. */
};

static tcl_strings my_tcl_strings[] = {
	{"logfile-suffix",		logfile_suffix,				20,		0},
	{"mainpage-top",		mainpage_top_filename,		256,	0},
	{"mainpage-bottom",		mainpage_bottom_filename,	256,	0},
	{"logspage-top",		logspage_top_filename,		256,	0},
	{"logspage-bottom",		logspage_bottom_filename,	256,	0},
	{"user-style",			userstyle_filename,			256,	0},
	{"insert-encoding-str",	encoding_string,			30,		0},
	{NULL,					NULL,						0,		0}	/* Mark end. */
};

static tcl_ints my_tcl_ints[] = {
	{"col-count",				&month_cols_count,			0},
	{"lines-per-page",			&lines_per_page,			0},
	{"start-year",				&start_year,				0},
	{"month-block-orientation",	&month_block_orientation,	0},
	{"dont-print-time",			&dont_print_time,			0},
	{"dont-print-join",			&dont_print_join,			0},
	{"dont-print-left",			&dont_print_left,			0},
	{"dont-print-modechange",	&dont_print_modechange,		0},
	{"dont-print-nickchange",	&dont_print_nickchange,		0},
	{"dont-print-kick",			&dont_print_kick,			0},
	{"dont-print-else",			&dont_print_else,			0},
	{"log-time",				&shtime,					1},
	{"keep-all-logs",			&keep_all_logs,				1},
	{NULL,						NULL,						0}	/* Mark end. */
};


static char *logs2html_close()
{

	del_hook(HOOK_LOADED, (Function) event_addlogs2htmlchan);
	del_hook(HOOK_REHASH, (Function) event_addlogs2htmlchan);
	del_hook(HOOK_REHASH, (Function) logs2html_hook_rehash);
	del_hook(HOOK_PRE_REHASH, (Function) logs2html_hook_pre_rehash);
	del_hook(HOOK_HOURLY, (Function) logs2html_hook_hourly);
	del_hook(HOOK_5MINUTELY, (Function) logs2html_hook_5minutely);

	finalize();

    rem_help_reference(MODULE_NAME".help");
	del_lang_section(MODULE_NAME);
	rem_builtins(H_dcc, mydcc);
    rem_tcl_commands(my_tcl_cmds);
	rem_tcl_strings(my_tcl_strings);
	rem_tcl_ints(my_tcl_ints);

	module_undepend(MODULE_NAME);

	return NULL;
}

EXPORT_SCOPE char *logs2html_start();

static Function logs2html_table[] = {
  (Function) logs2html_start,
  (Function) logs2html_close,
  (Function) logs2html_expmem,
  (Function) logs2html_report,
};

char *logs2html_start(Function *global_funcs)
{
	global = global_funcs;

	/* Register the module. */
	module_register(MODULE_NAME, logs2html_table, MODULE_MAJOR, MODULE_MINOR);

  if (!module_depend(MODULE_NAME, "eggdrop", 108, 4)) {
        module_undepend(MODULE_NAME);
        return "This module requires Eggdrop 1.8.4 or later.";
  }

	add_tcl_ints(my_tcl_ints);
	add_tcl_strings(my_tcl_strings);
	add_tcl_commands(my_tcl_cmds);
	add_builtins(H_dcc, mydcc);
	add_lang_section(MODULE_NAME);
	add_help_reference(MODULE_NAME".help");

	initialize();

	add_hook(HOOK_5MINUTELY, (Function) logs2html_hook_5minutely);
	add_hook(HOOK_HOURLY, (Function) logs2html_hook_hourly);
	add_hook(HOOK_PRE_REHASH, (Function) logs2html_hook_pre_rehash);
	add_hook(HOOK_REHASH, (Function) logs2html_hook_rehash);
	add_hook(HOOK_REHASH, (Function) event_addlogs2htmlchan);
	add_hook(HOOK_LOADED, (Function) event_addlogs2htmlchan);

	return NULL;
}

