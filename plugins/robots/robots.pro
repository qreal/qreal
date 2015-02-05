TEMPLATE = subdirs

SUBDIRS = \
	editor \
	examples \
	interpreters \
	generators \
	qextserialport \
	utils \

qextserialport.file = $$PWD/thirdparty/qextserialport/qextserialport.pro
interpreters.depends = qextserialport
interpreters.depends = utils
generators.depends = interpreters
