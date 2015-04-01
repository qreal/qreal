#pragma once

#include <QtCore/QMap>

#include "qrgui/textEditor/textEditorDeclSpec.h"
#include <qrkernel/ids.h>

namespace qReal {
namespace text {

class QRGUI_TEXT_EDITOR_EXPORT CodeBlockManager
{
public:
	CodeBlockManager();
	void addNewCode(QString const &path);
	void removeCode(QString const &path);
	QPair<int, int> intervalById(QString const &path, Id const &id);

private:
	void readDbgFile(QString const &path);

	QMap<QString, QMap<Id, QPair<int, int> > > mBlockToCode;
};

}
}
