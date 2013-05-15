fsi.exe build.fsx qrealRobots.xml %1 --setvars include_nxt_tools=yes
fsi.exe build.fsx qrealRobots.xml %1
fsi.exe build.fsx qrealRobots.xml %1 --setvars include_nxt_tools=yes unattended=yes
fsi.exe build.fsx qrealRobots.xml %1 --setvars unattended=yes