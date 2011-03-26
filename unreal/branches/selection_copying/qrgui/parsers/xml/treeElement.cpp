#include "treeElement.h"

using namespace qReal;
using namespace parsers;

TreeElement::TreeElement(const Id &id) : mId(id)
{
}

void TreeElement::addChild(TreeElement *const child)
{
	mChildren.append(child);
}
