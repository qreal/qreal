TEMPLATE = subdirs

SUBDIRS = \
	editor \
	interpreters \
	generators \
	qextserialport \
	utils \
	trikRuntimeUploaderPlugin \

qextserialport.file = thirdparty/qextserialport/qextserialport.pro
interpreters.depends = qextserialport
interpreters.depends = utils
generators.depends = interpreters
trikRuntimeUploaderPlugin.file = trikRuntimeUploaderPlugin/trikRuntimeUploaderPlugin.pro
