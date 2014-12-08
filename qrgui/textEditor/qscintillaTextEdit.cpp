#include "qscintillaTextEdit.h"

using namespace qReal;
using namespace text;

QScintillaTextEdit::QScintillaTextEdit()
	: mRole(0)
{
	init();
}

QScintillaTextEdit::QScintillaTextEdit(QPersistentModelIndex const &index, int const &role)
	: mIndex(index)
	, mRole(role)
{
	init();
}

QScintillaTextEdit::~QScintillaTextEdit()
{
	emit textSaved(text(), mIndex, mRole);
}

LanguageInfo const &QScintillaTextEdit::currentLanguage() const
{
	return *mLanguage;
}

void QScintillaTextEdit::setCurrentLanguage(LanguageInfo const &language)
{
	setLexer(0);
	delete mLanguage;

	mLanguage = &language;
	setIndentationsUseTabs(mLanguage->tabIndentation);
	setIndentationWidth(mLanguage->tabSize);
	setLexer(mLanguage->lexer);
}

void QScintillaTextEdit::init()
{
	connect(this, &QsciScintilla::textChanged, this, &QScintillaTextEdit::emitTextWasModified);
	setDefaultSettings();
	setCurrentLanguage(Languages::textFileInfo("*.txt"));
}

void QScintillaTextEdit::setDefaultSettings()
{
	// Current line highlighting
	setCaretLineVisible(true);
	setCaretLineBackgroundColor(QColor("gainsboro"));

	// Auto indent
	setAutoIndent(true);
	setIndentationGuides(false);

	// Whitespaces visibility
	setWhitespaceVisibility(QsciScintilla::WsVisible);

	// Show margin with line numbers (up to 1000)
	setMarginsBackgroundColor(QColor("gainsboro"));
	setMarginLineNumbers(1, true);
	setMarginWidth(1, QString("1000"));

	// Autocompletion of lexems
	setAutoCompletionSource(QsciScintilla::AcsAll);
	setAutoCompletionCaseSensitivity(true);
	setAutoCompletionReplaceWord(true);
	setAutoCompletionShowSingle(true);
	setAutoCompletionThreshold(2);

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
}

void QScintillaTextEdit::emitTextWasModified()
{
	emit textWasModified(this);
}
