TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	interpreterBase \
	nullKitInterpreter \
	nxtKitInterpreter \
	trikKitInterpreter \
	ev3KitInterpreter \
	commonTwoDModel \
	twoDModelRunner \

interpreterCore.depends = commonTwoDModel
nullKitInterpreter.depends = interpreterBase
nxtKitInterpreter.depends = commonTwoDModel
trikKitInterpreter.depends = commonTwoDModel
ev3KitInterpreter.depends = interpreterBase
commonTwoDModel.depends = interpreterBase
twoDModelRunner.depends = commonTwoDModel
