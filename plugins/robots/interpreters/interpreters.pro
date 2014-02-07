TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	interpreterBase \
	nullKitInterpreter \
	nxtKitInterpreter \
	twoDModel \

interpreterCore.depends = interpreterBase
nullKitInterpreter.depends = interpreterBase
nxtKitInterpreter.depends = interpreterBase
nxtKitInterpreter.depends = twoDModel
