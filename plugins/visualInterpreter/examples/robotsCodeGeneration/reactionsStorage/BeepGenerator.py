import os

beepCode = "ecrobot_sound_tone(1000, 100, " + beep.Volume+ ");\n"

relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", beepCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
