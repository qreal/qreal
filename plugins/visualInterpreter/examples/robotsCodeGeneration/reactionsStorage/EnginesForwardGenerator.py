import os

power = engine.Power
brakeMode = engine.BrakeMode

if brakeMode == "скользить":
  brakeMode = "0"
elif brakeMode == "тормозить":
  brakeMode = "1"
else:
  brakeMode = "1"

ports = engine.Ports
portNames = []

if ports.find("A") != -1:
  portNames.append("NXT_PORT_A")

if ports.find("B") != -1:
  portNames.append("NXT_PORT_B")

if ports.find("C") != -1:
  portNames.append("NXT_PORT_C")

relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

initTerminateVelocity = "0"
initTerminateBrakeMode = "1"
enginesForwardCode = ""

for port in portNames:
  enginesForwardCode = enginesForwardCode + "nxt_motor_set_speed(" + port + ", " + power + ", " + brakeMode + ");\n"
  initCodeEngines = "nxt_motor_set_speed(" + port + ", " + initTerminateVelocity + ", " + initTerminateBrakeMode + ");\n"
  terminateCodeEngines = "nxt_motor_set_speed(" + port + ", " + initTerminateVelocity + ", " + initTerminateBrakeMode + ");\n"
  if code.find(initCodeEngines) == -1:
    code = code.replace("@@INITHOOKS@@", initCodeEngines + "@@INITHOOKS@@")
    code = code.replace("@@TERMINATEHOOKS@@", terminateCodeEngines + "@@TERMINATEHOOKS@@")

code = code.replace("@@CODE@@", enginesForwardCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
