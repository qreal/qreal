TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	robotsGeneratorBase \
	nxtGenerator \
	trikGenerator \
	russianCGenerator \
	qextserialport \
        fSharpGenerator \



qextserialport.file = thirdparty/qextserialport/qextserialport.pro
robotsInterpreter.depends = qextserialport
nxtGenerator.depends = robotsGeneratorBase
trikGenerator.depends = robotsGeneratorBase
fSharpGenerator.depends = robotsGeneratorBase
russianCGenerator.depends = robotsGeneratorBase

