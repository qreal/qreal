#include "treeElement.h"

using namespace qReal;
using namespace parsers;

TreeElement::TreeElement(const NewType &type) : mType(type)
{
}

void TreeElement::addChild(TreeElement *const child)
{
	mChildren.append(child);
}
