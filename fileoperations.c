/*
 * fileoperations.c -- part of logs2html.mod
 *
 * Written by Fedotov Alexander aka Shmupsik aka shurikvz <shurikvz@mail.ru>
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

#include <stdarg.h>

/****************************************************************************/
/*
 * function FILE *openfile(char *newfilename, const char *mode, bool silent)
 *
 * Input:
 *   newfilename	-	имя файла, который необходимо создать
 *   mode					-	режим открытия файла
 *
 * Output:
 *   указатель на файл
 *
 * Discription:
 *   функция осуществляет создание и открытие файла в указанном режиме
 *   и возвращает указатель на созданный файл
 */
static FILE *openfile(char *filename, const char *mode, bool silent) {
	FILE *file;

	if (filename == NULL) {
		putlog(LOG_MISC, "*", "logs2html: ERROR! Can't allocate enough space for filename.");
		return NULL;
	}

	file = fopen(filename, mode);
	if ((file == NULL) && (!silent)) {
		putlog(LOG_MISC, "*", "logs2html: Warning! Can't open file \"%s\".", filename);
	}

	return file;
} /* openfile() */
/****************************************************************************/


/****************************************************************************/
/*
 * function void writefromexfile(FILE *dst_file, char *exfilename)
 *
 * Input:
 *   
 *   
 *   
 *
 * Output:
 *   
 *
 * Discription:
 *   
 *   
 */
static void writefromexfile(FILE *dst_file, char *exfilename) {
	FILE *addfile;
	char buffer[512];
	size_t n;

	if (strlen(exfilename) > 0) {
		if ((addfile = openfile(exfilename, "r", false)) != NULL) {
			while(!feof(addfile)) {
				n = fread(buffer, sizeof(char), sizeof buffer, addfile);
				fwrite(buffer, sizeof(char), n, dst_file);
			}
			fclose(addfile);
		}
	}

	return;
} /* writefromexfile() */
/****************************************************************************/


/****************************************************************************/
/*
 * function void str_write(FILE *file, char *fmt, ... )
 *
 * Input:
 *   file -	файл в который пишем данные
 *   fmt -	строка для записи со спецификаторами формата
 *   ... -	данные для записи в строку
 *
 * Output:
 *   ничего
 *
 * Discription:
 *   функция осуществляет запись переданной строки в указанный файл,
 *   производя соответствующее ее форматирование
 */
static void str_write(FILE *file, char *fstr, ... ) {
	va_list ap;
	int nchars;
	int size = 256;
	static char *buffer = NULL;

	buffer = (char *)nmalloc(size);

	va_start(ap, fstr);
	while (true) {
		nchars = egg_vsnprintf(buffer, size, fstr, ap);
		if (nchars < size) break;
		size *= 2;
		buffer = (char *)nrealloc(buffer, size);
	}
	va_end(ap);

	fwrite(buffer, sizeof(char), strlen(buffer), file);
	nfree(buffer); buffer = NULL;

	return;
} /* str_write() */
/****************************************************************************/

