/* Copyright 2007-2015 QReal Research Group
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

#include <QtCore/QList>

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {

/// Node that contains multiple other nodes and is used as temporary container (with connection). Shall be discarded
/// from resulting tree.
class TemporaryList : public ast::Node
{
public:
	QList<QSharedPointer<ast::Node>> &list()
	{
		return mList;
	}

private:
	QList<QSharedPointer<ast::Node>> mList;
};

}
}
