TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	qextSerialPort \

robotsInterpreter.depends = qextSerialPort

qextSerialPort.file = thirdparty/qextserialport/qextserialport.pro
editor.file = editor/generated/robotsMetamodel.pro

