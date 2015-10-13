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

#include "conditionalNode.h"

namespace generatorBase {
namespace semantics {

/// A semantic node for loops inside program
class LoopNode : public ConditionalNode
{
public:
	explicit LoopNode(const qReal::Id &idBinded, QObject *parent = 0);

	void appendChildren(QLinkedList<SemanticNode *> const &nodes);

	/// Switches between while-do and do-while forms.
	void setForm(bool doWhileForm);

	ZoneNode *bodyZone();

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	ZoneNode *mBodyZone;  // Takes ownership
	bool mDoWhileForm;
};

}
}
