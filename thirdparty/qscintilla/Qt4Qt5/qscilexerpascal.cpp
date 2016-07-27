#include "Qsci/qscilexerpascal.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerPascal::QsciLexerPascal(QObject *parent, bool caseInsensitiveKeywords)
	: QsciLexer(parent),
	  fold_atelse(false), fold_comments(false), fold_compact(true),
	  dollars(true), nocase(caseInsensitiveKeywords)
{
}


// The dtor.
QsciLexerPascal::~QsciLexerPascal()
{
}


// Returns the language name.
const char *QsciLexerPascal::language() const
{
	return "PascalABC";
}


// Returns the lexer name.
const char *QsciLexerPascal::lexer() const
{
	return (nocase ? "cppnocase" : "cpp");
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList QsciLexerPascal::autoCompletionWordSeparators() const
{
	QStringList wl;

	wl << ".";

	return wl;
}


// Return the list of keywords that can start a block.
const char *QsciLexerPascal::blockStartKeyword(int *style) const
{
	if (style)
		*style = Keyword;

	return "case catch class default do else finally for if private "
		   "protected public struct try union while";
}


// Return the list of characters that can start a block.
const char *QsciLexerPascal::blockStart(int *style) const
{
	if (style)
		*style = Operator;

	return "{" "begin";
}


// Return the list of characters that can end a block.
const char *QsciLexerPascal::blockEnd(int *style) const
{
	if (style)
		*style = Operator;

	return "}" "end";
}


// Return the style used for braces.
int QsciLexerPascal::braceStyle() const
{
	return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerPascal::wordCharacters() const
{
	return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerPascal::defaultColor(int style) const
{
	switch (style)
	{
	case Default:
		return QColor(0x80, 0x80, 0x80);

	case CommentLine:
		return QColor(0x00, 0x7f, 0x00);

	case Number:
		return QColor(0x00, 0x7f, 0x7f);

	case Keyword:
		return QColor(0x00, 0x00, 0x7f);

	case DoubleQuotedString:
	case SingleQuotedString:
		return QColor(0x7f, 0x00, 0x7f);

	case Operator:
	case UnclosedString:
		return QColor(0x00, 0x00, 0x00);

	case InactiveDefault:
		return QColor(0xc0, 0xc0, 0xc0);

	case InactiveCommentLine:
	case InactiveNumber:
		return QColor(0x90, 0xb0, 0x90);

	case InactiveKeyword:
		return QColor(0x90, 0x90, 0xb0);

	case InactiveDoubleQuotedString:
	case InactiveSingleQuotedString:
		return QColor(0xb0, 0x90, 0xb0);


	case InactiveOperator:
	case InactiveIdentifier:
		return QColor(0xb0, 0xb0, 0xb0);

	case InactiveUnclosedString:
		return QColor(0x00, 0x00, 0x00);
	}

	return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerPascal::defaultEolFill(int style) const
{
	switch (style)
	{
	case UnclosedString:
	case InactiveUnclosedString:
		return true;
	}

	return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerPascal::defaultFont(int style) const
{
	QFont f;

	switch (style)
	{
	case CommentLine:
	case InactiveCommentLine:
#if defined(Q_OS_WIN)
		f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
		f = QFont("Comic Sans MS", 12);
#else
		f = QFont("Bitstream Vera Serif",9);
#endif
		break;

	case Keyword:
	case InactiveKeyword:
	case Operator:
	case InactiveOperator:
		f = QsciLexer::defaultFont(style);
		f.setBold(true);
		break;

	case DoubleQuotedString:
	case InactiveDoubleQuotedString:
	case SingleQuotedString:
	case InactiveSingleQuotedString:
	case UnclosedString:
	case InactiveUnclosedString:
#if defined(Q_OS_WIN)
		f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
		f = QFont("Courier", 12);
#else
		f = QFont("Bitstream Vera Sans Mono",9);
#endif
		break;

	default:
		f = QsciLexer::defaultFont(style);
	}

	return f;
}


// Returns the set of keywords.
const char *QsciLexerPascal::keywords(int set) const
{
	if (set == 1)
		return
			"abstract and array as auto begin boolean break "
			"case class const continue constructor "
			"default destructor div do downto "
			"else end event except extensionmethod external "
			"false file finalization finally for foreach forward function "
			"goto if implementation in integer inherited initialization interface internal is "
			"label lock mod new nil not of on operator or overload override "
			"params private procedure program property protected public "
			"raise read record real reintroduce repeat sealed set sequence shl shr sizeof "
			"template then to try true type typeof unit until uses using "
			"var virtual where while with write xor";

	return 0;
}


// Returns the user name of a style.
QString QsciLexerPascal::description(int style) const
{
	switch (style)
	{
	case Default:
		return tr("Default");

	case InactiveDefault:
		return tr("Inactive default");


	case CommentLine:
		return tr("C++ comment");

	case InactiveCommentLine:
		return tr("Inactive C++ comment");

	case Number:
		return tr("Number");

	case InactiveNumber:
		return tr("Inactive number");

	case Keyword:
		return tr("Keyword");

	case InactiveKeyword:
		return tr("Inactive keyword");

	case DoubleQuotedString:
		return tr("Double-quoted string");

	case InactiveDoubleQuotedString:
		return tr("Inactive double-quoted string");

	case SingleQuotedString:
		return tr("Single-quoted string");

	case InactiveSingleQuotedString:
		return tr("Inactive single-quoted string");

	case Operator:
		return tr("Operator");

	case InactiveOperator:
		return tr("Inactive operator");

	case Identifier:
		return tr("Identifier");

	case InactiveIdentifier:
		return tr("Inactive identifier");

	case UnclosedString:
		return tr("Unclosed string");

	case InactiveUnclosedString:
		return tr("Inactive unclosed string");
	}

	return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerPascal::defaultPaper(int style) const
{
	switch (style)
	{
	case UnclosedString:
	case InactiveUnclosedString:
		return QColor(0xe0,0xc0,0xe0);
	}

	return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerPascal::refreshProperties()
{
	setAtElseProp();
	setCommentProp();
	setCompactProp();
	//setPreprocProp();
	//setStylePreprocProp();
	setDollarsProp();
	//setHighlightTripleProp();
	//setHighlightHashProp();
}


// Read properties from the settings.
bool QsciLexerPascal::readProperties(QSettings &qs,const QString &prefix)
{
	int rc = true;

	fold_atelse = qs.value(prefix + "foldatelse", false).toBool();
	fold_comments = qs.value(prefix + "foldcomments", false).toBool();
	fold_compact = qs.value(prefix + "foldcompact", true).toBool();
	dollars = qs.value(prefix + "dollars", true).toBool();
	return rc;
}


// Write properties to the settings.
bool QsciLexerPascal::writeProperties(QSettings &qs,const QString &prefix) const
{
	int rc = true;

	qs.setValue(prefix + "foldatelse", fold_atelse);
	qs.setValue(prefix + "foldcomments", fold_comments);
	qs.setValue(prefix + "foldcompact", fold_compact);
	qs.setValue(prefix + "dollars", dollars);

	return rc;
}


// Set if else can be folded.
void QsciLexerPascal::setFoldAtElse(bool fold)
{
	fold_atelse = fold;

	setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerPascal::setAtElseProp()
{
	emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}


// Set if comments can be folded.
void QsciLexerPascal::setFoldComments(bool fold)
{
	fold_comments = fold;

	setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerPascal::setCommentProp()
{
	emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Set if folds are compact
void QsciLexerPascal::setFoldCompact(bool fold)
{
	fold_compact = fold;

	setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerPascal::setCompactProp()
{
	emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}

// Set if '$' characters are allowed.
void QsciLexerPascal::setDollarsAllowed(bool allowed)
{
	dollars = allowed;

	setDollarsProp();
}


// Set the "lexer.cpp.allow.dollars" property.
void QsciLexerPascal::setDollarsProp()
{
	emit propertyChanged("lexer.cpp.allow.dollars",(dollars ? "1" : "0"));
}
