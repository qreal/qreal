#pragma once

#include "../../thirdparty/qscintilla/Qt4Qt5/Qsci/qsciscintilla.h"

namespace qReal {
namespace gui {

/// TextEdit is text editor widget based on QsciScintilla (Qt version of Scintilla project) which can highlight 
/// source code on different textual programming languages, can do autocompletion and have lots of other functionalities
/// of standart code editors.
class TextEdit : public QsciScintilla
{
	Q_OBJECT

public:
	/// Assigns widget to work with python code
	void setPythonLexer();

	/// Configures some settings such as autoindentation and margin properties, brackets highlighting, etc
	void setPythonEditorProperties();
};

}
}
