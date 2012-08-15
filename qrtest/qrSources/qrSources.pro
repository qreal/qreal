include(../../plugins/blockDiagram/visualDebugSupport/visualDebugSupport.pro)

include(../../qrkernel/qrkernel.pro)
include(../../qrutils/qrutils.pro)
include(../../qrrepo/qrrepo.pro)
include(../../qrmc/qrmc-lib.pro)

TEMPLATE = lib
CONFIG += staticlib

QT += xml svg

DESTDIR = ../../bin/unittests
TARGET = qrealSources

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc
