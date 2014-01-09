TEMPLATE = subdirs

SUBDIRS = \
	editor \
	interpreters \
	#generators \
	qextserialport \

qextserialport.file = thirdparty/qextserialport/qextserialport.pro
interpreters.depends = qextserialport
