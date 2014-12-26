#pragma once

#include <QtCore/QPersistentModelIndex>

#include <thirdparty/qscintilla/Qt4Qt5/Qsci/qsciscintilla.h>

#include "qrgui/textEditor/textEditorDeclSpec.h"
#include "qrgui/textEditor/languageInfo.h"

namespace qReal {
namespace text {

/// QScintillaTextEdit is text editor widget based on QsciScintilla (Qt version of Scintilla project) which
/// can highlight source code on different textual programming languages, can do autocompletion and have lots
/// of other functionalities of standart code editors.
class QRGUI_TEXT_EDITOR_EXPORT QScintillaTextEdit : public QsciScintilla
{
	Q_OBJECT

public:
	QScintillaTextEdit();
	QScintillaTextEdit(QPersistentModelIndex const &index, int const &role);

	/// Destructor which runs after closing the tab with text editor. Allows to save written text.
	~QScintillaTextEdit();

	/// Returns a reference to an object that keeps current editor settings.s
	LanguageInfo currentLanguage() const;

	/// Applies language editor settings to current editor instance.
	/// Takes ownership on passed language.
	void setCurrentLanguage(LanguageInfo const &language);

signals:
	/// Emitted in destructor to save written text to according model element
	void textSaved(QString const &text, QPersistentModelIndex const &index, int const &role);

	void textWasModified(text::QScintillaTextEdit *code);

private slots:
	void emitTextWasModified();

private:
	void init();
	/// Configures some settings such as autoindentation and margin properties, brackets highlighting, etc
	void setDefaultSettings();

	QPersistentModelIndex const mIndex;
	int const mRole;
	LanguageInfo mLanguage;
};

}
}
