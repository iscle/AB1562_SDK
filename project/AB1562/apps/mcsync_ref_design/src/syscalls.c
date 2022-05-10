//#include "exception_handler.h"
#include "FreeRTOS.h"
#include <string.h>
#include <stdlib.h>

void _Assert(const char *mesg, const char *fun)
{
    (void)(fun);
    (void)(mesg);
    /*
    char *substr = NULL, *saveptr = NULL;
    char *file = NULL, *line = NULL;

    substr = strtok_r((char*)mesg, ":", &saveptr);
    file = substr;

    substr = strtok_r(NULL, " ", &saveptr);
    line = substr;
    */
    //platform_assert("0", file, atoi(line));
    configASSERT(0);
}
/*
 * stub funs for xclib
 * only inbyte/outbyte/_exit are needed according to ./XtensaTools/xtensa-elf/src/libgloss/gloss.h
*/
void _exit(int status)
{
    (void)(status);

    //platform_assert("exit", __FILE__, __LINE__);
    configASSERT(0);

    for (;;);
}

char inbyte(void)
{
    /*
     * not implement, please directly use hal_uart interface
    */
    //platform_assert("not implement", __FILE__, __LINE__);
    configASSERT(0);
    return (char)0;
}

int outbyte(char c)
{
    (void)(c);
    /*
     * not implement, please directly use hal_uart interface
    */
    //platform_assert("not implement", __FILE__, __LINE__);
    configASSERT(0);
    return 0;
}
