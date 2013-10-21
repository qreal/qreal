TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	robotsGeneratorBase \
	nxtGenerator \
	trikGenerator \
	qextserialport \

qextserialport.file = thirdparty/qextserialport/qextserialport.pro
robotsInterpreter.depends = qextserialport
nxtGenerator.depends = robotsGeneratorBase
trikGenerator.depends = robotsGeneratorBase
