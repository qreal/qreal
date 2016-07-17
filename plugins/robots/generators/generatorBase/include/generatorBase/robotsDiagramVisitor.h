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

#include <qrrepo/repoApi.h>
#include <qrutils/graphUtils/deepFirstSearcher.h>

#include "generatorBase/generatorCustomizer.h"

#include "robotsGeneratorDeclSpec.h"

namespace generatorBase {

/// A base for those entities who must travel through model and process every block
/// differentiating their semantics
class ROBOTS_GENERATOR_EXPORT RobotsDiagramVisitor : public utils::DeepFirstSearcher::VisitorInterface
{
public:
	RobotsDiagramVisitor(const qrRepo::RepoApi &repo, GeneratorCustomizer &customizer);
	virtual ~RobotsDiagramVisitor();

protected:
	// TODO: move this enum to global robots definitions and use it
	/// A property that used for marking links in model
	enum LinkGuard
	{
		emptyGuard = 0
		, bodyGuard  // :)
		, trueGuard
		, falseGuard
		, threadIdGuard
	};

	/// Starts repo graph dfs-traversal
	void startSearch(const qReal::Id &startingBlock);

	/// Terminates current graph traversal
	void terminateSearch();

	/// Parses guard property of given link in repo-representation and returns
	/// corresponding value of LinkGuard enumeration
	LinkGuard guardOf(const qReal::Id &linkId) const;

	/// This method is called when traverser gets into a block with regular semantics
	virtual void visitRegular(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with final-block semantics
	virtual void visitFinal(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with if semantics
	virtual void visitConditional(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with loop semantics
	virtual void visitLoop(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with switch semantics
	virtual void visitSwitch(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with fork semantics
	virtual void visitFork(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> &links) = 0;

	/// This method is called when traverser gets into a block with join semantics
	virtual void visitJoin(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> &links) = 0;

	/// This method is called when traverser gets into a block with unknown semantics
	virtual void visitUnknown(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);

private:
	virtual void visit(const qReal::Id &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links);

	const qrRepo::RepoApi &mRepo;
	GeneratorCustomizer &mCustomizer;
	utils::DeepFirstSearcher mDfser;
};

}
