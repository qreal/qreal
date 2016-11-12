#ifndef QSCILEXERPASCAL_H
#define QSCILEXERPASCAL_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <qobject.h>

#include "qsciglobal.h"
#include "qscilexer.h"


//! \brief The QsciLexerCPP class encapsulates the Scintilla Pascal
//! lexer.
class QSCINTILLA_EXPORT QsciLexerPascal : public QsciLexer
{
	Q_OBJECT

public:
	enum {
		//! The default.
		Default = 0,
		InactiveDefault = Default + 64,

		//! A comment line.
		CommentLine = 2,
		InactiveCommentLine = CommentLine + 64,

		//! A number.
		Number = 4,
		InactiveNumber = Number + 64,

		//! A keyword.
		Keyword = 5,
		InactiveKeyword = Keyword + 64,

		//! A double-quoted string.
		DoubleQuotedString = 6,
		InactiveDoubleQuotedString = DoubleQuotedString + 64,

		//! A single-quoted string.
		SingleQuotedString = 7,
		InactiveSingleQuotedString = SingleQuotedString + 64,

		//! An operator.
		Operator = 10,
		InactiveOperator = Operator + 64,

		//! An identifier
		Identifier = 11,
		InactiveIdentifier = Identifier + 64,

		//! The end of a line where a string is not closed.
		UnclosedString = 12,
		InactiveUnclosedString = UnclosedString + 64,
	};

	//! Construct a QsciLexerPascal with parent \a parent.  \a parent is typically
	//! the QsciScintilla instance.  \a caseInsensitiveKeywords is true if the
	//! lexer ignores the case of keywords.
	QsciLexerPascal(QObject *parent = 0, bool caseInsensitiveKeywords = false);

	//! Destroys the QsciLexerPascal instance.
	virtual ~QsciLexerPascal();

	//! Returns the name of the language.
	const char *language() const;

	//! Returns the name of the lexer.  Some lexers support a number of
	//! languages.
	const char *lexer() const;

	//! \internal Returns the character sequences that can separate
	//! auto-completion words.
	QStringList autoCompletionWordSeparators() const;

	//! \internal Returns a space separated list of words or characters in
	//! a particular style that define the end of a block for
	//! auto-indentation.  The styles is returned via \a style.
	const char *blockEnd(int *style = 0) const;

	//! \internal Returns a space separated list of words or characters in
	//! a particular style that define the start of a block for
	//! auto-indentation.  The styles is returned via \a style.
	const char *blockStart(int *style = 0) const;

	//! \internal Returns a space separated list of keywords in a
	//! particular style that define the start of a block for
	//! auto-indentation.  The styles is returned via \a style.
	const char *blockStartKeyword(int *style = 0) const;

	//! \internal Returns the style used for braces for brace matching.
	int braceStyle() const;

	//! Returns the string of characters that comprise a word.
	const char *wordCharacters() const;

	//! Returns the foreground colour of the text for style number \a style.
	//!
	//! \sa defaultPaper()
	QColor defaultColor(int style) const;

	//! Returns the end-of-line fill for style number \a style.
	bool defaultEolFill(int style) const;

	//! Returns the font for style number \a style.
	QFont defaultFont(int style) const;

	//! Returns the background colour of the text for style number \a style.
	//!
	//! \sa defaultColor()
	QColor defaultPaper(int style) const;

	//! Returns the set of keywords
	const char *keywords(int set) const;

	//! Returns the descriptive name for style number \a style.  If the
	//! style is invalid for this language then an empty QString is returned.
	//! This is intended to be used in user preference dialogs.
	QString description(int style) const;

	//! Causes all properties to be refreshed by emitting the
	//! propertyChanged() signal as required.
	void refreshProperties();

	//! Returns true if "} else {" lines can be folded.
	//!
	//! \sa setFoldAtElse()
	bool foldAtElse() const {return fold_atelse;}

	//! Returns true if multi-line comment blocks can be folded.
	//!
	//! \sa setFoldComments()
	bool foldComments() const {return fold_comments;}

	//! Returns true if trailing blank lines are included in a fold block.
	//!
	//! \sa setFoldCompact()
	bool foldCompact() const {return fold_compact;}

	//! If \a allowed is true then '$' characters are allowed in identifier
	//! names.  The default is true.
	//!
	//! \sa dollarsAllowed()
	void setDollarsAllowed(bool allowed);

	//! Returns true if '$' characters are allowed in identifier names.
	//!
	//! \sa setDollarsAllowed()
	bool dollarsAllowed() const {return dollars;}

public slots:
	//! If \a fold is true then "} else {" lines can be folded.  The
	//! default is false.
	//!
	//! \sa foldAtElse()
	virtual void setFoldAtElse(bool fold);

	//! If \a fold is true then multi-line comment blocks can be folded.
	//! The default is false.
	//!
	//! \sa foldComments()
	virtual void setFoldComments(bool fold);

	//! If \a fold is true then trailing blank lines are included in a fold
	//! block. The default is true.
	//!
	//! \sa foldCompact()
	virtual void setFoldCompact(bool fold);

protected:
	//! The lexer's properties are read from the settings \a qs.  \a prefix
	//! (which has a trailing '/') should be used as a prefix to the key of
	//! each setting.  true is returned if there is no error.
	//!
	//! \sa writeProperties()
	bool readProperties(QSettings &qs,const QString &prefix);

	//! The lexer's properties are written to the settings \a qs.
	//! \a prefix (which has a trailing '/') should be used as a prefix to
	//! the key of each setting.  true is returned if there is no error.
	//!
	//! \sa readProperties()
	bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
	void setAtElseProp();
	void setCommentProp();
	void setCompactProp();
	void setDollarsProp();

	bool fold_atelse;
	bool fold_comments;
	bool fold_compact;
	bool dollars;

	bool nocase;

	QsciLexerPascal(const QsciLexerPascal &);
	QsciLexerPascal &operator=(const QsciLexerPascal &);
};

#ifdef __APPLE__
}
#endif

#endif
