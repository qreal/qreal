#pragma once

#include "../../../../qrkernel/ids.h"

namespace qReal {

namespace models {

namespace details {

namespace modelsImplementation {

class AbstractModelItem
{
public:
	typedef QList<AbstractModelItem*> PointerList;

	AbstractModelItem(Id const &id, AbstractModelItem *parent);
	Id id() const;
	AbstractModelItem *parent() const;
	void setParent(AbstractModelItem *parent);
	PointerList children() const;
	void addChild(AbstractModelItem *child);
	void removeChild(AbstractModelItem *child);
	int row();
	void clearChildren();

private:
	AbstractModelItem *mParent;
	const Id mId;
	PointerList mChildren;
};

}

}

}

}
