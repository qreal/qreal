#include "modelTreeItem.h"
#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace model;
using namespace details;

ModelTreeItem::ModelTreeItem(NewType const &id, ModelTreeItem *parent)
        : mType(id)
{
	mParent = parent;
}

NewType ModelTreeItem::type() const
{
        return mType;
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
                throw Exception("Model: Adding already existing child " + child->type().toString() + "  to object " + mType.toString());
}

void ModelTreeItem::removeChild(ModelTreeItem *child)
{
	if (mChildren.contains(child))
		mChildren.removeAll(child);
	else
                throw Exception("Model: Removing nonexistent child " + child->type().toString() + "  from object " + mType.toString());
}

int ModelTreeItem::row()
{
	return mParent->children().indexOf(this);
}

void ModelTreeItem::clearChildren()
{
	mChildren.clear();
}
