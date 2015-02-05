TEMPLATE = subdirs

SUBDIRS = \
	editor \
	examples \
	interpreters \
	generators \
	qextserialport \
	utils \

qextserialport.subdir = $$PWD/thirdparty/qextserialport
interpreters.depends = qextserialport utils
generators.depends = interpreters
