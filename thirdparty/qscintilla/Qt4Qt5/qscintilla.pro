# The project file for the QScintilla library.
#
# Copyright (c) 2017 Riverbank Computing Limited <info@riverbankcomputing.com>
#
# This file is part of QScintilla.
#
# This file may be used under the terms of the GNU General Public License
# version 3.0 as published by the Free Software Foundation and appearing in
# the file LICENSE included in the packaging of this file.  Please review the
# following information to ensure the GNU General Public License version 3.0
# requirements will be met: http://www.gnu.org/copyleft/gpl.html.
#
# If you do not wish to use this file under the terms of the GPL version 3.0
# then you may purchase a commercial license.  For more information contact
# info@riverbankcomputing.com.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Modified by Yurii Litvinov to comply with QReal build rules.

# This must be kept in sync with Python/configure.py, Python/configure-old.py,
# example-Qt4Qt5/application.pro and designer-Qt4Qt5/designer.pro.
!win32:VERSION = 13.0.0

TARGET = qscintilla2

include(../../../global.pri)
TEMPLATE = lib

CONFIG += qt warn_off thread

#INCLUDEPATH = . $$PWD/../include $$PWD/../lexlib $$PWD/../src
INCLUDEPATH += $$PWD $$PWD/../include $$PWD/../lexlib $$PWD/../src

DEFINES = STATIC_BUILD QT SCI_LEXER
DEFINES += SCINTILLA_QT SCI_LEXER

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
	QT += printsupport
}

# Comment this in if you want the internal Scintilla classes to be placed in a
# Scintilla namespace rather than pollute the global namespace.
DEFINES += SCI_NAMESPACE

HEADERS = \
	./Qsci/qsciglobal.h \
	./Qsci/qsciscintilla.h \
	./Qsci/qsciscintillabase.h \
	./Qsci/qsciabstractapis.h \
	./Qsci/qsciapis.h \
	./Qsci/qscicommand.h \
	./Qsci/qscicommandset.h \
	./Qsci/qscidocument.h \
	./Qsci/qscilexer.h \
	./Qsci/qscilexercpp.h \
	./Qsci/qscilexerpascal.h \
	./Qsci/qscilexerpython.h \
	./Qsci/qscilexermatlab.h \
	./Qsci/qscilexerlua.h \
	./Qsci/qscimacro.h \
	./Qsci/qsciprinter.h \
	./Qsci/qscistyle.h \
	./Qsci/qscistyledtext.h \
	ListBoxQt.h \
	SciClasses.h \
	SciNamespace.h \
	ScintillaQt.h \
	../include/ILexer.h \
	../include/Platform.h \
	../include/Sci_Position.h \
	../include/SciLexer.h \
	../include/Scintilla.h \
	../include/ScintillaWidget.h \
	../lexlib/Accessor.h \
	../lexlib/CharacterCategory.h \
	../lexlib/CharacterSet.h \
	../lexlib/LexAccessor.h \
	../lexlib/LexerBase.h \
	../lexlib/LexerModule.h \
	../lexlib/LexerNoExceptions.h \
	../lexlib/LexerSimple.h \
	../lexlib/OptionSet.h \
	../lexlib/PropSetSimple.h \
	../lexlib/StringCopy.h \
	../lexlib/StyleContext.h \
	../lexlib/SubStyles.h \
	../lexlib/WordList.h \
	../src/AutoComplete.h \
	../src/CallTip.h \
	../src/CaseConvert.h \
	../src/CaseFolder.h \
	../src/Catalogue.h \
	../src/CellBuffer.h \
	../src/CharClassify.h \
	../src/ContractionState.h \
	../src/Decoration.h \
	../src/Document.h \
	../src/EditModel.h \
	../src/Editor.h \
	../src/EditView.h \
	../src/ExternalLexer.h \
	../src/FontQuality.h \
	../src/Indicator.h \
	../src/KeyMap.h \
	../src/LineMarker.h \
	../src/MarginView.h \
	../src/Partitioning.h \
	../src/PerLine.h \
	../src/PositionCache.h \
	../src/RESearch.h \
	../src/RunStyles.h \
	../src/ScintillaBase.h \
	../src/Selection.h \
	../src/SplitVector.h \
	../src/Style.h \
	../src/UnicodeFromUTF8.h \
	../src/UniConversion.h \
	../src/ViewStyle.h \
	../src/XPM.h

SOURCES = \
	qsciscintilla.cpp \
	qsciscintillabase.cpp \
	qsciabstractapis.cpp \
	qsciapis.cpp \
	qscicommand.cpp \
	qscicommandset.cpp \
	qscidocument.cpp \
	qscilexer.cpp \
	qscilexercpp.cpp \
	qscilexerpascal.cpp \
	qscilexerpython.cpp \
	qscilexermatlab.cpp \
	qscilexerlua.cpp \
	qscimacro.cpp \
	qsciprinter.cpp \
	qscistyle.cpp \
	qscistyledtext.cpp \
	MacPasteboardMime.cpp \
	InputMethod.cpp \
	SciClasses.cpp \
	ListBoxQt.cpp \
	PlatQt.cpp \
	ScintillaQt.cpp \
	../lexers/LexCPP.cpp \
	../lexers/LexPascal.cpp \
	../lexers/LexPython.cpp \
	../lexers/LexMatlab.cpp \
	../lexers/LexLua.cpp \
	../lexlib/Accessor.cpp \
	../lexlib/CharacterCategory.cpp \
	../lexlib/CharacterSet.cpp \
	../lexlib/LexerBase.cpp \
	../lexlib/LexerModule.cpp \
	../lexlib/LexerNoExceptions.cpp \
	../lexlib/LexerSimple.cpp \
	../lexlib/PropSetSimple.cpp \
	../lexlib/StyleContext.cpp \
	../lexlib/WordList.cpp \
	../src/AutoComplete.cpp \
	../src/CallTip.cpp \
	../src/CaseConvert.cpp \
	../src/CaseFolder.cpp \
	../src/Catalogue.cpp \
	../src/CellBuffer.cpp \
	../src/CharClassify.cpp \
	../src/ContractionState.cpp \
	../src/Decoration.cpp \
	../src/Document.cpp \
	../src/EditModel.cpp \
	../src/Editor.cpp \
	../src/EditView.cpp \
	../src/ExternalLexer.cpp \
	../src/Indicator.cpp \
	../src/KeyMap.cpp \
	../src/LineMarker.cpp \
	../src/MarginView.cpp \
	../src/PerLine.cpp \
	../src/PositionCache.cpp \
	../src/RESearch.cpp \
	../src/RunStyles.cpp \
	../src/ScintillaBase.cpp \
	../src/Selection.cpp \
	../src/Style.cpp \
	../src/UniConversion.cpp \
	../src/ViewStyle.cpp \
	../src/XPM.cpp

TRANSLATIONS = \
	qscintilla_cs.ts \
	qscintilla_de.ts \
	qscintilla_es.ts \
	qscintilla_fr.ts \
	qscintilla_pt_br.ts
