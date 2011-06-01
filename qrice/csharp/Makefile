CSC=gmcs
CSCFLAGS=/reference:icecs.dll /lib:/opt/IceCS/bin/

all: client.exe
client.exe: client.cs repoclientice.cs
	$(CSC) $(CSCFLAGS) $^
clean:
	rm -f  client.exe
