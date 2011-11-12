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
	PointerList children() const;

	int row();
	void addChild(AbstractModelItem *child);
	void removeChild(AbstractModelItem *child);
	void clearChildren();

	void setParent(AbstractModelItem *parent);

	/// Stacks item element before sibling (they should have the same parent)
	void stackBefore(AbstractModelItem *element, AbstractModelItem *sibling);

private:
	AbstractModelItem *mParent;
	const Id mId;
	PointerList mChildren;
};

}

}

}

}
