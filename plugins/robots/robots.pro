TEMPLATE = subdirs

SUBDIRS = \
	editor \
	interpreters \
	#generators \
	qextserialport \
	utils \

qextserialport.file = thirdparty/qextserialport/qextserialport.pro
interpreters.depends = qextserialport
interpreters.depends = utils
