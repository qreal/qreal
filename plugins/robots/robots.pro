TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	robotsGenerator \
	qextSerialPort \

qextSerialPort.file = thirdparty/qextserialport/qextserialport.pro
robotsInterpreter.depends = qextSerialPort
