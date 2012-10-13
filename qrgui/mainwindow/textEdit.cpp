#include "textEdit.h"

#include "../../thirdparty/qscintilla/Qt4Qt5/Qsci/qscilexerpython.h"

using namespace qReal;
using namespace gui;

TextEdit::TextEdit(QPersistentModelIndex const &index, int const &role) : mIndex(index), mRole(role)
{}

TextEdit::~TextEdit()
{
	emit textSaved(text(), mIndex, mRole);
}

void TextEdit::setPythonLexer()
{
	QsciLexerPython *lexer = new QsciLexerPython();
	setLexer(lexer);
}

void TextEdit::setPythonEditorProperties()
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
