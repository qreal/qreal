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
#include <QtCore/QRegularExpression>

#include <thirdparty/qscintilla/Qt4Qt5/Qsci/qsciapis.h>
#include <brandManager/brandManager.h>
#include <qrutils/widgets/searchLinePanel.h>

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
	setLexer(nullptr);

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

bool QScintillaTextEdit::supportsSearching() const
{
	return true;
}

bool QScintillaTextEdit::supportsFindAndReplace() const
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

void QScintillaTextEdit::find()
{
	mSearchLinePanel->setMode(ui::SearchLinePanel::OperationOptions::Find);
	mSearchLinePanel->attachTo(this);
}

void QScintillaTextEdit::init()
{
	// For some reason c++11-style connections do not work here!
	connect(this, SIGNAL(textChanged()), this, SLOT(emitTextWasModified()));
	initFindModeConnections();
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

	// Ctrl + / comment/uncomment
	QShortcut * const ctrlSlash = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Slash), this);
	connect(ctrlSlash, &QShortcut::activated, this, &QScintillaTextEdit::commentUncommentLines);

	// Ctrl + L Go to line and column
	QShortcut * const ctrlL = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), this);
	connect(ctrlL, &QShortcut::activated, this, &QScintillaTextEdit::goToLineColumn);
}

void QScintillaTextEdit::commentUncommentLines()
{
	auto regExpForm = [](const QString &str) {
		QString result;
		result.reserve(2 * str.size());
		QSet<QChar> shouldBeEscaped = {'?', '*', '['}; ///@todo: add more
		for (auto ch : str) {
			if (shouldBeEscaped.contains(ch)) {
				result.append(QString("\\%1").arg(ch));
			} else {
				result.append(ch);
			}
		}

		return result;
	};

	QString selectedText = this->selectedText();

	const QRegularExpression lineRegExp(QString("^(?<indent>[ |\t]*)%1(?<innerCode>.*)%2[ |\t]*$")
			.arg(regExpForm(mLanguage.lineCommentStart)).arg(regExpForm(mLanguage.lineCommentEnd)));
	if (not selectedText.isEmpty()) {
		long startSelectedPosition = SendScintilla(SCI_GETSELECTIONSTART);
		long endSelectedPosition = SendScintilla(SCI_GETSELECTIONEND);
		int lineStart = -1;
		int indexStart = -1;
		int lineEnd = -1;
		int indexEnd = -1;
		lineIndexFromPosition(static_cast<int>(startSelectedPosition), &lineStart, &indexStart);
		lineIndexFromPosition(static_cast<int>(endSelectedPosition), &lineEnd, &indexEnd);

		long lastSelectedLine = SendScintilla(SCI_LINEFROMPOSITION, endSelectedPosition);
		long lastSelectedLineEndPos = SendScintilla(SCI_GETLINEENDPOSITION, lastSelectedLine);

		// first case: selected full block with one or more lines
		if (indexStart == 0 && lastSelectedLineEndPos == endSelectedPosition) {
			QVector<QStringRef> selectedLines = selectedText.splitRef('\n');
			int sizeOfSelectedText = selectedText.length()
					+ selectedLines.length()
							* (mLanguage.lineCommentStart.length() + mLanguage.lineCommentEnd.length());
			QString textForReplace;
			textForReplace.reserve(sizeOfSelectedText);
			bool fullyCommented = true;

			for (const QStringRef &str : selectedLines) {
				fullyCommented &= lineRegExp.match(str).hasMatch();
			}

			int selectedLinesCount = selectedLines.count();
			if (fullyCommented) {
				const QStringRef &first = selectedLines.first();
				const QStringRef indent = lineRegExp.match(first).capturedRef("indent");
				textForReplace.append(indent);
				const QStringRef innerCode = lineRegExp.match(first).capturedRef("innerCode");
				textForReplace.append(innerCode);
				for (int i = 1; i < selectedLinesCount; ++i) {
					textForReplace.append('\n');
					textForReplace.append(lineRegExp.match(selectedLines[i]).capturedRef("innerCode"));
				}
			} else {
				textForReplace.append(QString("%1%2%3").arg(mLanguage.lineCommentStart)
						.arg(selectedLines.first().toString())
						.arg(mLanguage.lineCommentEnd));
				for (int i = 1; i < selectedLinesCount; ++i) {
					textForReplace.append(QString("\n%1%2%3").arg(mLanguage.lineCommentStart)
							.arg(selectedLines[i].toString()).arg(mLanguage.lineCommentEnd));
				}
			}

			replaceSelectedText(textForReplace);
		// second case: selected block with one or more lines
		} else {
			const QRegularExpression multiLineRegExp(QString("^%1(?<innerCode>.*)%2$")
					.arg(regExpForm(mLanguage.multilineCommentStart))
					.arg(regExpForm(mLanguage.multilineCommentEnd))
					, QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
			QRegularExpressionMatch match = multiLineRegExp.match(selectedText);
			if (match.hasMatch()) {
				QString replaceTo = match.captured("innerCode");
				replaceSelectedText(replaceTo);
			} else {
				QString replaceTo = QString("%1%2%3")
						.arg(mLanguage.multilineCommentStart).arg(selectedText).arg(mLanguage.multilineCommentEnd);
				replaceSelectedText(replaceTo);
			}
		}

		SendScintilla(SCI_SETCURRENTPOS, startSelectedPosition);
	} else {
		// third case: comment single line
		int currentPosition = static_cast<int>(SendScintilla(SCI_GETCURRENTPOS));
		int currentPositionLine = static_cast<int>(SendScintilla(SCI_LINEFROMPOSITION, currentPosition));
		int currentPositionLineEnd = static_cast<int>(SendScintilla(SCI_GETLINEENDPOSITION, currentPositionLine));
		int endLineIndex = -1;
		int currentPositionIndex = -1;
		lineIndexFromPosition(currentPositionLineEnd, &currentPositionLine, &endLineIndex);
		lineIndexFromPosition(currentPosition, &currentPositionLine, &currentPositionIndex);
		setSelection(currentPositionLine, 0, currentPositionLine, endLineIndex);
		const QString selectedText = this->selectedText();
		const QRegularExpressionMatch match = lineRegExp.match(selectedText);
		if (match.hasMatch()) {
			QString replaceTo(QString(match.capturedRef("indent").toString()
					+ match.capturedRef("innerCode").toString()));
			replaceSelectedText(replaceTo);
		} else {
			QString replaceTo;
			replaceTo.append(QString("%1%2%3").arg(mLanguage.lineCommentStart)
					.arg(selectedText).arg(mLanguage.lineCommentEnd));
			replaceSelectedText(replaceTo);
		}

		const int length = mLanguage.lineCommentStart.length();
		const int shift = match.hasMatch() ? -length : length;
		setSelection(currentPositionLine, currentPositionIndex + shift
				, currentPositionLine, currentPositionIndex + shift);
	}
}

void QScintillaTextEdit::goToLineColumn()
{
	mSearchLinePanel->setMode(ui::SearchLinePanel::OperationOptions::GoToLineAndColumn);
	mSearchLinePanel->attachTo(this);
}

void QScintillaTextEdit::findAndReplace()
{
	mSearchLinePanel->setMode(ui::SearchLinePanel::OperationOptions::FindAndReplace);
	mSearchLinePanel->attachTo(this);
}

void QScintillaTextEdit::emitTextWasModified()
{
	cancelList();
	setMarginWidth(1, QString::number(lines()).size() * mAverageCharWidth + 10);
	mSearchLinePanel->update();
	emit textWasModified(this);
}

void QScintillaTextEdit::initFindModeConnections()
{
	mSearchLinePanel = new ui::SearchLinePanel(this);

	auto findlambda = [this](bool forward){
		if (mLastSearch.isEmpty()) {
			return;
		}

		if (mSearchLinePanel->getMode() == ui::SearchLinePanel::OperationOptions::GoToLineAndColumn) {
			QRegularExpression regExp("^(?<line>\\d+)(:(?<column>\\d+))*$");
			QRegularExpressionMatch match = regExp.match(mLastSearch.pattern());
			if (not match.hasMatch()) {
				mSearchLinePanel->reportError();
			} else {
				mLastSearchCalled = true;
			}
		} else {
			int currentPosition = static_cast<int>(SendScintilla(SCI_GETCURRENTPOS));
			int currentPositionLine = static_cast<int>(SendScintilla(SCI_LINEFROMPOSITION, currentPosition));
			int currentPositionIndex = -1;
			lineIndexFromPosition(currentPosition, &currentPositionLine, &currentPositionIndex);
			bool cs = mLastSearch.caseSensitivity() == Qt::CaseSensitive;
			if (not findFirst(mLastSearch.pattern(), true, cs
					, true, true, forward, currentPositionLine, currentPositionIndex)) {
				mSearchLinePanel->reportError();
			} else {
				mLastSearchCalled = true;
			}
		}
	};

	connect(mSearchLinePanel, &ui::SearchLinePanel::nextPressed, this, [this, findlambda](){
		if (mSearchLinePanel->getMode() == ui::SearchLinePanel::OperationOptions::GoToLineAndColumn) {
			QRegularExpression regExp("^(?<line>\\d+)(:(?<column>\\d+))*$");
			QRegularExpressionMatch match = regExp.match(mSearchLinePanel->getTextForFind());
			if (not match.hasMatch()) {
				mSearchLinePanel->reportError();
				return;
			}

			int line = match.captured("line").toInt();
			int column = match.captured("column").toInt();
			if (lines() >= line) {
				mSearchLinePanel->detach();
				mSearchLinePanel->hide();
				this->setFocus(Qt::TabFocusReason);
				SendScintilla(SCI_GOTOPOS, positionFromLineIndex(line - 1, column));
			}
		} else {
			if (mSearchForward && mLastSearchCalled) {
				findNext();
			} else {
				mSearchForward = true;
				findlambda(true);
			}
		}
	});

	connect(mSearchLinePanel, &ui::SearchLinePanel::previousPressed, this, [this, findlambda](){
		if (mSearchForward || not mLastSearchCalled) {
			mSearchForward = false;
			findlambda(false);
		} else {
			findNext();
		}
	});

	connect(mSearchLinePanel, &ui::SearchLinePanel::findTextChanged, this, [this](const QRegExp &textToSearch){
		mLastSearchCalled = false;
		mLastSearch = textToSearch;
	});

	connect(mSearchLinePanel, &ui::SearchLinePanel::replacePressed, this, [this](){
		if (not selectedText().isEmpty()) {
			this->replaceSelectedText(mSearchLinePanel->getTextForReplace());
			this->findNext();
		}
	});
}
