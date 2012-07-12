#include "kernel.h"
#include "ecrobot_interface.h"

#include "balancer.h" 

@@VARIABLES@@

void ecrobot_device_initialize(void)
{
@@HOOKS@@
}

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

TASK(OSEK_Task_Background)
{
@@CODE@@
}
