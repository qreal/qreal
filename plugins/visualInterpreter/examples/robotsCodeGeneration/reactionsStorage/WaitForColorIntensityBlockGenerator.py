import os

port = "NXT_PORT_S" + waitFor.Port
condition = waitFor.Sign + " " + waitFor.Intensity

waitForColorIntensityCode = "while (!(ecrobot_get_nxtcolorsensor_light(" + port + ") " + condition + "))\n{}\n"
partInitCode = "ecrobot_init_nxtcolorsensor(" + port
initCode = "ecrobot_init_nxtcolorsensor(" + port + ", NXT_COLORSENSOR));\n"
terminateCode = "ecrobot_term_nxtcolorsensor(" + port + ");\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

if code.find(partInitCode) == -1:
  code.replace("@@INITHOOKS@@", initCode + "@@INITHOOKS@@")
  code.replace("@@TERMINATEHOOKS@@", terminateCode + "@@TERMINATEHOOKS@@")

code = code.replace("@@CODE@@", waitForColorIntensityCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
