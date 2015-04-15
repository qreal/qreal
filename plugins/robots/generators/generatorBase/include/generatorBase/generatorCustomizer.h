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

#include <qrkernel/ids.h>
#include "generatorFactoryBase.h"
#include "robotsGeneratorDeclSpec.h"

namespace generatorBase {

namespace enums {
namespace semantics {

enum Semantics {
	regularBlock = 0
	, finalBlock
	, conditionalBlock
	, loopBlock
	, switchBlock
	, forkBlock
	, joinBlock
};

}
}

/// This class must be inherited in each concrete generator. Implementation
/// must customize such aspects like blocks-semantics mapping and factory
/// for producing different generator parts.
class ROBOTS_GENERATOR_EXPORT GeneratorCustomizer
{
public:
	virtual ~GeneratorCustomizer() {}

	/// Implementation must tell if block with given id has initial node semantics
	virtual bool isInitialNode(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has final node semantics
	virtual bool isFinalNode(const qReal::Id &block) const;

	/// Implementation must tell if block with given id calls some new routine
	virtual bool isSubprogramCall(const qReal::Id &block) const;

	/// Returns semantics type of the given block
	enums::semantics::Semantics semanticsOf(const qReal::Id &block) const;

	/// Initializes everything that couldn`t be initialized in constructor
	/// (for example, everything about pure virtual methods)
	void initialize();

	/// Implementation must return a pointer to a concrete factory for a
	/// concrete generator
	virtual GeneratorFactoryBase *factory() = 0;

protected:
	GeneratorCustomizer();

	/// Implementation must tell if block with given id has if-block semantics
	virtual bool isConditional(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has loop semantics
	virtual bool isLoop(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has switch semantics
	virtual bool isSwitch(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has fork semantics
	virtual bool isFork(const qReal::Id &block) const;

	/// Implementation must tell if block with given id has join semantics
	virtual bool isJoin(const qReal::Id &block) const;

private:
	const qReal::Id mDefaultInitialBlockType;
	const qReal::Id mDefaultFinalBlockType;
	const qReal::Id mDefaultConditionalBlockType;
	const qReal::Id mDefaultLoopBlockType;
	const qReal::Id mDefaultSwitchBlockType;
	const qReal::Id mDefaultForkBlockType;
	const qReal::Id mDefaultJoinBlockType;
	const qReal::Id mDefaultSubprogramCallBlockType;
};

}
