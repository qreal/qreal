TEMPLATE = subdirs

SUBDIRS = \
	qrSources \
	unitTests \

unitTests.depends = qrSources
