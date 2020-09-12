#include "kernel.h"
#include <stdio.h>

extern int userMain(int argc, char** argv);

int main(int argc, char** argv) {		// program entry point
	Kernel::setup();					// setup kernel
	int ret = userMain(argc, argv);		// run user code
	Kernel::restore();					// restore everything
	return ret;
}
