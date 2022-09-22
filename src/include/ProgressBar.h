#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <stdlib.h>
#include <string.h>

#define MAX_PROGRESS_BAR_TITLE 100

typedef struct ProgressBarStruct {
    uint16_t progress;
    uint16_t total;
    char* title;
} ProgressBar;

// creates it on the stack
ProgressBar NewProgressBar(char* title, uint16_t total) {

    ProgressBar pb = {
        .progress = 0,
        .total = total,
        .title = (char*) malloc(strlen(title) * sizeof(char))
    };

    strcpy(pb.title, title);

    return pb;
}

int FreeProgressBar(ProgressBar* pb) {
    if (pb->title == NULL) {
        return EXIT_FAILURE;
    }

    free(pb->title);

    return EXIT_SUCCESS;
}

int UpdateProgressBar(ProgressBar* pb, uint16_t progress) {
	pb->progress += progress;
	return EXIT_SUCCESS;
}

#endif // PROGRESS_BAR_H
