QT		+=  sql network
INCLUDEPATH	+= ../repo 
SOURCES		= main.cpp

OBJECTS_DIR 	= obj
MOC_DIR		= moc

# Model
HEADERS         +=  ../repo/realrepomodel.h \
		    ../repo/realrepoclient.h \
		    ../repo/realrepoinfo.h 
#		    ../common/realrepoapiclasses.h \
#		    ../common/classes.h

SOURCES	       += ../repo/realrepomodel.cpp \
		    ../repo/realrepoclient.cpp \
		    ../repo/realrepoinfo.cpp \


