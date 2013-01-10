import os

functionCode = func.Body
newFunctionCode = ""

variables = set()

for leftPart in functionCode.split(';'):
  if leftPart != "":
    newFunctionCode = newFunctionCode + leftPart.strip() + ";\n"
  if leftPart.find('=') != -1:
    leftPart = leftPart[: leftPart.find('=')]
    if leftPart[len(leftPart) - 1] in '+-=*/><':
      leftPart = leftPart[: len(leftPart) - 1]
    leftPart = leftPart.strip()
    variables.add(leftPart)

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

if not func.Init:
  code = code.replace("@@CODE@@", newFunctionCode + "@@CODE@@")
else:
  code = code.replace("@@INITHOOKS@@", newFunctionCode + "@@INITHOOKS@@")

for variable in variables:
  code = code.replace("@@VARIABLES@@", "static int " + variable + ";\n@@VARIABLES@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
