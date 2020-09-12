#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

#define lock asm{\
	pushf;\
	cli;\
}
#define unlock asm popf

int syncPrintf(const char *format, ...){
	int res;
	va_list args;
	lock;
	va_start(args,format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
	return res;
}
