TEMPLATE = subdirs

SUBDIRS = \
	kitBase \
	twoDModel \
	nxtKit \
	trikKit \
	ev3Kit \

twoDModel.depends = kitBase
nxtKit.depends = kitBase
trikKit.depends = kitBase
ev3Kit.depends = kitBase
