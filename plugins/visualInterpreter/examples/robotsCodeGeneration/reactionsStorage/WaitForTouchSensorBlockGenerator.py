import os

waitForTouchSensorCode = "while (!ecrobot_get_touch_sensor(NXT_PORT_S" + waitFor.Port + "))\n{}\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", waitForTouchSensorCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
