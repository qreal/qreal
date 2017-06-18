/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "qscintillaTextEdit.h"

#include <QtWidgets/QShortcut>

#include <thirdparty/qscintilla/Qt4Qt5/Qsci/qsciapis.h>
#include <brandManager/brandManager.h>

using namespace qReal;
using namespace text;

QScintillaTextEdit::QScintillaTextEdit()
	: mRole(0)
{
	init();
}

QScintillaTextEdit::QScintillaTextEdit(const QPersistentModelIndex &index, const int &role)
	: mIndex(index)
	, mRole(role)
{
	init();
}

QScintillaTextEdit::~QScintillaTextEdit()
{
	emit textSaved(text(), mIndex, mRole);
}

LanguageInfo QScintillaTextEdit::currentLanguage() const
{
	return mLanguage;
}

void QScintillaTextEdit::setCurrentFont(const QFont &font) {
	mFont = font;
	QFontMetrics metrics(mFont);
	mAverageCharWidth = metrics.averageCharWidth();
}

void QScintillaTextEdit::setCurrentLanguage(const LanguageInfo &language)
{
	setLexer(0);

	mLanguage = language;
	setIndentationsUseTabs(mLanguage.tabIndentation);
	setTabWidth(mLanguage.tabSize);
	setFont(mFont);
	setLexer(mLanguage.lexer);

	if (mLanguage.lexer) {
		mFont.setPointSize(mLanguage.lexer->defaultFont().pointSize());
		mLanguage.lexer->setFont(mFont);
		QsciAPIs * const api = new QsciAPIs(mLanguage.lexer);
		for (const QString &additionalToken : mLanguage.additionalAutocompletionTokens) {
			api->add(additionalToken);
		}

		api->prepare();
	}
}

QString QScintillaTextEdit::editorId() const
{
	return "qReal.TextEditor." + QString::number(mIndex.internalId());
}

bool QScintillaTextEdit::supportsZooming() const
{
	return true;
}

bool QScintillaTextEdit::supportsCopying() const
{
	return true;
}

bool QScintillaTextEdit::supportsPasting() const
{
	return true;
}

bool QScintillaTextEdit::supportsCutting() const
{
	return true;
}

void QScintillaTextEdit::zoomIn()
{
	QsciScintilla::zoomIn();
}

void QScintillaTextEdit::zoomOut()
{
	QsciScintilla::zoomOut();
}

void QScintillaTextEdit::copy()
{
	QsciScintilla::copy();
}

void QScintillaTextEdit::paste()
{
	QsciScintilla::paste();
}

void QScintillaTextEdit::cut()
{
	QsciScintilla::cut();
}

void QScintillaTextEdit::init()
{
	// For some reason c++11-style connections do not work here!
	connect(this, SIGNAL(textChanged()), this, SLOT(emitTextWasModified()));
	setDefaultSettings();
	setCurrentLanguage(Languages::textFileInfo("*.txt"));
}

void QScintillaTextEdit::setDefaultSettings()
{
	// Default font
	int id = QFontDatabase::addApplicationFont(BrandManager::fonts()->monospaceFont());
	QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	mFont = QFont(family, 9, 50);
	QFontMetrics metrics(mFont);
	mAverageCharWidth = metrics.averageCharWidth();
	setFont(mFont);

	// Current line highlighting
	setCaretLineVisible(true);
	setCaretLineBackgroundColor(QColor("gainsboro"));

	// Auto indent
	setAutoIndent(true);
	setIndentationGuides(false);

	// Tab size will be used
	setIndentationWidth(0);

	// Whitespaces visibility
	setWhitespaceVisibility(QsciScintilla::WsInvisible);

	// Show margin with line numbers (up to 1000)
	setMarginsBackgroundColor(QColor("gainsboro"));
	setMarginLineNumbers(1, true);
	setMarginWidth(1, QString("1000"));

	// Autocompletion of lexems
	setAutoCompletionSource(QsciScintilla::AcsAll);
	setAutoCompletionCaseSensitivity(true);
	setAutoCompletionReplaceWord(true);
	setAutoCompletionShowSingle(true);
	setAutoCompletionThreshold(1);

	// Autohighlighting of brackets
	setBraceMatching(QsciScintilla::SloppyBraceMatch);
	setMatchedBraceBackgroundColor(Qt::yellow);
	setUnmatchedBraceForegroundColor(Qt::blue);

	// EOL symbol
#if defined Q_OS_X11
	setEolMode(QsciScintilla::EolUnix);
#elif defined Q_OS_WIN
	setEolMode(QsciScintilla::EolWindows);
#elif defined Q_OS_MAC
	setEolMode(QsciScintilla::EolMac);
#endif

	// Input encoding
	setUtf8(true);

	// Ctrl + Space Autocomplete
	QShortcut * const ctrlSpace = new QShortcut(QKeySequence("Ctrl+Space"), this);
	connect(ctrlSpace, &QShortcut::activated, this, &QScintillaTextEdit::autoCompleteFromAll);
}

void QScintillaTextEdit::emitTextWasModified()
{
	setMarginWidth(1, QString::number(lines()).size() * mAverageCharWidth + 10);
	emit textWasModified(this);
}
