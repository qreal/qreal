#include "modelTreeItem.h"
#include "../kernel/exception/exception.h"

#include "QDebug"

using namespace qReal;
using namespace model;
using namespace details;

ModelTreeItem::ModelTreeItem(Id const &id, ModelTreeItem *parent)
	: mId(id)
{
	mParent = parent;
}

Id ModelTreeItem::id() const
{
	return mId;
}

ModelTreeItem* ModelTreeItem::parent() const
{
	return mParent;
}

void ModelTreeItem::setParent(ModelTreeItem *parent)
{
	mParent = parent;
}

ModelTreeItem::PointerList ModelTreeItem::children() const
{
	return mChildren;
}

void ModelTreeItem::addChild(ModelTreeItem *child)
{
	if (!mChildren.contains(child))
		mChildren.append(child);
	else
		throw Exception("Model: Adding already existing child " + child->id().toString() + "  to object " + mId.toString());
}

void ModelTreeItem::removeChild(ModelTreeItem *child)
{
	if (mChildren.contains(child))
		mChildren.removeAll(child);
	else
		throw Exception("Model: Removing nonexistent child " + child->id().toString() + "  from object " + mId.toString());
}

int ModelTreeItem::row()
{
	return mParent->children().indexOf(this);
}

void ModelTreeItem::clearChildren()
{
	mChildren.clear();
}
