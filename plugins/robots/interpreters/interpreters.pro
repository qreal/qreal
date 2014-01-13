TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	interpreterBase \
	nullKitInterpreter \
	nxtKitInterpreter \
	twoDModel \
	utils \

interpreterCore.depends = interpreterBase
nullKitInterpreter.depends = interpreterBase
nxtKitInterpreter.depends = interpreterBase
nxtKitInterpreter.depends = twoDModel
interpretersBase.depends = utils
twoDModel.depends = utils
