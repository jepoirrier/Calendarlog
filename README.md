# Calendarlog

## Introduction

Calendarlog is a small utility to build a logbook skeleton. Its 3 main features are:

* prints the calendar of a given month in plain text
* prints the calendar of a given month in HTML with each day linked to its logbook page
* creates all the HTML skeleton files for each day (for WinHelp or JavaHelp system ; or simply to read them in your browser)

The first feature is a bit like BSD cal with fewer options. Other features are explained below.
Usage

Options --help and --usage gives you all information you want about how to run the software. Here is the transcript of the --help option:

```
Usage: calendarlog [OPTION...] MONTH
calendarlog -- a calendar printer for logbook

  -c, --calendar             Prints out a calendar for MONTH in format YYYYMM
                             on standard output
  -f, --files                Creates a HTML file for each day in MONTH
  -h, --html                 Prints out a calendar for MONTH in HTML on
                             standard output
  -l, --links                Add links to HTML calendar (works with 'html'
                             only)
  -v, --verbose              Prints out some additional information (works with
                             'calendar' and 'html')
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Report bugs to <jepoirrier@gmail.com>.
```

Please note that MONTH has to be in 6 digits! If you want April 2009, you'll need to type 200904. This can be improved in future versions.

## Download

Directory src/ has all sources.

Requirement: in order to run this software, you should have the argp from the GNU C library which is included with most GNU/Linux and BSD systems.

How to compile: extract all the files in the archive and launch command "gcc -Wall calendarlog.c -o calendarlog". You'll have now the binary calendarlog which you can use with the above commands. For example: type ./calendarlog -v 200904 to obtain a simple calendar like cal 4 2009 will do.

## TO DO list, known bugs, room for improvements

As you can see from the version number, this small utility is in an early stage. Here is what is on my TODO list:

### Functional aspects:

* higher flexibility in the name + content of the files produced
* allow month number on 1 digits instead of 2

### Technical aspects:

* function to return a string with the datestring given a time_t
* function to write to file (given file pointer) with given string to write
* indent code
* valgrind on code
* improve code and user documentation
* see also TODOs in source code

## Contact

Please contact me for any bug reports, enhancements, feature requests, suggestions, comments, etc.
