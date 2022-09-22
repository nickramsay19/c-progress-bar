#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // for sleep(int)
#include "include/ProgressBar.h"
#include "include/ProgressBarPrinter.h"

int main(int argc, char** argv) {

	setbuf(stdout, NULL);

	// create array of progress bars
	const uint16_t bars_count = 2;
	ProgressBar* bars = malloc(bars_count * sizeof(ProgressBar));
	// add some bars
	bars[0] = NewProgressBar("zero", 100);
	bars[1] = NewProgressBar("1", 100);
	//bars[2] = NewProgressBar("2", 100);

	// printer
	ProgressBarPrinter printer = NewProgressBarPrinter(bars, bars_count, PBP_TITLES | PBP_CURLY_BRACKETS);

	for (uint8_t i = 0; i < 100; i++) {
		UpdateProgressBar(&bars[0], 1);
		UpdateProgressBar(&bars[1], 2);
		ProgressBarPrinterPrint(printer);

		usleep(100000); 
	}

	// free progress bars
	for (uint8_t i = 0; i < bars_count; i++) {
		FreeProgressBar(&bars[i]);
	}
	free(bars);

	return EXIT_SUCCESS;
}
