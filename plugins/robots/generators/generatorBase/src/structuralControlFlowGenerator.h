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

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/controlFlowGeneratorBase.h"
#include "generatorBase/semanticTree/semanticTree.h"

#include "structurizator.h"

#include <QStack>
#include <QVector>

namespace generatorBase {

/// Generates semantic tree in control-structured style.
/// First we obtain control flow graph.
/// Then we are trying structurize such a graph with Structurizator.
/// If structurization was successfull then we are trying to transform
/// control flow tree to Semantic Tree used for code generation.
///
/// If control flow can`t be represented with structured code generation process
/// is considered to be unsuccessfull.
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
			, bool isThisDiagramMain = true
			, qReal::Id simpleId = qReal::Id());

	/// Implementation of clone operation for structural generator
	ControlFlowGeneratorBase *cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram) override;

	void beforeSearch() override;

	/// functions for visiting Ids. While each visit we contruct graph.
	void visit(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitConditional(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitUnknown(const qReal::Id &id, QList<LinkInfo> const &links) override;

	/// We introduce fake-loop-preheader vertex incident to vertices not from loop body.
	/// We remember vertices belonging to loop.
	void visitLoop(const qReal::Id &id, const QList<LinkInfo> &links) override;

	/// We clean old info about vertices belonging to some loop.
	void afterVisit(const qReal::Id &id, QList<LinkInfo> &links) override;


	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	void afterSearch() override;

	/// Returns true if some generation errors occured but the generation process may be proceeded with other
	/// control flow generators (non-fatal errors occured).
	bool cantBeGeneratedIntoStructuredCode() const;

private:
	/// Implementation of generation process for structural generator.
	/// Important: the graph in the model would be traversed two or more times
	/// to build dominators tree and then perform structural analysis
	void performGeneration() override;

	/// Rule is applied only when generation was performed so there's a ForkNode or JoinNode variable.

	/// When forkNode was obtained we register other threads to generate
	void registerOtherThreads(const qReal::Id &id, const QList<LinkInfo> &threads
			, const QHash<qReal::Id, QString> &threadIds, parts::Threads &threadsStorage) override;

	/// When JoinNode was obtained we register other threads to generate
	void registerTerminatingThreads(const qReal::Id &id, parts::Threads &threadsStorage
			, bool fromMain) override;

	void performStructurization();
	void obtainSemanticTree(IntermediateStructurizatorNode *root);

	/// helper method for ZoneNode
	void checkAndAppendBlock(semantics::ZoneNode *zone, IntermediateStructurizatorNode *node);

	/// transformation methods
	semantics::SemanticNode *transformNode(IntermediateStructurizatorNode *node);
	semantics::SemanticNode *transformSimple(SimpleStructurizatorNode *simpleNode);
	semantics::SemanticNode *transformBlock(BlockStructurizatorNode *blockNode);
	semantics::SemanticNode *transformIfThenElse(IfStructurizatorNode *ifNode);
	semantics::SemanticNode *transformSelfLoop(SelfLoopStructurizatorNode *selfLoopNode);
	semantics::SemanticNode *transformWhileLoop(WhileStructurizatorNode *whileNode);
	semantics::SemanticNode *transformSwitch(SwitchStructurizatorNode *switchNode);
	semantics::SemanticNode *transformBreakNode();

	/// helper functions
	semantics::SemanticNode *createConditionWithBreaks(StructurizatorNodeWithBreaks *nodeWithBreaks);
	semantics::SemanticNode *createSemanticIfNode(const qReal::Id &conditionId
			, IntermediateStructurizatorNode *thenNode
			, IntermediateStructurizatorNode *elseNode);

	semantics::SemanticNode *createSemanticSwitchNode(const qReal::Id &conditionId
			, const QList<IntermediateStructurizatorNode *> &branches
			, bool generateIfs);

	/// methods for building graph
	void appendVertex(const qReal::Id &vertex);
	void addEdgeIntoGraph(const qReal::Id &from, const qReal::Id &to);
	void appendEdgesAndVertices(const qReal::Id &vertex, const QList<LinkInfo> &links);

	/// methods for handling vertices belonging to loop body
	void addVerticesInLoopBody(const qReal::Id &vertex, const QList<LinkInfo> &links);
	void removeVerticesFromLoopBody(const qReal::Id &vertex, const QList<LinkInfo> &links);

	QMap<int, semantics::SemanticNode *> mTrees;
	bool mCantBeGeneratedIntoStructuredCode;
	Structurizator *mStructurizator;

	int mVerticesNumber;
	QSet<qReal::Id> mIds;
	int mStartVertex;
	QMap<qReal::Id, int> mVertexNumber;
	QMap<int, int> mLoopHeader;
	QMap<int, QSet<int>> mFollowers;
	bool mIsGraphBeingConstructed;
	QStack<int> mLoopNumbers;
	QSet<int> mVerticesInsideLoopBody;
	QVector<qReal::Id> mAdditionalVertices;
	bool mWasDoneThisIteration;
	qReal::Id mSimpleId;
};

}
