CSC=gmcs
CSCFLAGS=/reference:icecs.dll /lib:/usr/lib/cli/libzeroc-ice-3.2/ 
#/lib:/opt/IceCS-3.2.1/bin/

all: client.exe
client.exe: client.cs repoclientice.cs
	$(CSC) $(CSCFLAGS) $^
clean:
	rm -f  client.exe
