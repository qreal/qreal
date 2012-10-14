TEMPLATE = subdirs

SUBDIRS = \
	qrSources \
	unitTests \
	editorPluginTestingFramework \

unitTests.depends = qrSources
