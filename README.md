# Progress Bar Printer | c-progress-bar
> Created by Nicholas Ramsay

A header-only library for creating and printing progress bars to the console. Unlike the many existing progress bar libraries, this one can print multiple progress bars and has multiple styles.

Currently works for:
* MacOS
* Linux

## Usage
### 1. Include
```c
#include "include/ProgressBar.h"
#include "include/ProgressBarPrinter.h"
```

### 2. Create some progress bars
```c
const uint16_t bars_count = 2;
ProgressBar* bars = malloc(bars_count * sizeof(ProgressBar));
bars[0] = NewProgressBar("0", 100);
bars[1] = NewProgressBar("1", 100);
```

### 3. Update the progress bars
```c
UpdateProgressBar(&bars[0], 1);
UpdateProgressBar(&bars[1], 2);
```

### 4. Create the printer
```c
uint8_t printer_flags = PBP_TITLES | PBP_SQUARE_BRACKETS;
ProgressBarPrinter printer = NewProgressBarPrinter(bars, bars_count, printer_flags);
```

### 5. Print
```c
ProgressBarPrinterPrint(printer);
```

### 6. Free progress bar memory
```c
FreeProgressBar(&bars[0]);
FreeProgressBar(&bars[1]);
free(bars);
```