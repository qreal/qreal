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

#include "structurizator.h"

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

	/// Implementation of clone operation for structural generator
	ControlFlowGeneratorBase *cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram) override;

	void beforeSearch() override;

	void visit(const qReal::Id &id, QList<LinkInfo> &links) override;
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
	bool cantBeGeneratedIntoStructuredCode() const;

	semantics::SemanticTree *generate(const qReal::Id &initialNode = qReal::Id(), const QString &threadId = "main");

	typedef QMap<QString, int> Region;

private:
	/// Implementation of generation process for structural generator.
	/// Important: the graph in the model would be traversed two or more times
	/// to build dominators tree and then perform structural analysis
	void performGeneration() override;

	void performStructurization();
	void obtainSemanticTree(myUtils::IntermediateNode *root);

	void checkAndAppendBlock(semantics::ZoneNode *zone, myUtils::IntermediateNode *node);
	semantics::SemanticNode *transformNode(myUtils::IntermediateNode *node);
	semantics::SemanticNode *transformSimple(myUtils::SimpleNode *simpleNode);
	semantics::SemanticNode *transformBlock(myUtils::BlockNode *blockNode);
	semantics::SemanticNode *transformIfThenElse(myUtils::IfNode *ifNode);
	semantics::SemanticNode *transformSelfLoop(myUtils::SelfLoopNode *selfLoopNode);
	semantics::SemanticNode *transformWhileLoop(myUtils::WhileNode *whileNode);
	semantics::SemanticNode *transformSwitch(myUtils::SwitchNode *switchNode);
	semantics::SemanticNode *transformBreakNode();
	semantics::SemanticNode *transformFakeCycleHead();

	semantics::SemanticNode *createConditionWithBreaks(myUtils::NodeWithBreaks *nodeWithBreaks);
	semantics::SemanticNode *createSemanticIfNode(const qReal::Id &conditionId, myUtils::IntermediateNode *thenNode, myUtils::IntermediateNode *elseNode);
	semantics::SemanticNode *createSemanticSwitchNode(const qReal::Id &conditionId, const QList<myUtils::IntermediateNode *> &branches, bool generateIfs);
	semantics::SemanticNode *createSemanticForkNode(const qReal::Id &conditionId, const QList<myUtils::IntermediateNode *> &branches, const QString &currentThread);

	void addThreadsToJoin(const QList<myUtils::IntermediateNode *> &branches, myUtils::IntermediateNode *joinNode);
	void resolveThreads(myUtils::IntermediateNode *node, const QString &currentThreadName);

	void appendVertex(const qReal::Id &vertex);
	//generatorBase::semantics::IfNode *createIfFromSwitch(int v, int bodyNumber);
	//QString constructConditionFromSwitch(const qReal::Id &id, const QList<qReal::Id> &links) const;

	QMap<int, semantics::SemanticNode *> mTrees;
	bool mCantBeGeneratedIntoStructuredCode;
	Structurizator *mStructurizator;


	int mVerticesNumber;
	QSet<qReal::Id> mIds;
	qReal::Id mStartVertex;
	QMap<qReal::Id, int> mVertexNumber;
	QMap<int, QSet<int> > mFollowers;
};

}


