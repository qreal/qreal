TEMPLATE = subdirs

SUBDIRS = \
	kitBase \
	twoDModel \
	nxtKit \
	trikKit \
	ev3Kit \

twoDModel.depends = kitBase
nxtKit.depends = twoDModel
trikKit.depends = twoDModel
ev3Kit.depends = twoDModel
