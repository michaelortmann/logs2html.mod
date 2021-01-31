/*
 * htmloperations.c -- part of logs2html.mod
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

/****************************************************************************/
/*
* function void writecss
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
static void WriteCSSFile(FILE *dst_file)
{
	str_write(dst_file, "@charset \"utf-8\";\n");
	str_write(dst_file, "/* CSS Document */\n\n");

	str_write(dst_file, "<!--\n");
	str_write(dst_file, "body  {\n");
	str_write(dst_file, "font-family:\"Times New Roman\", Times, serif;\n");
	str_write(dst_file, "background: #666666;\n");
	str_write(dst_file, "margin: 0;\n");
	str_write(dst_file, "padding: 0;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "color: #000000;\n");
	str_write(dst_file, "}\n\n");

	str_write(dst_file, "/* mainpage styles */\n");
	str_write(dst_file, ".l2hMainPage #container {\n");
	str_write(dst_file, "width: 99%%;\n");
	str_write(dst_file, "background:#FFFF99;\n");
	str_write(dst_file, "margin: 0 auto;\n");
	str_write(dst_file, "border: 1px solid #000000;\n");
	str_write(dst_file, "text-align: left;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #container #monthtable tr {\n");
	str_write(dst_file, "background: Orange;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #container .monthblock .monthname {\n");
	str_write(dst_file, "font-family:\"Comic Sans MS\", \"Brush Script Std\", Verdana, Arial, Helvetica, sans-serif;\n");
	str_write(dst_file, "font-size: 1.3em;\n");
	str_write(dst_file, "height: 1.8em;\n");
	str_write(dst_file, "background:#FF6600;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "color: white;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #container #monthtable .monthblock .dayname {\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "height: 1.8em;\n");
	str_write(dst_file, "background: Silver;\n"); 
	str_write(dst_file, "text-align: left;\n");
	str_write(dst_file, "padding: 0 5px;\n");
	str_write(dst_file, "color:#333333;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #container #monthtable .monthblock .weekend {\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "height: 1.8em;\n");
	str_write(dst_file, "background: #FF6633;\n");
	str_write(dst_file, "text-align: left;\n");
	str_write(dst_file, "padding: 0 5px;\n");
	str_write(dst_file, "color:#333333;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #container #monthtable .monthblock .emptycell {\n");
	str_write(dst_file, "height: 1.8em;\n");
	str_write(dst_file, "background:#DDDDDD;\n"); 
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #container #monthtable .monthblock .day {\n");
	str_write(dst_file, "font-family:\"Courier New\", Courier, monospace;\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "height: 1.8em;\n");
	str_write(dst_file, "background:#FFCC00;\n"); 
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "color:#FF0000;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #header {\n");
	str_write(dst_file, "background: #DDDDDD;\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #header h2 {\n");
	str_write(dst_file, "font-style:italic;\n");
	str_write(dst_file, "margin: 0;\n");
	str_write(dst_file, "padding: 10px 0;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #mp_top {\n");
	str_write(dst_file, "background: #DDDDDD;\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #mp_bottom {\n");
	str_write(dst_file, "background: #DDDDDD;\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "}\n\n");

	str_write(dst_file, "/* leftsidebar styles */\n");
	str_write(dst_file, ".l2hMainPage #leftsidebar {\n");
	str_write(dst_file, "float: left;\n");
	str_write(dst_file, "width: 7em;\n");
	str_write(dst_file, "background: #EBEBEB;\n");
	str_write(dst_file, "padding: 10px 0;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #leftsidebar div.indexlink {\n");
	str_write(dst_file, "background-color: #EBEBEB;\n");
	str_write(dst_file, "margin-left: 5px;\n");
	str_write(dst_file, "margin-right: 5px;\n");
	str_write(dst_file, "text-align:center;\n");
	str_write(dst_file, "border-bottom-width: thin;\n");
	str_write(dst_file, "border-bottom-style: dotted;\n");
	str_write(dst_file, "border-bottom-color: #CCCCCC;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #leftsidebar div.indexlink a {\n");
	str_write(dst_file, "display:block;\n");
	str_write(dst_file, "letter-spacing:.1em;\n");
	str_write(dst_file, "line-height:1.5em;\n");
	str_write(dst_file, "text-decoration: none;\n");
	str_write(dst_file, "vertical-align:baseline;\n");
	str_write(dst_file, "color:#333333;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #leftsidebar div.indexlink a:hover {\n");
	str_write(dst_file, "background:#FF6600;\n");
	str_write(dst_file, "letter-spacing:.3em;\n");
	str_write(dst_file, "line-height:1.5em;\n");
	str_write(dst_file, "color:#FFFFFF;\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #mainblock {\n");
	str_write(dst_file, "margin: 0 20px 0 8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #footer {\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "background:#DDDDDD;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hMainPage #footer p {\n");
	str_write(dst_file, "margin: 0;\n");
	str_write(dst_file, "padding: 10px 0;\n");
	str_write(dst_file, "font-size: 0.9em;\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "color: #333333;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #lp_top {\n");
	str_write(dst_file, "background: #DDDDDD;\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #lp_bottom {\n");
	str_write(dst_file, "background: #DDDDDD;\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "}\n\n");

	str_write(dst_file, "/* Log Page styles */\n");
	str_write(dst_file, ".l2hLogPage #header {\n");
	str_write(dst_file, "background: #DDDDDD;\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container {\n");
	str_write(dst_file, "width: 99%%;\n");
	str_write(dst_file, "background: #FFFF99;\n");
	str_write(dst_file, "margin: 0 auto;\n");
	str_write(dst_file, "border: 1px solid #000000;\n");
	str_write(dst_file, "text-align: left;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container #navtop {\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "border-bottom: 1px solid Orange;\n");
	str_write(dst_file, "margin-bottom: 10px;\n");
	str_write(dst_file, "padding: 10px 0;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container #navtop a {\n");
	str_write(dst_file, "color:#FF0000;\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container #totoplink {\n");
	str_write(dst_file, "color:#FF0000;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "border-top: 1px solid Orange;\n");
	str_write(dst_file, "margin-top: 10px;\n");
	str_write(dst_file, "padding: 10px 0;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container #totoplink a {\n");
	str_write(dst_file, "color:#FF0000;\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container #navbottom {\n");
	str_write(dst_file, "color:#FF0000;\n");
	str_write(dst_file, "text-align: center;\n");
	str_write(dst_file, "padding: 10px;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container #navbottom span {\n");
	str_write(dst_file, "padding-left:5px;\n");
	str_write(dst_file, "padding-right:5px;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #container #navbottom #navbottom_currentpg {\n");
	str_write(dst_file, "font-size:1.1em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #footer {\n");
	str_write(dst_file, "padding: 0 10px;\n");
	str_write(dst_file, "background:#DDDDDD;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #footer p {\n");
	str_write(dst_file, "margin: 0;\n");
	str_write(dst_file, "padding: 10px 0;\n");
	str_write(dst_file, "font-size: 0.9em;\n");
	str_write(dst_file, "font-weight:bold;\n");
	str_write(dst_file, "color: #333333;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock div {\n");
	str_write(dst_file, "white-space: pre-wrap; /* css-3 */\n");
	str_write(dst_file, "white-space: pre-line; /* css-3, css-2.1 */\n");
	str_write(dst_file, "white-space: -moz-pre-wrap !important; /* Mozilla, since 1999 */\n");
	str_write(dst_file, "white-space: -hp-pre-wrap; /* HP printers */\n");
	str_write(dst_file, "white-space: -pre-wrap; /* Opera 4-6 */\n");
	str_write(dst_file, "white-space: -o-pre-wrap; /* Opera 7 */\n");
	str_write(dst_file, "word-wrap: break-word; /* Internet Explorer 5.5+ */\n");
	str_write(dst_file, "overflow:hidden;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.time {\n");
	str_write(dst_file, "color: #D0D0D0;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.nick {\n");
	str_write(dst_file, "color: #0040C0;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.text {\n");
	str_write(dst_file, "color: #111111;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.else_j {\n");
	str_write(dst_file, "color: #99CC00;\n");
	str_write(dst_file, "font-size: 0.8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.else_li {\n");
	str_write(dst_file, "color: #99CC00;\n");
	str_write(dst_file, "font-size: 0.8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.else_l {\n");
	str_write(dst_file, "color: #99CC00;\n");
	str_write(dst_file, "font-size: 0.8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.else_n {\n");
	str_write(dst_file, "color: #FF6666;\n");
	str_write(dst_file, "font-size: 0.8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.else_m {\n");
	str_write(dst_file, "color: #00CCFF;\n");
	str_write(dst_file, "font-size: 0.8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.else_k {\n");
	str_write(dst_file, "color: #FF0066;\n");
	str_write(dst_file, "font-size: 0.8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.else {\n");
	str_write(dst_file, "color: #40C000;\n");
	str_write(dst_file, "font-size: 0.8em;\n");
	str_write(dst_file, "}\n");
	str_write(dst_file, ".l2hLogPage #mainblock span.action {\n");
	str_write(dst_file, "color: #FF99FF;\n");
	str_write(dst_file, "}\n\n");

	str_write(dst_file, "/* mirc colors */\n");
	str_write(dst_file, ".l2hLogPage span.c0000, .l2hLogPage span.c0100, .l2hLogPage span.c0200, .l2hLogPage span.c0300, .l2hLogPage span.c0400, .l2hLogPage span.c0500, .l2hLogPage span.c0600, .l2hLogPage span.c0700, .l2hLogPage span.c0800, .l2hLogPage span.c0900, .l2hLogPage span.c1000, .l2hLogPage span.c1100, .l2hLogPage span.c1200, .l2hLogPage span.c1300, .l2hLogPage span.c1400, .l2hLogPage span.c1500 {background-color: white;}\n");
	str_write(dst_file, ".l2hLogPage span.c0001, .l2hLogPage span.c0101, .l2hLogPage span.c0201, .l2hLogPage span.c0301, .l2hLogPage span.c0401, .l2hLogPage span.c0501, .l2hLogPage span.c0601, .l2hLogPage span.c0701, .l2hLogPage span.c0801, .l2hLogPage span.c0901, .l2hLogPage span.c1001, .l2hLogPage span.c1101, .l2hLogPage span.c1201, .l2hLogPage span.c1301, .l2hLogPage span.c1401, .l2hLogPage span.c1501 {background-color: black;}\n");
	str_write(dst_file, ".l2hLogPage span.c0002, .l2hLogPage span.c0102, .l2hLogPage span.c0202, .l2hLogPage span.c0302, .l2hLogPage span.c0402, .l2hLogPage span.c0502, .l2hLogPage span.c0602, .l2hLogPage span.c0702, .l2hLogPage span.c0802, .l2hLogPage span.c0902, .l2hLogPage span.c1002, .l2hLogPage span.c1102, .l2hLogPage span.c1202, .l2hLogPage span.c1302, .l2hLogPage span.c1402, .l2hLogPage span.c1502 {background-color: navy;}\n");
	str_write(dst_file, ".l2hLogPage span.c0003, .l2hLogPage span.c0103, .l2hLogPage span.c0203, .l2hLogPage span.c0303, .l2hLogPage span.c0403, .l2hLogPage span.c0503, .l2hLogPage span.c0603, .l2hLogPage span.c0703, .l2hLogPage span.c0803, .l2hLogPage span.c0903, .l2hLogPage span.c1003, .l2hLogPage span.c1103, .l2hLogPage span.c1203, .l2hLogPage span.c1303, .l2hLogPage span.c1403, .l2hLogPage span.c1503 {background-color: green;}\n");
	str_write(dst_file, ".l2hLogPage span.c0004, .l2hLogPage span.c0104, .l2hLogPage span.c0204, .l2hLogPage span.c0304, .l2hLogPage span.c0404, .l2hLogPage span.c0504, .l2hLogPage span.c0604, .l2hLogPage span.c0704, .l2hLogPage span.c0804, .l2hLogPage span.c0904, .l2hLogPage span.c1004, .l2hLogPage span.c1104, .l2hLogPage span.c1204, .l2hLogPage span.c1304, .l2hLogPage span.c1404, .l2hLogPage span.c1504 {background-color: red;}\n");
	str_write(dst_file, ".l2hLogPage span.c0005, .l2hLogPage span.c0105, .l2hLogPage span.c0205, .l2hLogPage span.c0305, .l2hLogPage span.c0405, .l2hLogPage span.c0505, .l2hLogPage span.c0605, .l2hLogPage span.c0705, .l2hLogPage span.c0805, .l2hLogPage span.c0905, .l2hLogPage span.c1005, .l2hLogPage span.c1105, .l2hLogPage span.c1205, .l2hLogPage span.c1305, .l2hLogPage span.c1405, .l2hLogPage span.c1505 {background-color: maroon;}\n");
	str_write(dst_file, ".l2hLogPage span.c0006, .l2hLogPage span.c0106, .l2hLogPage span.c0206, .l2hLogPage span.c0306, .l2hLogPage span.c0406, .l2hLogPage span.c0506, .l2hLogPage span.c0606, .l2hLogPage span.c0706, .l2hLogPage span.c0806, .l2hLogPage span.c0906, .l2hLogPage span.c1006, .l2hLogPage span.c1106, .l2hLogPage span.c1206, .l2hLogPage span.c1306, .l2hLogPage span.c1406, .l2hLogPage span.c1506 {background-color: purple;}\n");
	str_write(dst_file, ".l2hLogPage span.c0007, .l2hLogPage span.c0107, .l2hLogPage span.c0207, .l2hLogPage span.c0307, .l2hLogPage span.c0407, .l2hLogPage span.c0507, .l2hLogPage span.c0607, .l2hLogPage span.c0707, .l2hLogPage span.c0807, .l2hLogPage span.c0907, .l2hLogPage span.c1007, .l2hLogPage span.c1107, .l2hLogPage span.c1207, .l2hLogPage span.c1307, .l2hLogPage span.c1407, .l2hLogPage span.c1507 {background-color: orange;}\n");
	str_write(dst_file, ".l2hLogPage span.c0008, .l2hLogPage span.c0108, .l2hLogPage span.c0208, .l2hLogPage span.c0308, .l2hLogPage span.c0408, .l2hLogPage span.c0508, .l2hLogPage span.c0608, .l2hLogPage span.c0708, .l2hLogPage span.c0808, .l2hLogPage span.c0908, .l2hLogPage span.c1008, .l2hLogPage span.c1108, .l2hLogPage span.c1208, .l2hLogPage span.c1308, .l2hLogPage span.c1408, .l2hLogPage span.c1508 {background-color: yellow;}\n");
	str_write(dst_file, ".l2hLogPage span.c0009, .l2hLogPage span.c0109, .l2hLogPage span.c0209, .l2hLogPage span.c0309, .l2hLogPage span.c0409, .l2hLogPage span.c0509, .l2hLogPage span.c0609, .l2hLogPage span.c0709, .l2hLogPage span.c0809, .l2hLogPage span.c0909, .l2hLogPage span.c1009, .l2hLogPage span.c1109, .l2hLogPage span.c1209, .l2hLogPage span.c1309, .l2hLogPage span.c1409, .l2hLogPage span.c1509 {background-color: lime;}\n");
	str_write(dst_file, ".l2hLogPage span.c0010, .l2hLogPage span.c0110, .l2hLogPage span.c0210, .l2hLogPage span.c0310, .l2hLogPage span.c0410, .l2hLogPage span.c0510, .l2hLogPage span.c0610, .l2hLogPage span.c0710, .l2hLogPage span.c0810, .l2hLogPage span.c0910, .l2hLogPage span.c1010, .l2hLogPage span.c1110, .l2hLogPage span.c1210, .l2hLogPage span.c1310, .l2hLogPage span.c1410, .l2hLogPage span.c1510 {background-color: teal;}\n");
	str_write(dst_file, ".l2hLogPage span.c0011, .l2hLogPage span.c0111, .l2hLogPage span.c0211, .l2hLogPage span.c0311, .l2hLogPage span.c0411, .l2hLogPage span.c0511, .l2hLogPage span.c0611, .l2hLogPage span.c0711, .l2hLogPage span.c0811, .l2hLogPage span.c0911, .l2hLogPage span.c1011, .l2hLogPage span.c1111, .l2hLogPage span.c1211, .l2hLogPage span.c1311, .l2hLogPage span.c1411, .l2hLogPage span.c1511 {background-color: aqua;}\n");
	str_write(dst_file, ".l2hLogPage span.c0012, .l2hLogPage span.c0112, .l2hLogPage span.c0212, .l2hLogPage span.c0312, .l2hLogPage span.c0412, .l2hLogPage span.c0512, .l2hLogPage span.c0612, .l2hLogPage span.c0712, .l2hLogPage span.c0812, .l2hLogPage span.c0912, .l2hLogPage span.c1012, .l2hLogPage span.c1112, .l2hLogPage span.c1212, .l2hLogPage span.c1312, .l2hLogPage span.c1412, .l2hLogPage span.c1512 {background-color: blue;}\n");
	str_write(dst_file, ".l2hLogPage span.c0013, .l2hLogPage span.c0113, .l2hLogPage span.c0213, .l2hLogPage span.c0313, .l2hLogPage span.c0413, .l2hLogPage span.c0513, .l2hLogPage span.c0613, .l2hLogPage span.c0713, .l2hLogPage span.c0813, .l2hLogPage span.c0913, .l2hLogPage span.c1013, .l2hLogPage span.c1113, .l2hLogPage span.c1213, .l2hLogPage span.c1313, .l2hLogPage span.c1413, .l2hLogPage span.c1513 {background-color: fuchsia;}\n");
	str_write(dst_file, ".l2hLogPage span.c0014, .l2hLogPage span.c0114, .l2hLogPage span.c0214, .l2hLogPage span.c0314, .l2hLogPage span.c0414, .l2hLogPage span.c0514, .l2hLogPage span.c0614, .l2hLogPage span.c0714, .l2hLogPage span.c0814, .l2hLogPage span.c0914, .l2hLogPage span.c1014, .l2hLogPage span.c1114, .l2hLogPage span.c1214, .l2hLogPage span.c1314, .l2hLogPage span.c1414, .l2hLogPage span.c1514 {background-color: silver;}\n");
	str_write(dst_file, ".l2hLogPage span.c0015, .l2hLogPage span.c0115, .l2hLogPage span.c0215, .l2hLogPage span.c0315, .l2hLogPage span.c0415, .l2hLogPage span.c0515, .l2hLogPage span.c0615, .l2hLogPage span.c0715, .l2hLogPage span.c0815, .l2hLogPage span.c0915, .l2hLogPage span.c1015, .l2hLogPage span.c1114, .l2hLogPage span.c1215, .l2hLogPage span.c1315, .l2hLogPage span.c1415, .l2hLogPage span.c1515 {background-color: gray;}\n");
	str_write(dst_file, ".l2hLogPage span.c0000, .l2hLogPage span.c0001, .l2hLogPage span.c0002, .l2hLogPage span.c0003, .l2hLogPage span.c0004, .l2hLogPage span.c0005, .l2hLogPage span.c0006, .l2hLogPage span.c0007, .l2hLogPage span.c0008, .l2hLogPage span.c0009, .l2hLogPage span.c0010, .l2hLogPage span.c0011, .l2hLogPage span.c0012, .l2hLogPage span.c0013, .l2hLogPage span.c0014, .l2hLogPage span.c0015 {color: white;}\n");
	str_write(dst_file, ".l2hLogPage span.c0100, .l2hLogPage span.c0101, .l2hLogPage span.c0102, .l2hLogPage span.c0103, .l2hLogPage span.c0104, .l2hLogPage span.c0105, .l2hLogPage span.c0106, .l2hLogPage span.c0107, .l2hLogPage span.c0108, .l2hLogPage span.c0109, .l2hLogPage span.c0110, .l2hLogPage span.c0111, .l2hLogPage span.c0112, .l2hLogPage span.c0113, .l2hLogPage span.c0114, .l2hLogPage span.c0115 {color: black;}\n");
	str_write(dst_file, ".l2hLogPage span.c0200, .l2hLogPage span.c0201, .l2hLogPage span.c0202, .l2hLogPage span.c0203, .l2hLogPage span.c0204, .l2hLogPage span.c0205, .l2hLogPage span.c0206, .l2hLogPage span.c0207, .l2hLogPage span.c0208, .l2hLogPage span.c0209, .l2hLogPage span.c0210, .l2hLogPage span.c0211, .l2hLogPage span.c0212, .l2hLogPage span.c0213, .l2hLogPage span.c0214, .l2hLogPage span.c0215 {color: navy;}\n");
	str_write(dst_file, ".l2hLogPage span.c0300, .l2hLogPage span.c0301, .l2hLogPage span.c0302, .l2hLogPage span.c0303, .l2hLogPage span.c0304, .l2hLogPage span.c0305, .l2hLogPage span.c0306, .l2hLogPage span.c0307, .l2hLogPage span.c0308, .l2hLogPage span.c0309, .l2hLogPage span.c0310, .l2hLogPage span.c0311, .l2hLogPage span.c0312, .l2hLogPage span.c0313, .l2hLogPage span.c0314, .l2hLogPage span.c0315 {color: green;}\n");
	str_write(dst_file, ".l2hLogPage span.c0400, .l2hLogPage span.c0401, .l2hLogPage span.c0402, .l2hLogPage span.c0403, .l2hLogPage span.c0404, .l2hLogPage span.c0405, .l2hLogPage span.c0406, .l2hLogPage span.c0407, .l2hLogPage span.c0408, .l2hLogPage span.c0409, .l2hLogPage span.c0410, .l2hLogPage span.c0411, .l2hLogPage span.c0412, .l2hLogPage span.c0413, .l2hLogPage span.c0414, .l2hLogPage span.c0415 {color: red;}\n");
	str_write(dst_file, ".l2hLogPage span.c0500, .l2hLogPage span.c0501, .l2hLogPage span.c0502, .l2hLogPage span.c0503, .l2hLogPage span.c0504, .l2hLogPage span.c0505, .l2hLogPage span.c0506, .l2hLogPage span.c0507, .l2hLogPage span.c0508, .l2hLogPage span.c0509, .l2hLogPage span.c0510, .l2hLogPage span.c0511, .l2hLogPage span.c0512, .l2hLogPage span.c0513, .l2hLogPage span.c0514, .l2hLogPage span.c0515 {color: maroon;}\n");
	str_write(dst_file, ".l2hLogPage span.c0600, .l2hLogPage span.c0601, .l2hLogPage span.c0602, .l2hLogPage span.c0603, .l2hLogPage span.c0604, .l2hLogPage span.c0605, .l2hLogPage span.c0606, .l2hLogPage span.c0607, .l2hLogPage span.c0608, .l2hLogPage span.c0609, .l2hLogPage span.c0610, .l2hLogPage span.c0611, .l2hLogPage span.c0612, .l2hLogPage span.c0613, .l2hLogPage span.c0614, .l2hLogPage span.c0615 {color: purple;}\n");
	str_write(dst_file, ".l2hLogPage span.c0700, .l2hLogPage span.c0701, .l2hLogPage span.c0702, .l2hLogPage span.c0703, .l2hLogPage span.c0704, .l2hLogPage span.c0705, .l2hLogPage span.c0706, .l2hLogPage span.c0707, .l2hLogPage span.c0708, .l2hLogPage span.c0709, .l2hLogPage span.c0710, .l2hLogPage span.c0711, .l2hLogPage span.c0712, .l2hLogPage span.c0713, .l2hLogPage span.c0714, .l2hLogPage span.c0715 {color: orange;}\n");
	str_write(dst_file, ".l2hLogPage span.c0800, .l2hLogPage span.c0801, .l2hLogPage span.c0802, .l2hLogPage span.c0803, .l2hLogPage span.c0804, .l2hLogPage span.c0805, .l2hLogPage span.c0806, .l2hLogPage span.c0807, .l2hLogPage span.c0808, .l2hLogPage span.c0809, .l2hLogPage span.c0810, .l2hLogPage span.c0811, .l2hLogPage span.c0812, .l2hLogPage span.c0813, .l2hLogPage span.c0814, .l2hLogPage span.c0815 {color: yellow;}\n");
	str_write(dst_file, ".l2hLogPage span.c0900, .l2hLogPage span.c0901, .l2hLogPage span.c0902, .l2hLogPage span.c0903, .l2hLogPage span.c0904, .l2hLogPage span.c0905, .l2hLogPage span.c0906, .l2hLogPage span.c0907, .l2hLogPage span.c0908, .l2hLogPage span.c0909, .l2hLogPage span.c0910, .l2hLogPage span.c0911, .l2hLogPage span.c0912, .l2hLogPage span.c0913, .l2hLogPage span.c0914, .l2hLogPage span.c0915 {color: lime;}\n");
	str_write(dst_file, ".l2hLogPage span.c1000, .l2hLogPage span.c1001, .l2hLogPage span.c1002, .l2hLogPage span.c1003, .l2hLogPage span.c1004, .l2hLogPage span.c1005, .l2hLogPage span.c1006, .l2hLogPage span.c1007, .l2hLogPage span.c1008, .l2hLogPage span.c1009, .l2hLogPage span.c1010, .l2hLogPage span.c1011, .l2hLogPage span.c1012, .l2hLogPage span.c1013, .l2hLogPage span.c1014, .l2hLogPage span.c1015 {color: teal;}\n");
	str_write(dst_file, ".l2hLogPage span.c1100, .l2hLogPage span.c1101, .l2hLogPage span.c1102, .l2hLogPage span.c1103, .l2hLogPage span.c1104, .l2hLogPage span.c1105, .l2hLogPage span.c1106, .l2hLogPage span.c1107, .l2hLogPage span.c1108, .l2hLogPage span.c1109, .l2hLogPage span.c1110, .l2hLogPage span.c1111, .l2hLogPage span.c1112, .l2hLogPage span.c1113, .l2hLogPage span.c1114, .l2hLogPage span.c1115 {color: aqua;}\n");
	str_write(dst_file, ".l2hLogPage span.c1200, .l2hLogPage span.c1201, .l2hLogPage span.c1202, .l2hLogPage span.c1203, .l2hLogPage span.c1204, .l2hLogPage span.c1205, .l2hLogPage span.c1206, .l2hLogPage span.c1207, .l2hLogPage span.c1208, .l2hLogPage span.c1209, .l2hLogPage span.c1210, .l2hLogPage span.c1211, .l2hLogPage span.c1212, .l2hLogPage span.c1213, .l2hLogPage span.c1214, .l2hLogPage span.c1215 {color: blue;}\n");
	str_write(dst_file, ".l2hLogPage span.c1300, .l2hLogPage span.c1301, .l2hLogPage span.c1302, .l2hLogPage span.c1303, .l2hLogPage span.c1304, .l2hLogPage span.c1305, .l2hLogPage span.c1306, .l2hLogPage span.c1307, .l2hLogPage span.c1308, .l2hLogPage span.c1309, .l2hLogPage span.c1310, .l2hLogPage span.c1311, .l2hLogPage span.c1312, .l2hLogPage span.c1313, .l2hLogPage span.c1314, .l2hLogPage span.c1315 {color: fuchsia;}\n");
	str_write(dst_file, ".l2hLogPage span.c1400, .l2hLogPage span.c1401, .l2hLogPage span.c1402, .l2hLogPage span.c1403, .l2hLogPage span.c1404, .l2hLogPage span.c1405, .l2hLogPage span.c1406, .l2hLogPage span.c1407, .l2hLogPage span.c1408, .l2hLogPage span.c1409, .l2hLogPage span.c1410, .l2hLogPage span.c1411, .l2hLogPage span.c1412, .l2hLogPage span.c1413, .l2hLogPage span.c1414, .l2hLogPage span.c1415 {color: silver;}\n");
	str_write(dst_file, ".l2hLogPage span.c1500, .l2hLogPage span.c1501, .l2hLogPage span.c1502, .l2hLogPage span.c1503, .l2hLogPage span.c1504, .l2hLogPage span.c1505, .l2hLogPage span.c1506, .l2hLogPage span.c1507, .l2hLogPage span.c1508, .l2hLogPage span.c1509, .l2hLogPage span.c1510, .l2hLogPage span.c1511, .l2hLogPage span.c1512, .l2hLogPage span.c1513, .l2hLogPage span.c1514, .l2hLogPage span.c1515 {color: gray;}\n");
	str_write(dst_file, ".l2hLogPage span.f00 {color: white;}\n");
	str_write(dst_file, ".l2hLogPage span.f01 {color: black;}\n");
	str_write(dst_file, ".l2hLogPage span.f02 {color: navy;}\n");
	str_write(dst_file, ".l2hLogPage span.f03 {color: green;}\n");
	str_write(dst_file, ".l2hLogPage span.f04 {color: red;}\n");
	str_write(dst_file, ".l2hLogPage span.f05 {color: maroon;}\n");
	str_write(dst_file, ".l2hLogPage span.f06 {color: purple;}\n");
	str_write(dst_file, ".l2hLogPage span.f07 {color: orange;}\n");
	str_write(dst_file, ".l2hLogPage span.f08 {color: yellow;}\n");
	str_write(dst_file, ".l2hLogPage span.f09 {color: lime;}\n");
	str_write(dst_file, ".l2hLogPage span.f10 {color: teal;}\n");
	str_write(dst_file, ".l2hLogPage span.f11 {color: aqua;}\n");
	str_write(dst_file, ".l2hLogPage span.f12 {color: blue;}\n");
	str_write(dst_file, ".l2hLogPage span.f13 {color: fuchsia;}\n");
	str_write(dst_file, ".l2hLogPage span.f14 {color: silver;}\n");
	str_write(dst_file, ".l2hLogPage span.f15 {color: gray;}\n\n");

	str_write(dst_file, "/* Miscellaneous classes for reuse */\n");
	str_write(dst_file, ".clearfloat {\n");
	str_write(dst_file, "/* this class should be placed on a div or break element and should be the final element before\n");
	str_write(dst_file, "the close of a container that should fully contain a float */\n");
	str_write(dst_file, "clear:both;\n");
	str_write(dst_file, "height:0;\n");
	str_write(dst_file, "font-size: 1px;\n");
	str_write(dst_file, "line-height: 0px;\n");
	str_write(dst_file, "}\n\n");

	str_write(dst_file, "--> \n\n");

	return;
}
