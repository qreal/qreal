/* Copyright 2018 Konstantin Batoev
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

#include "intermediateStructurizatorNode.h"

namespace generatorBase {

class SimpleStructurizatorNode : public IntermediateStructurizatorNode
{
	Q_OBJECT

public:
	explicit SimpleStructurizatorNode(const qReal::Id &id, QObject *parent = nullptr);

	Type type() const;
	qReal::Id firstId() const;
	bool analyzeBreak();

	qReal::Id id() const;
private:
	const qReal::Id mId;
};

}
