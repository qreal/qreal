# Here must be the call to QRMC console generator

TARGET = generated

TEMPLATE = subdirs

SUBDIRS = \
	generated \

generated.file = generated/robotsMetamodel.pro

OTHER_FILES += \
	doc/changelog.txt \

