#include "kernel.h"
#include "ecrobot_interface.h"

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

TASK(OSEK_Task_Background)
{
	ecrobot_init_sonar_sensor(NXT_PORT_S1);
	@@CODE@@
}
