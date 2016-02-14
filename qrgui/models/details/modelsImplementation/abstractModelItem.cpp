/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "abstractModelItem.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace models::details::modelsImplementation;

AbstractModelItem::~AbstractModelItem()
{
}

AbstractModelItem::AbstractModelItem(const Id &id, AbstractModelItem *parent)
	: mParent(parent), mId(id)
{
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

void AbstractModelItem::addChild(AbstractModelItem *child)
{
	if (mChildren.contains(child)) {
		throw Exception("Model: Adding already existing child " + child->id().toString()
				+ "  to object " + mId.toString());
	}

	mChildren.append(child);
}

void AbstractModelItem::removeChild(AbstractModelItem *child)
{
	if (mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Model: Removing nonexistent child " + child->id().toString()
				+ "  from object " + mId.toString());
	}
}

void AbstractModelItem::stackBefore(AbstractModelItem *element, AbstractModelItem *sibling)
{
	if (element == sibling) {
		return;
	}

	if (!mChildren.contains(element)) {
		throw Exception("Model: Trying to move nonexistent child " + element->id().toString());
	}

	if (!mChildren.contains(sibling)) {
		throw Exception("Model: Trying to stack element before nonexistent child " + sibling->id().toString());
	}

	mChildren.removeOne(element);
	mChildren.insert(mChildren.indexOf(sibling), element);
}

int AbstractModelItem::row() const
{
	return mParent->children().indexOf(const_cast<AbstractModelItem *>(this));
}

void AbstractModelItem::clearChildren()
{
	mChildren.clear();
}
