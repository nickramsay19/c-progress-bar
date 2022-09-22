#ifndef PROGRESS_BAR_PRINTER_H
#define PROGRESS_BAR_PRINTER_H

#include <stdlib.h>
#include <string.h> // for strlen
#include <stdbool.h>
#include <sys/ioctl.h> // get window width
#include <unistd.h> // get window width
//#include <conio.h> // get cursor position on windows only
//#include <curses.h>
#include "ProgressBar.h"

// check os
#ifdef __MACH__
	#define clear() printf("\033[H\033[J")
	#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#endif

#define MAX_PROGRESS_BARS 10

// config flags
#define PBP_SQUARE_BRACKETS (uint8_t) 0b00000000 // default option so unneeded
#define PBP_CURLY_BRACKETS (uint8_t) 0b00000001
#define PBP_NO_BRACKETS (uint8_t) 0b00000010
#define PBP_NO_TITLES (uint8_t) 0b00000000 // default option so unneeded
#define PBP_TITLES (uint8_t) 0b00000100
#define PBP_PROGRESS_STAR (uint8_t) 0b00000000 // default option so unneeded
#define PBP_PROGRESS_PERCENT (uint8_t) 0b00001000
#define PBP_PROGRESS_DASH (uint8_t) 0b00010000
#define PBP_PROGRESS_MINUS (uint8_t) 0b00010000 // alias for above
#define PBP_PROGRESS_PLUS (uint8_t) 0b00100000
#define PBP_PROGRESS_HASH (uint8_t) 0b01000000

/*
Features:
* Multiple progress bar print (one per line)
* Set width to window width
* Multiple styles
*/

typedef struct ProgressBarPrinterStruct {

	// progress bars
	uint16_t bars_count;
	ProgressBar* bars;

	uint16_t console_width; // will be the width of all of them

	uint16_t max_title_length;
	bool print_titles;

	char Lc, Pc, Ec, Rc; // progress bar characters to print '[', '%', ' ', ']'

} ProgressBarPrinter;

ProgressBarPrinter NewProgressBarPrinter(ProgressBar* bars, uint16_t bars_count, uint8_t flags) {
	ProgressBarPrinter printer;

	// bars 
	printer.bars = bars;
	printer.bars_count = bars_count;

	// calculate window/terminal width
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	printer.console_width = w.ws_col - 1;

	// set default chars
	printer.Lc = '[';
	printer.Pc = '*';
	printer.Ec = ' ';
	printer.Rc = ']';

	// check flags
	if ((flags & PBP_TITLES) > 0) {
		printer.print_titles = true;

		// calculate maximum title length
		printer.max_title_length = 0;
		for (uint8_t i = 0; i < bars_count; i++) {
			size_t len = strlen(bars[i].title);
			if (len > printer.max_title_length) {
				printer.max_title_length = len;
			}
		}

	}
	if ((flags & PBP_CURLY_BRACKETS) > 0) {
		printer.Lc = '{';
		printer.Rc = '}';
	} else if ((flags & PBP_NO_BRACKETS) > 0) {
		printer.Lc = ' ';
		printer.Rc = ' ';
	}

	return printer;
}

int ProgressBarPrinterPrint(ProgressBarPrinter printer) {

	clear();

	// calculate the progress bar width
	uint16_t progress_bar_width = printer.console_width - 1; // assume no title
	if (printer.print_titles) {
		progress_bar_width -= printer.max_title_length + 1;
	}

	// loop over each progress bar
	for (uint8_t b = 0; b < printer.bars_count; b++) {

		// get progress as a fraction
		float bar_progress = ((float) printer.bars[b].progress)/((float) printer.bars[b].total);

		// print title 
		if (printer.print_titles) {
			printf("%s", printer.bars[b].title);

			// calculate margin (at least one) plus the difference between this title and the longest
			uint16_t margin = printer.max_title_length-strlen(printer.bars[b].title) + 1;
			for (uint16_t m = 0; m < margin; m++) {
				printf(" ");
			}
		}

		// print the progress bar column by column
		printf("%c", printer.Lc);
		for (uint16_t col = 1; col < progress_bar_width; col++) {
			float col_progress = ((float) col)/((float) printer.console_width);

			if (col_progress < bar_progress) {
				printf("%c", printer.Pc);
			} else {
				printf("%c", printer.Ec);
			}
		}
		printf("%c ", printer.Rc); // add a space so next print will overflow to newline (because we know the console_width)
	}

	return EXIT_SUCCESS;
}

#endif // PROGRESS_BAR_PRINTER_H
