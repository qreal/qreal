/* Copyright 2017 QReal Research Group
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

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/controlFlowGeneratorBase.h"
#include "generatorBase/semanticTree/semanticTree.h"
#include "rules/semanticTransformationRule.h"

#include "generatorBase/node.h"

using graphUtils::Node;
using graphUtils::RegionType;
//using graphUtils::VertexIterator;
//using graphUtils::VertexLabel;

namespace generatorBase {

class ROBOTS_GENERATOR_EXPORT StructuralControlFlowGenerator : public ControlFlowGeneratorBase
{
	Q_OBJECT

public:
	StructuralControlFlowGenerator(
			const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, PrimaryControlFlowValidator &validator
			, const qReal::Id &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);

	/// Implementation of clone operation for readable generator
	ControlFlowGeneratorBase *cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram) override;

	void beforeSearch() override;

	void visitFinal(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFork(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitJoin(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitRegular(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitConditional(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitLoop(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitUnknown(const qReal::Id &id, QList<LinkInfo> const &links) override;

	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	void afterSearch() override;

	/// Returns true if some generation errors occured but the generation process may be proceeded with other
	/// control flow generators (non-fatal errors occured).
	//bool cantBeGeneratedIntoStructuredCode() const;

private:

	QVector<Node *> allNodes;

	/// Implementation of generation process for structural generator.
	/// Important: the graph in the model would be traversed two or more times
	/// to build dominators tree and then perform structural analysis
	void performGeneration() override;

	void buildGraph(const qReal::Id &id, const QList<utils::DeepFirstSearcher::LinkInfo> &links);
	void performAnalysis();
	// naive approach for finding dominators
	void findDominators();
	void dfs(graphUtils::Node *v, int &postOrderLabel);

	RegionType determineAcyclicRegionType(graphUtils::Node* &node, QVector<graphUtils::Node *> &nodesThatComposeRegion);
	RegionType determineCyclicRegionType(graphUtils::Node* &node, QVector<graphUtils::Node *> &reachUnder);

	Node * reduce(RegionType type, QVector<Node *> &nodesThatComposeRegion);
	void replace(Node *node, QVector<Node *> &nodesThatComposeRegion);
	void compact(Node *node, QVector<Node *> &nodesThatComposeRegion);

	QVector<Node *> countReachUnder(Node *currentNode);

	int mNumberOfVerteces;
	int mCurrentTime;
	int mMaxTime;
	Node *mEntry;
	QList<Node *> mVerteces;
	QMap<qReal::Id, Node *> mInitialVerteces;
	QMap<Node, QVector<Node *> > mFollowers;
	QMap<Node, QVector<Node *> > mPredecessors;
	QMap<Node, QSet<Node *>> mDominators;
	QMap<Node, int> mPostOrder;
	QMap<Node, bool> mUsed;

	int mCounter;
};

}


