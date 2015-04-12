TEMPLATE = subdirs

SUBDIRS = \
	editor \
	examples \
	common \
	interpreters \
	generators \
	qextserialport \
	utils \

qextserialport.subdir = $$PWD/thirdparty/qextserialport
interpreters.depends = common qextserialport utils
generators.depends = common
