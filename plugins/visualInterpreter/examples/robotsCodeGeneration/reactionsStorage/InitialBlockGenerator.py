import os

template = "#include \"kernel.h\"\n#include \"ecrobot_interface.h\"\n@@BALANCER@@\n@@VARIABLES@@\n\nvoid ecrobot_device_initialize(void)\n{\n@@INITHOOKS@@\n}\n\nvoid ecrobot_device_terminate(void)\n{\n@@TERMINATEHOOKS@@\n}\n\n/* nxtOSEK hook to be invoked from an ISR in category 2 */\nvoid user_1ms_isr_type2(void){ /* do nothing */ }\n\n@@CODE@@"

taskTemplate = "TASK(OSEK_Task_Number_0)\n{\n@@CODE@@\n}"
template = template.replace("@@CODE@@", taskTemplate)

numberOfPorts = 4
portValues = [initBlock.port_1, initBlock.port_2, initBlock.port_3, initBlock.port_4]

initCode = []
terminateCode = []

for i in xrange(numberOfPorts):
  initEcrobotColorSensorPortS = "ecrobot_init_nxtcolorsensor(NXT_PORT_S"
  if portValues[i] == "Ультразвуковой сенсор":
    initCode.append("ecrobot_init_sonar_sensor(NXT_PORT_S" + str(i+1) + ");\n")
    terminateCode.append("ecrobot_term_sonar_sensor(NXT_PORT_S" + str(i+1) + ");\n")
  elif portValues[i] == "Сенсор цвета (все цвета)":
    initCode.append(initEcrobotColorSensorPortS + "1, NXT_LIGHTSENSOR_WHITE);\n")
    terminateCode.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i+1) + ");\n")
  elif portValues[i] == "Сенсор цвета (красный)":
    initCode.append(initEcrobotColorSensorPortS + str(i) + ", NXT_LIGHTSENSOR_RED);\n")
    terminateCode.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i+1) + ");\n")
  elif portValues[i] == "Сенсор цвета (зеленый)":
    initCode.append(initEcrobotColorSensorPortS + str(i) + ", NXT_LIGHTSENSOR_GREEN);\n")
    terminateCode.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i+1) + ");\n")
  elif portValues[i] == "Сенсор цвета (синий)":
    initCode.append(initEcrobotColorSensorPortS + str(i) + ", NXT_LIGHTSENSOR_BLUE);\n")
    terminateCode.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i+1) + ");\n")
  elif portValues[i] == "Сенсор цвета (пассивный)":
    initCode.append(initEcrobotColorSensorPortS + str(i) + ", NXT_COLORSENSOR);\n")
    terminateCode.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i+1) + ");\n")

template = template.replace("@@INITHOOKS@@", ''.join(initCode) + "@@INITHOOKS@@")
template = template.replace("@@TERMINATEHOOKS@@", ''.join(terminateCode) + "@@TERMINATEHOOKS@@")

scriptDir = os.path.dirname(__file__)
dirPath = os.path.join(scriptDir, "nxt-tools")
if not os.path.exists(dirPath):
  os.mkdir(dirPath)
filePath = os.path.join(dirPath, "program0.c")

codeFile = open(filePath, 'w')
codeFile.write(template)
codeFile.close()
