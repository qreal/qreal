TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	interpreterBase \
	nullKitInterpreter \
	nxtKitInterpreter \
	commonTwoDModel \

interpreterCore.depends = interpreterBase
nullKitInterpreter.depends = interpreterBase
nxtKitInterpreter.depends = interpreterBase
nxtKitInterpreter.depends = commonTwoDModel
commonTwoDModel.depends = interpreterBase
