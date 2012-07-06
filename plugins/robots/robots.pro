TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	robotsGenerator \
	qextSerialPort \
#	constraintsRobotsMetamodel/constraintsRobotsMetamodel.pro \

qextSerialPort.file = thirdparty/qextserialport/qextserialport.pro
robotsInterpreter.depends = qextSerialPort
