TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	interpreterBase \
	nullKitInterpreter \
#	nxtRobotsInterpreter \
	twoDModel \
	utils \

interpreterCore.depends = interpreterBase
nullKitInterpreter.depends = interpreterBase
#nxtRobotsInterpreter.depends = interpreterBase
#nxtRobotsInterpreter.depends = twoDModel
interpretersBase.depends = utils
twoDModel.depends = utils
