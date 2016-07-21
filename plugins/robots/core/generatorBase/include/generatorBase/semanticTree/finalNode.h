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

/// A semantic node for the block with final semantics
class FinalNode : public NonZoneNode
{
public:
	/// @param inMainDiagram Is corresponding final node placed on the main diagram.
	/// It is important to differentiate theese cases because generated code for
	/// the main diagram final block may differ from subprograms one
	/// (for example, terminating task vs 'return')
	FinalNode(const qReal::Id &idBinded, bool inMainDigram, QObject *parent = 0);

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	const bool mInMainDiagram;
};

}
}
