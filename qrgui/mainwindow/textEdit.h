#pragma once

#include "../../thirdparty/qscintilla/Qt4Qt5/Qsci/qsciscintilla.h"

namespace qReal {
namespace gui {

class TextEdit : public QsciScintilla
{
	Q_OBJECT

public:
	void setPythonLexer();
	void setPythonEditorProperties();
};

}
}

