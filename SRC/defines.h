#define OLD_ENTRY 0x08
#define NEW_ENTRY 0x60


typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;
typedef unsigned int Time;
typedef unsigned long StackSize;
typedef unsigned Address;
typedef int ID;
