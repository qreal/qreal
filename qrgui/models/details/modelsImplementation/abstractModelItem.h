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

#pragma once

#include <qrkernel/ids.h>

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class AbstractModelItem
{
public:
	virtual ~AbstractModelItem();

	typedef QList<AbstractModelItem*> PointerList;

	AbstractModelItem(const Id &id, AbstractModelItem *parent);
	Id id() const;
	AbstractModelItem *parent() const;
	PointerList children() const;

	int row() const;
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
