import os

port = "NXT_PORT_S" + waitFor.Port
condition = waitFor.Sign + " " + waitFor.Percents

waitForLightCode = "while (!(ecrobot_get_light_sensor(" + port + ") " + condition + "))\n{}\n"
initCode = "ecrobot_set_light_sensor_active(" + port + ");\n"
terminateCode = "ecrobot_set_light_sensor_inactive(" + port + ");\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

if code.find(initCode) == -1:
  code.replace("@@INITHOOKS@@", initCode + "@@INITHOOKS@@")
  code.replace("@@TERMINATEHOOKS@@", terminateCode + "@@TERMINATEHOOKS@@")

code = code.replace("@@CODE@@", waitForLightCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
