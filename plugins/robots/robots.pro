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
interpreters.depends = qextserialport utils common
generators.depends = common
