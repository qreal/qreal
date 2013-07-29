#include "qscintillaTextEdit.h"

#include "../../thirdparty/qscintilla/Qt4Qt5/Qsci/qscilexerpython.h"

using namespace qReal;
using namespace gui;

QScintillaTextEdit::QScintillaTextEdit()
	:mRole(0)
{
	connect(this, SIGNAL(textChanged()), this, SLOT(emitTexWasModified()));
	setUtf8(true);
	setCppLexer();
}

QScintillaTextEdit::QScintillaTextEdit(QPersistentModelIndex const &index
		, int const &role)
		: mIndex(index)
		, mRole(role)
{}

QScintillaTextEdit::~QScintillaTextEdit()
{
	emit textSaved(text(), mIndex, mRole);
}

void QScintillaTextEdit::setPythonLexer()
{
	QsciLexerPython *lexer = new QsciLexerPython();
	setLexer(lexer);
}

void QScintillaTextEdit::setCppLexer()
{
	//QsciLexerCPP *lexCpp = new QsciLexerCPP(this);
	//setLexer(lexCpp);
}

void QScintillaTextEdit::emitTexWasModified()
{
	emit textWasModified(this);
}

void QScintillaTextEdit::setPythonEditorProperties()
{
	// Current line highlighting
	setCaretLineVisible(true);
	setCaretLineBackgroundColor(QColor("gainsboro"));

	// Auto indent
	setAutoIndent(true);
	setIndentationGuides(false);

	// Replace tabs with 2 spaces
	setIndentationsUseTabs(false);
	setIndentationWidth(2);

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
#if defined Q_WS_X11
	setEolMode(QsciScintilla::EolUnix);
#elif defined Q_WS_WIN
	setEolMode(QsciScintilla::EolWindows);
#elif defined Q_WS_MAC
	setEolMode(QsciScintilla::EolMac);
#endif

	// Input encoding
	setUtf8(true);
}
