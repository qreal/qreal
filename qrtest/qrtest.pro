TEMPLATE = subdirs

SUBDIRS = \
	gmock \
	unitTests \
#	editorPluginTestingFramework \

unitTests.depends = gmock

gmock.file = thirdparty/gmock-1.6.0/gmock.pro
