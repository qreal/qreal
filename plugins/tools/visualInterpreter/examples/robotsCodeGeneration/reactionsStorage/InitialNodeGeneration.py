# Reaction
template = "#include \"kernel.h\"\n#include \"ecrobot_interface.h\"\n@@BALANCER@@\n@@VARIABLES@@\n\nvoid ecrobot_device_initialize(void)\n{\n@@INITHOOKS@@\n}\n\nvoid ecrobot_device_terminate(void)\n{\n@@TERMINATEHOOKS@@\n}\n\n/* nxtOSEK hook to be invoked from an ISR in category 2 */\nvoid user_1ms_isr_type2(void){ /* do nothing */ }\n\n@@CODE@@"

task_template = "TASK(OSEK_Task_Number_0)\n{\n@@CODE@@\n}"
template = template.replace("@@CODE@@", task_template)

start.id = max_used_id

cur_node_is_processed = True
