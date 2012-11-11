import os

port = "NXT_PORT_S" + waitFor.Port

colorNxtType = ""
colorStr = waitFor.Color

if colorStr == "Красный":
  colorNxtType = "NXT_COLOR_RED"
elif colorStr == "Зелёный":
  colorNxtType = "NXT_COLOR_GREEN"
elif colorStr == "Синий":
  colorNxtType = "NXT_COLOR_BLUE"
elif colorStr == "Чёрный":
  colorNxtType = "NXT_COLOR_BLACK"
elif colorStr == "Жёлтый":
  colorNxtType = "NXT_COLOR_YELLOW"
elif colorStr == "Белый":
  colorNxtType = "NXT_COLOR_WHITE"


waitForColorBlockCode = "while (ecrobot_get_nxtcolorsensor_id(" + port + ") != " + colorNxtType + ")\n{}\n"
partInitCode = "ecrobot_init_nxtcolorsensor(" + port
initCode = "ecrobot_init_nxtcolorsensor(" + port + ", " + colorNxtType + ");\n"
terminateCode = "ecrobot_init_nxtcolorsensor(" + port + ", " + colorNxtType + ");\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

if code.find(partInitCode) == -1:
  code.replace("@@INITHOOKS@@", initCode + "@@INITHOOKS@@")
  code.replace("@@TERMINATEHOOKS@@", terminateCode + "@@TERMINATEHOOKS@@")

code = code.replace("@@CODE@@", waitForColorBlockCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
