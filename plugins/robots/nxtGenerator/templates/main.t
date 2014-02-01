#include <string.h>
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

@@VARIABLES@@

@@BMP_FILES@@

void ecrobot_device_initialize(void)
{
	srand(systick_get_ms());
@@INITHOOKS@@
}

void ecrobot_device_terminate(void)
{
@@TERMINATEHOOKS@@
}

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{ 
@@USERISRHOOKS@@
}

@@SUBPROGRAMS@@

/* Main task */
TASK(OSEK_Task_Number_0)
{
@@MAIN_CODE@@
}