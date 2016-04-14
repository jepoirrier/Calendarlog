/** \file calendarlog.c
 * \brief calendarlog prints a calendar + logfiles
 *
 * \author Jean-Etienne Poirrier <jepoirrier@gmail.com>
 * \version 0.3
 * \license GNU GPL v3.0 or later
 *
 * \date 090330 (0.1) Initial version: only calendar
 * \date 090331 (0.2) Added HTML output
 * \date 090402 (0.3) Added creation of iiles
 */

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "calendarlog.h"

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;
     
	switch (key) {
	case 'c':
		arguments->calendar = 1;
		break;
	case 'h':
		arguments->html = 1;
		break;
	case 'f':
		arguments->files = 1;
		break;
        case 'l':
                arguments->links = WITHLINKS;
                break;
        case 'v':
                arguments->verbose = 1;
                break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 1) /* Too many arguments. */
			argp_usage (state);
		arguments->args[state->arg_num] = arg;
		break;
	case ARGP_KEY_END:
		if (state->arg_num < 1) /* Not enough arguments. */
		argp_usage (state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * Checks if a year is a leap year or not
 * \param year an integer for the current year (e.g. 2008)
 * \return int an integer 0 if not a leap year and 1 if a lieap year
 */
int checkLeapYear(int year){
    if((year % 4) == 0){
        maxmonthday[1] = maxmonthday[1]++;
        return(1);
    } else {
        return(0);
    } 
}

/**
 * Prints week days on the standard output
 * 
 * \param inHTML an integer asking for HTML output (1) or not (0)
 */
void printweekdays(int inHTML) {
    switch(inHTML){
    case INHTML:
        printf("\t<tr>\n\t\t<th>Mon</th>\n\t\t<th>Tue</th>\n\t\t"
               "<th>Wed</th>\n\t\t<th>Thu</th>\n\t\t<th>Fri</th>"
               "\n\t\t<th>Sat</th>\n\t\t<th>Sun</th>\n\t</tr>\n");
        break;
    case NOHTML:
    default:
	printf("Mon Tue Wed Thu Fri Sat Sun\n");
    }
}

/**
 * Prints a simple calendar on standard output
 *
 * \param mytime a structure tm (time) giving at least the month and year
 */
void printsCalendar(struct tm mytime) {
    int i;
    time_t mytime2;

    printweekdays(NOHTML);

    switch(mytime.tm_wday){
    case 0:
        printf("                        ");
        break;
    case 1:
        break;
    default:
        for(i = 1; i < mytime.tm_wday; i++)
            printf("    ");
    }

    for(mytime.tm_mday = 1; mytime.tm_mday < (maxmonthday[mytime.tm_mon]+1); mytime.tm_mday++) {
            mytime2 = mktime(&mytime);
            memcpy(&mytime, localtime(&mytime2), sizeof(mytime));

            if(mytime.tm_mday < 10)
                   printf("  %d ", mytime.tm_mday);
            else
                   printf(" %d ", mytime.tm_mday);

            if(mytime.tm_wday == 0)
                   printf("\n");

    }

    printf("\n");
}

/**
 * Prints a HTML calendar on standard output
 *
 * \param mytime a structure tm (time) giving at least the month and year
 */
void printsHTMLCalendar(struct tm mytime, int withlinks) {
    int i;
    char buffer[SIZE];
    time_t mytime2;

    printf("<table border=\"1\">\n");

    printweekdays(INHTML);

    switch(mytime.tm_wday){
    case 0:
        printf("\t<tr>\n\t\t<td>&nbsp;</td>\n\t\t<td>&nbsp;</td>\n\t\t"
               "<td>&nbsp;</td>\n\t\t<td>&nbsp;</td>\n\t\t<td>&nbsp;</td>"
               "\n\t\t<td>&nbsp;</td>\n");
        break;
    case 1:
        break;
    default:
        printf("\t<tr>\n");
        for(i = 1; i < mytime.tm_wday; i++){
                printf("\t\t<td>&nbsp;</td>\n");
        }
    }

    for(mytime.tm_mday = 1; mytime.tm_mday < (maxmonthday[mytime.tm_mon]+1); mytime.tm_mday++) {
        mytime2 = mktime(&mytime);
        memcpy(&mytime, localtime(&mytime2), sizeof(mytime));

        printf("\t\t<td align=\"center\">");
        switch(withlinks){
        case WITHLINKS:
            if(mytime.tm_wday != 0 && mytime.tm_wday != 6){
                strftime(buffer, SIZE, "%Y%m%d", &mytime);
                printf("<a href=\"%s%s.html\">%d</a>", fileprefix, buffer, mytime.tm_mday);
            } else {
                printf("%s%d%s", emph1, mytime.tm_mday, emph2);
            }
            break;
        case WITHOUTLINKS:
        default:
            if(mytime.tm_wday != 0 && mytime.tm_wday != 6)
                printf("%d", mytime.tm_mday);
            else
                printf("%s%d%s", emph1, mytime.tm_mday, emph2);
        }
        printf("</td>\n");

        if(mytime.tm_wday == 0) {
            printf("\t</tr>\n"); /* Close the row after Sundays */

            if(mytime.tm_mday + 1 < maxmonthday[mytime.tm_mon])
                printf("\t<tr>\n"); /* There are some days left for another row */
        }
        /* This end is a bit wrong since it happens most of the time that 
         * the last row is just ended by the following </table> */
    }

    printf("</table>\n");
}

/**
 * Create log files in the current directory
 *
 * \param mytime a structure tm (time) giving at least the month and year
 * \return int an integer EXIT_FAILURE or EXIT_SUCCESS
*/
int createsFiles(struct tm mytime){

    char buffer[SIZE];
    char filename[SIZE];
    char datestring[SIZE];
    int i;
    time_t mytime2;

    FILE *fp;

    for(mytime.tm_mday = 1; mytime.tm_mday < (maxmonthday[mytime.tm_mon]+1); mytime.tm_mday++) {
        mytime2 = mktime(&mytime);
        memcpy(&mytime, localtime(&mytime2), sizeof(mytime));

        /* we don't have log files for Saturdays nor Sundays */
        if(mytime.tm_wday != 0 && mytime.tm_wday != 6) {
            /* building filename */ /* TODO: use pre-defined file prefix */
            i = strftime(filename, SIZE, "logbook-%Y%m%d.html", &mytime);
            if(i == SIZE){
                fprintf(stderr, "Could not allocate enough room for filename\n");
                return(EXIT_FAILURE);
            } else {
                filename[i] = '\0'; /* really needed? */
            }
            printf("File: %s ", filename);

            /* building datestring */
            i = strftime(datestring, SIZE, "%A %B %d, %Y", &mytime);
            if(i == SIZE){
                fprintf(stderr, "Could not allocate enough room for datestring\n");
                return(EXIT_FAILURE);
            } else {
                datestring[i] = '\0'; /* really needed? */
            }
            printf("(%s) ", datestring);

            /* opening file */
            fp = fopen(filename, "w");
            if(fp == NULL) {
                fprintf(stderr, "Could not open file %s.\n", filename);
                return(EXIT_FAILURE);
            }
            printf(".");

            /* writing data to file */
            memcpy(buffer, "<html>\n<head>\n\t<title>", sizeof(char)*SIZE);
            i = fwrite(buffer, sizeof(char), strlen(buffer), fp);
            if(i < 0) {
                fprintf(stderr, "Error writing string to file\n");
                return(EXIT_FAILURE);
            }
            printf(".");
            
            i = fwrite(datestring, sizeof(char), strlen(datestring), fp);
            if(i < 0) {
                fprintf(stderr, "Error writing string to file\n");
                return(EXIT_FAILURE);
            }
            printf(".");

            memcpy(buffer, "</title>\n</head>\n<body>\n\t<h1>", sizeof(char)*SIZE);
            i = fwrite(buffer, sizeof(char), strlen(buffer), fp);
            if(i < 0) {
                fprintf(stderr, "Error writing string to file\n");
                return(EXIT_FAILURE);
            }
            printf(".");

            i = fwrite(datestring, sizeof(char), strlen(datestring), fp);
            if(i < 0) {
                fprintf(stderr, "Error writing string to file\n");
                return(EXIT_FAILURE);
            }
            printf(".");

            memcpy(buffer, "</h1>\n\n\t<!-- Add content here -->\n\n", sizeof(char)*SIZE);
            i = fwrite(buffer, sizeof(char), strlen(buffer), fp);
            if(i < 0) {
                fprintf(stderr, "Error writing string to file\n");
                return(EXIT_FAILURE);
            }
            printf(".");
            
            memcpy(buffer, "\t<hr />\n</body>\n</html>", sizeof(char)*SIZE);
            i = fwrite(buffer, sizeof(char), strlen(buffer), fp);
            if(i < 0) {
                fprintf(stderr, "Error writing string to file\n");
                return(EXIT_FAILURE);
            }
            printf(".");

            /* finally closing file */
            i = fclose(fp);
            printf("\n");
            if(i < 0) {
                fprintf(stderr, "Error closing file %s.\n", filename);
                return(EXIT_FAILURE);
            }
        } /* no else: we don't write anything on Saturdays nor Sundays */
    }
    return(EXIT_FAILURE);
}

/**
 * Main function
 *
 * \param argc an integer with the number of arguments
 * \param argv a pointer to a list of the arguments
 * \return int an integer that is currently always EXIT_SUCCESS
 */
int main(int argc, char **argv) {

	/* argp data */
	struct arguments arguments;
	arguments.calendar = 0;
	arguments.html = 0;
	arguments.files = 0;
        arguments.links = WITHOUTLINKS;
	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	/* By default, just a calendar */
	if(arguments.calendar == 0 && arguments.html == 0 && arguments.files == 0)
		arguments.calendar = 1;

	/*
	printf ("MONTHS = %s\nCALENDAR = %s\nHTML = %s\nFILES = %s\n",
		arguments.args[0],
		arguments.calendar ? "yes" : "no",
		arguments.html ? "yes" : "no",
		arguments.files ? "yes" : "no");
	*/

	/* General data */
	unsigned int year;
	int month, i;
	char buffer[SIZE];
	struct tm mytime = {0};
        time_t mytime2;

	year = atoi(arguments.args[0]);
	if(year == 0 || year < 100000 || year > 999999) {
		fprintf(stderr, "Bad year: %d\n", year);
		return(EXIT_FAILURE);
	}
	year = year / 100;
        checkLeapYear(year);
	month = (atoi(arguments.args[0]) - (year * 100));
        if(month < 0 || month > 12){
            fprintf(stderr, "Bad month: %d\n", month);
            return(EXIT_FAILURE);
        }

	mytime.tm_year = year - 1900;
	mytime.tm_mday = 1;
	mytime.tm_mon = month - 1;

	mytime2 = mktime(&mytime);
	memcpy(&mytime, localtime(&mytime2), sizeof(mytime));

        /* Prints out a simple calendar */
        if(arguments.calendar == 1) {
                if(arguments.verbose == 1){
                        strftime(buffer, SIZE, "%B %Y", &mytime);
                        printf("%s\n", buffer);
                }
                printsCalendar(mytime);
        }

        /* Prints out a HTML calendar */
        if(arguments.html == 1) {
                if(arguments.verbose == 1){
                        strftime(buffer, SIZE, "%B %Y", &mytime);
                        printf("<h2>%s</h2>\n", buffer);
                }
                printsHTMLCalendar(mytime, arguments.links);
        }

        /* Creates HTML logbook files for each working day in a month */
        if(arguments.files == 1) {
                i = createsFiles(mytime);
                if(i == EXIT_FAILURE)
                    return(EXIT_FAILURE);
        }

	return(EXIT_SUCCESS);
}
