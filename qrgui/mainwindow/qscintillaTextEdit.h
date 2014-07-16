#pragma once

#include <QtCore/QPersistentModelIndex>

#include <thirdparty/qscintilla/Qt4Qt5/Qsci/qsciscintilla.h>

namespace qReal {
namespace gui {

/// QScintillaTextEdit is text editor widget based on QsciScintilla (Qt version of Scintilla project) which
/// can highlight source code on different textual programming languages, can do autocompletion and have lots
/// of other functionalities of standart code editors.

class QScintillaTextEdit : public QsciScintilla
{
	Q_OBJECT

public:
	QScintillaTextEdit();
	QScintillaTextEdit(QPersistentModelIndex const &index, int const &role);

	/// Destructor which runs after closing the tab with text editor. Allows to save written text.
	~QScintillaTextEdit();

	/// Assigns widget to work with python code
	void setPythonLexer();

	/// Configures some settings such as autoindentation and margin properties, brackets highlighting, etc
	void setPythonEditorProperties();

	void setCppLexer();

signals:
	/// Emitted in destructor to save written text to according model element
	void textSaved(QString const &text, QPersistentModelIndex const &index, int const &role);

	void textWasModified(gui::QScintillaTextEdit *code);

private slots:
	void emitTextWasModified();

private:
	QPersistentModelIndex const mIndex;
	int const mRole;
};

}
}
