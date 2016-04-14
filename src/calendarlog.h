/** \file calendarlog.h
 * \brief header for calendarlog.c
 *
 * \author Jean-Etienne Poirrier <jepoirrier@gmail.com>
 * \version 0.3
 * \license GNU GPL v3.0 or later
 *
 * \date 090402 (0.1) Initial version
 */

#define SIZE 256 /* buffer size */

#define INHTML 1 /* if week string is in HTML or not */
#define NOHTML 0

#define WITHLINKS 1 /* if displays HTML links or not */
#define WITHOUTLINKS 0

/* argp data */
const char *argp_program_version = "0.1";
const char *argp_program_bug_address = "<jepoirrier@gmail.com>";
static char doc[] = "calendarlog -- a calendar printer for logbook";
static char args_doc[] = {"MONTH"};
static struct argp_option options[] = {
        {"calendar", 'c', 0, 0, "Prints out a calendar for MONTH in format YYYYMM on standard output" },
        {"html",     'h', 0, 0, "Prints out a calendar for MONTH in HTML on standard output" },
        {"links",    'l', 0, 0, "Add links to HTML calendar (works with 'html' only)"},
        {"files",    'f', 0, 0, "Creates a HTML file for each day in MONTH"},
        {"verbose",  'v', 0, 0, "Prints out some additional information (works with 'calendar' and 'html')" },
        { 0 }
};
struct arguments {
        char *args[1]; /* MONTH */
        int calendar, html, links, files, verbose;
};
/* argp function */
static error_t parse_opt (int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, args_doc, doc };

/* Global variables */
int maxmonthday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char *fileprefix = "logbook-";
char *emph1 = "<em>";
char *emph2 = "</em>";

/**
 * Checks if a year is a leap year or not
 * \param year an integer for the current year (e.g. 2008)
 * \return int an integer 0 if not a leap year and 1 if a lieap year
 */
int checkLeapYear(int year);

/**
 * Prints week days on the standard output
 *
 * \param inHTML an integer asking for HTML output (1) or not (0)
 */
void printweekdays(int inHTML);

/**
 * Prints a simple calendar on standard output
 *
 * \param mytime a structure tm (time) giving at least the month and year
 */
void printsCalendar(struct tm mytime);

/**
 * Prints a HTML calendar on standard output
 *
 * \param mytime a structure tm (time) giving at least the month and year
 */
void printsHTMLCalendar(struct tm mytime, int withlinks);

/**
 * Create log files in the current directory
 *
 * \param mytime a structure tm (time) giving at least the month and year
 * \return int an integer EXIT_FAILURE or EXIT_SUCCESS
 */
int createsFile(struct tm mytime);

/**
 * Main function
 *
 * \param argc an integer with the number of arguments
 * \param argv a pointer to a list of the arguments
 * \return int an integer that is currently always EXIT_SUCCESS
 */
int main(int argc, char **argv);
