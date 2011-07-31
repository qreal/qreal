#include "abstractModelItem.h"

#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace models::details::modelsImplementation;

AbstractModelItem::AbstractModelItem(Id const &id, AbstractModelItem *parent)
	: mId(id)
{
	mParent = parent;
}

Id AbstractModelItem::id() const
{
	return mId;
}

AbstractModelItem* AbstractModelItem::parent() const
{
	return mParent;
}

void AbstractModelItem::setParent(AbstractModelItem *parent)
{
	mParent = parent;
}

AbstractModelItem::PointerList AbstractModelItem::children() const
{
	return mChildren;
}

void AbstractModelItem::addChild(AbstractModelItem *child, int beforePosition)
{
	if (mChildren.contains(child))
		throw Exception("Model: Adding already existing child " + child->id().toString() + "  to object " + mId.toString());

	if(beforePosition < 0){
		mChildren.append(child);
	} else {
		mChildren.insert(beforePosition, child);
	}
}

void AbstractModelItem::removeChild(AbstractModelItem *child)
{
	if (mChildren.contains(child))
		mChildren.removeAll(child);
	else
		throw Exception("Model: Removing nonexistent child " + child->id().toString() + "  from object " + mId.toString());
}

int AbstractModelItem::row()
{
	return mParent->children().indexOf(this);
}

void AbstractModelItem::clearChildren()
{
	mChildren.clear();
}
