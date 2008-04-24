CSC=gmcs
CSCFLAGS=/reference:icecs.dll /lib:/usr/lib/cli/libzeroc-ice-3.2/

all: client.exe
client.exe: client.cs repoclientice.cs
	$(CSC) $(CSCFLAGS) $^
clean:
	rm -f  client.exe
