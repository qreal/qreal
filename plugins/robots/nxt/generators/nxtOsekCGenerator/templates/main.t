#include <string.h>
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "trik_studio_utils.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

U32 __interpretation_started_timestamp__ = 0;
@@VARIABLES@@

@@BMP_FILES@@

@@SUBPROGRAMS_FORWARDING@@

@@THREADS_FORWARDING@@

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
	ts_dispatch_timers();
@@USERISRHOOKS@@
}

@@SUBPROGRAMS@@

@@THREADS@@

/* Main task */
TASK(TASK_MAIN)
{
	__interpretation_started_timestamp__ = systick_get_ms();
@@MAIN_CODE@@
}
