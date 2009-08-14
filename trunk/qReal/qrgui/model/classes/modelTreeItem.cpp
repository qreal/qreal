#include "modelTreeItem.h"
#include "../kernel/exception/exception.h"

using namespace qReal;

using namespace model;

ModelTreeItem::ModelTreeItem( IdType const &id, ModelTreeItem *parent ) : mId(id)
{
	mParent = parent;
}

IdType ModelTreeItem::id() const
{
	return mId;
}

ModelTreeItem * ModelTreeItem::parent() const
{
	return mParent;
}

void ModelTreeItem::setParent( ModelTreeItem *parent )
{
	mParent = parent;
}

ModelTreeItemPointerList ModelTreeItem::children() const
{
	return mChildren;
}

void ModelTreeItem::addChild( ModelTreeItem *child )
{
	if (!mChildren.contains(child)) {
		mChildren.append(child);
	} else {
		throw Exception("Model: Adding already existing child " + child->id().toString() + "  to object " + mId.toString());
	}
}

void ModelTreeItem::removeChild( ModelTreeItem *child )
{
	if (!mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Model: Removing nonexistent child " + child->id().toString() + "  from object " + mId.toString());
	}
}

int ModelTreeItem::row()
{
	return mParent->children().indexOf(this);
}
