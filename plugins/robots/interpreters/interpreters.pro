TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	nullKitInterpreter \
	nxtKitInterpreter \
	trikKitInterpreter \
	ev3KitInterpreter \
	robotsPlugin \
	twoDModelRunner \

robotsPlugin.depends = interpreterCore
twoDModelRunner.depends = interpreterCore
