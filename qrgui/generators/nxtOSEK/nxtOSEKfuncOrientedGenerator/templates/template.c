#include "kernel.h"
#include "ecrobot_interface.h"

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

@@PROCEDURE_CODE@@

TASK(OSEK_Task_Background)
{
	@@INIT_NODE_PROCEDURE_NAME@@();
}
