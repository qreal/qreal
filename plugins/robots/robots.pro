TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
#	robotsGenerator \
#	trikGenerator \
	qextserialport \

qextserialport.file = thirdparty/qextserialport/qextserialport.pro
robotsInterpreter.depends = qextserialport
