# Here must be the call to QRMC console generator

TARGET = generated

TEMPLATE = subdirs

SUBDIRS = \
	generated \

!win32 {
	SUBDIRS += \
		doc \
		copy-images.pro \
		copy-docs.pro \
}

generated.file = generated/robotsMetamodel.pro

