TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	interpreterBase \
	nullKitInterpreter \
#	nxtRobotsInterpreter \

interpreterCore.depends = interpreterBase
nullKitInterpreter.depends = interpreterBase
#nxtRobotsInterpreter.depends = interpreterBase
