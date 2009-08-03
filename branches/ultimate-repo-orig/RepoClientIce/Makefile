# The subdirectories in this directory.
SUBDIRS         = client
QMAKESUBDIRS 		= server

all: repoclientice.ice $(SUBDIRS) $(QMAKESUBDIRS)
	slice2cpp  repoclientice.ice
	slice2cs  repoclientice.ice
#	/bin/mv -f repoclientice.cpp repoclientice.h server
	/bin/mv -f repoclientice.cs client

.PHONY:         all clean $(SUBDIRS) $(QMAKESUBDIRS)

# For each subdirectory, invoke a make subprocess.
$(SUBDIRS):
	cd $@; $(MAKE)

$(QMAKESUBDIRS):
	cd $@; qmake && make

# Clean up build products in all subdirectories.
clean:
	for subdir in $(SUBDIRS); do \
		(cd $${subdir}; $(MAKE) $@); \
	done
	for subdir in $(QMAKESUBDIRS); do \
		(cd $${subdir}; $(MAKE) $@); \
	done
                           
	
