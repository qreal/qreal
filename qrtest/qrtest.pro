TEMPLATE = subdirs

SUBDIRS = \
	thirdparty/gmock-1.6.0 \
	unitTests \
#	editorPluginTestingFramework \

unitTests.depends = thirdparty/gmock-1.6.0