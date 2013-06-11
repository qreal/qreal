#include "kernel.h"
#include "ecrobot_interface.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

@@BALANCER@@

@@VARIABLES@@

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

@@CODE@@