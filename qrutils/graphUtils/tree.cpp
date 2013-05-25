#include "tree.h"

using namespace qReal;

Tree::Tree()
{
}

QListIterator<Tree *> Tree::children() const
{
	return QListIterator<Tree *>(mChildren);
}

void Tree::appendChild(Tree * const child)
{
	if (child) {
		mChildren << child;
	}
}

void Tree::insertChild(int index, Tree * const child)
{
	if (child) {
		mChildren.insert(index, child);
	}
}

void Tree::removeChild(Tree * const child)
{
	if (child) {
		mChildren.removeOne(child);
	}
}
