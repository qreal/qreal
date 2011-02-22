#pragma once

#include <QtCore/QList>
#include "../../kernel/NewType.h"

namespace qrRepo {
class RepoApi;
}

namespace qReal {

class EditorManager;

namespace parsers {

class TreeElement
{
public:
	TreeElement(const NewType &id);
	void addChild(TreeElement *const child);

private:
	QList<TreeElement*> mChildren;
	NewType mType;
};
}
}

