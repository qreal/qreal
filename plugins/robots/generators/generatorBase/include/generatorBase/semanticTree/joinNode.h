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

#include "nonZoneNode.h"

namespace generatorBase {
namespace semantics {

/// A semantic node for a block of join semantics.
class JoinNode : public NonZoneNode
{
public:
	explicit JoinNode(const qReal::Id &idBinded, QObject *parent = 0);

	/// Sets a main thread for this join (the one which will continue execution after join).
	void setThreadId(const QString &threadId);

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	QString mThreadId;
};

}
}
