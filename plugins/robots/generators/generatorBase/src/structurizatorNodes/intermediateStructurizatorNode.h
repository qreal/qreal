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

#include <qrkernel/ids.h>

namespace generatorBase {

class IntermediateStructurizatorNode : public QObject
{
	Q_OBJECT

public:

	enum Type {
		simple
		, block
		, ifThenElseCondition
		, switchCondition
		, infiniteloop
		, whileloop
		, breakNode
		, nodeWithBreaks
	};

	explicit IntermediateStructurizatorNode(QObject *parent);
	virtual ~IntermediateStructurizatorNode();

	virtual Type type() const = 0;
	virtual qReal::Id firstId() const = 0;
	virtual bool analyzeBreak() = 0;
	bool hasBreakInside() const;

protected:
	bool mHasBreakInside;
	bool mBreakWasAnalyzed;
};

}
