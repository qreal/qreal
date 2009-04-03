#include "ModelTreeItem.h"
#include "../Service/Exception/Exception.h"

using namespace qReal;

using namespace model;

ModelTreeItem::ModelTreeItem( IdType id, ModelTreeItem *parent ) : mId(id)
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
		throw Exception("Model: Adding already existing child " + child->id() + "  to object " + mId);
	}
}

void ModelTreeItem::removeChild( ModelTreeItem *child )
{
	if (!mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Model: Removing nonexistent child " + child->id() + "  from object " + mId);
	}
}

int ModelTreeItem::row()
{
	return mParent->children().indexOf(this);
}