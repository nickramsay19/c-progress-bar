#ifndef PROGRESS_BAR_PRINTER_H
#define PROGRESS_BAR_PRINTER_H

#define MAX_PROGRESS_BARS 10

#include <stdlib.h>
#include <sys/ioctl.h> // get window width
#include <unistd.h> // get window width
//#include <conio.h> // get cursor position on windows only
#include <curses.h>
#include "ProgressBar.h"

/*
Features:
* Multiple progress bar print (one per line)
* Set width to window width
* Multiple styles
*/

typedef struct ProgressBarPrinterStruct {
	//ProgressBar pbs [MAX_PROGRESS_BARS];
	uint8_t pbs_count; // how many are we actually using
	
	uint16_t bar_width; // will be the width of all of them

} ProgressBarPrinter;

ProgressBarPrinter NewProgressBarPrinter() {
	ProgressBarPrinter pbp;
	pbp.pbs_count = 0;

	// calculate window/terminal width
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	pbp.bar_width = w.ws_col - 1;

	return pbp;
}

int ProgressBarPrinterAddCopy(ProgressBarPrinter* pbp, ProgressBar* pb) {
	if (pbp->pbs_count >= MAX_PROGRESS_BARS) {
		return EXIT_FAILURE;
	}

	pbp->pbs[pbp->pbs_count++] = *pb; // dereference and copy 
	return EXIT_SUCCESS;
}

int ProgressBarPrinterAdd(ProgressBarPrinter* pbp, char* title, uint16_t total) {
	if (pbp->pbs_count >= MAX_PROGRESS_BARS) {
		return EXIT_FAILURE;
	}

	printf("2b\n");
	uint8_t count = pbp->pbs_count++;
	printf("2c\n");
	ProgressBar pb = NewProgressBar(title, total);
	printf("2d\n");
	pbp->pbs[count] = pb;
	printf("2e\n");
	return EXIT_SUCCESS;
}

int ProgressBarPrinterPrint(ProgressBarPrinter* pbp) {
	for (uint8_t pbi = 0; pbi < pbp->pbs_count; pbi++) {
		printf("[");
		for (uint16_t col = 1; col < pbp->bar_width-1; col++) {
			printf("%%");
		}
		printf("]\n");
	}

	return EXIT_SUCCESS;
}

#endif // PROGRESS_BAR_PRINTER_H
