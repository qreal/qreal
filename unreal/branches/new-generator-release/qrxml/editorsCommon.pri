QT += xml
TEMPLATE =  lib
CONFIG += plugin
DESTDIR = ../../qrgui/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

win32 | win32-msvc* {
        CONFIG(debug, debug|release) {
                QRXC = ..\..\qrxc\debug\qrxc.exe
        } else:CONFIG(release, debug|release){
                QRXC = ..\..\qrxc\release\qrxc.exe
        } else {
                error(Windows build definitely needs to be fixed)
        }
} else {
        QRXC = ..\..\qrxc\qrxc
}

COMMAND = cd $$QREAL_EDITOR_NAME && $$QRXC $$QREAL_XML

!exists($$QREAL_EDITOR_NAME$$quote(\generated\pluginInterface.h)) {
	!exists($$QRXC) {
		win32-msvc* {
			MAKE = nmake
		} else {
			win32 {
				MAKE = mingw32-make
			} else {
				MAKE = make
			}
		}
		QRXC_COMMAND = cd ..\qrxc && $${QMAKE_QMAKE} && $$MAKE
		QRXC_SYS = $$system($$QRXC_COMMAND)
	}
	SYS = $$system($$COMMAND) 
}

include(extraCompilers.pri)

SOURCES		+= \
			../../qrgui/umllib/sdfrenderer.cpp \
			../../qrgui/umllib/uml_nodeelement.cpp \
			../../qrgui/umllib/uml_edgeelement.cpp \
			../../qrgui/umllib/uml_element.cpp \
			../../qrgui/kernel/ids.cpp \

HEADERS		+= \
			../../qrgui/umllib/sdfrenderer.h \
			../../qrgui/umllib/uml_nodeelement.h \
			../../qrgui/umllib/uml_edgeelement.h \
			../../qrgui/umllib/uml_element.h \
			../../qrgui/kernel/ids.h \
			../editorinterface.h \


