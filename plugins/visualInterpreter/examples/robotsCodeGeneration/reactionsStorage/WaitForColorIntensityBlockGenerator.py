import os

port = "NXT_PORT_S" + waitFor.Port
sign = waitFor.Sign
if sign == "меньше":
  sign = "<"
elif sign == "больше":
  sign = ">"
elif sign == "не меньше":
  sign = ">="
elif sign == "не больше":
  sign = "<="
elif sign == "равно":
  sign = "=="

condition = sign + " " + waitFor.Intensity

waitForColorIntensityCode = "while (!(ecrobot_get_nxtcolorsensor_light(" + port + ") " + condition + "))\n{}\n"
partInitCode = "ecrobot_init_nxtcolorsensor(" + port
initCode = "ecrobot_init_nxtcolorsensor(" + port + ", NXT_COLORSENSOR));\n"
terminateCode = "ecrobot_term_nxtcolorsensor(" + port + ");\n"

relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

if code.find(partInitCode) == -1:
  code = code.replace("@@INITHOOKS@@", initCode + "@@INITHOOKS@@")
  code = code.replace("@@TERMINATEHOOKS@@", terminateCode + "@@TERMINATEHOOKS@@")

code = code.replace("@@CODE@@", waitForColorIntensityCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
