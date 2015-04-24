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
	QScintillaTextEdit(const QPersistentModelIndex &index, const int &role);

	/// Destructor which runs after closing the tab with text editor. Allows to save written text.
	~QScintillaTextEdit();

	/// Returns a reference to an object that keeps current editor settings.s
	LanguageInfo currentLanguage() const;

	/// Applies language editor settings to current editor instance.
	/// Takes ownership on passed language.
	void setCurrentLanguage(const LanguageInfo &language);

signals:
	/// Emitted in destructor to save written text to according model element
	void textSaved(const QString &text, const QPersistentModelIndex &index, const int &role);

	void textWasModified(text::QScintillaTextEdit *code);

private slots:
	void emitTextWasModified();

private:
	void init();
	/// Configures some settings such as autoindentation and margin properties, brackets highlighting, etc
	void setDefaultSettings();

	const QPersistentModelIndex mIndex;
	const int mRole;
	LanguageInfo mLanguage;
};

}
}
