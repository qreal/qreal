import os

template = "#include \"kernel.h\"\n#include \"ecrobot_interface.h\"\n@@BALANCER@@\n@@VARIABLES@@\n\nvoid ecrobot_device_initialize(void)\n{\n@@INITHOOKS@@\n}\n\nvoid ecrobot_device_terminate(void)\n{\n@@TERMINATEHOOKS@@\n}\n\n/* nxtOSEK hook to be invoked from an ISR in category 2 */\nvoid user_1ms_isr_type2(void){ /* do nothing */ }\n\n@@CODE@@"

taskTemplate = "TASK(OSEK_Task_Number_0)\n{\n@@CODE@@\n}"
template = template.replace("@@CODE@@", taskTemplate)

dirPath = os.path.join(scriptDir, "nxt-tools")
if not os.path.exists(dirPath):
  os.mkdir(dirPath)
filePath = os.path.join(dirPath, "program0.c")

codeFile = open(filePath, 'w')
codeFile.write(template)
codeFile.close()
