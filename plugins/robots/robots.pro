TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	qextSerialPort \

qextSerialPort.file = thirdparty/qextserialport/qextserialport.pro
editor.file = editor/generated/robotsMetamodel.pro
robotsInterpreter.depends = qextSerialPort