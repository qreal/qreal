import os

ports = engine.Ports
portNames = []

if ports.find("A") != -1:
  portNames.append("NXT_PORT_A")

if ports.find("B") != -1:
  portNames.append("NXT_PORT_B")

if ports.find("C") != -1:
  portNames.append("NXT_PORT_C")

enginesStopCode = ""

for port in portNames:
  enginesStopCode = enginesStopCode + "nxt_motor_set_speed(" + port + ", 0, 1);\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", enginesStopCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
