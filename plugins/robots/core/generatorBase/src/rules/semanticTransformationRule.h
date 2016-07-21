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

#include <QtCore/QObject>

#include <qrutils/graphUtils/deepFirstSearcher.h>
#include "generatorBase/semanticTree/semanticTree.h"

namespace generatorBase {
namespace semantics {

/// A base for all commands that perform semantic tree transformations.
class SemanticTransformationRule
{
public:
	typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

	/// Returns true if rule has been applied successfully; otherwise returns false
	virtual bool apply() = 0;

protected:
	SemanticTransformationRule(SemanticTree *tree, const qReal::Id &id);
	virtual ~SemanticTransformationRule();

	LoopNode *makeLoopStartingFrom(NonZoneNode *node);

	/// Returns true if semantic node for the given id was already created.
	bool alreadyCreated(const qReal::Id &id);

	/// Returns true if semantic node for the link dst node was already created.
	bool alreadyCreated(const LinkInfo &link);

	SemanticTree *mTree;  // Does not take ownership
	const qReal::Id mId;
};

}
}
