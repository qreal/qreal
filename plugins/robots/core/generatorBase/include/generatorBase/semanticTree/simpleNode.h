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

/// A semantic node for the block with simple semantics
class SimpleNode : public NonZoneNode
{
public:
	/// Represents all options that may be added by control flow generator
	/// artificially for more smart generation
	enum SyntheticBlockType
	{
		noSytheticBinding = 0
		, breakNode
		, continueNode
		, gotoNode
	};

	explicit SimpleNode(const qReal::Id &idBinded, QObject *parent = 0);

	/// Binds this block to given artificial construction instead of binding to id.
	void bindToSyntheticConstruction(SyntheticBlockType type);

protected:
	QLinkedList<SemanticNode *> children() const override;

	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	SyntheticBlockType mSyntheticBinding;
};

}
}
