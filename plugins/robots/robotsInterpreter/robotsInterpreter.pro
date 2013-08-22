TEMPLATE = subdirs

SUBDIRS = \
	robotsInterpreterCore \
	nullKitInterpreter \
#	nxtRobotsInterpreter \

nullKitInterpreter.depends = robotsInterpreterCore
#nxtRobotsInterpreter.depends = robotsInterpreterCore
