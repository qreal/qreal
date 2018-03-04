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

using graphUtils::RegionType;
using graphUtils::VertexLabel;
using graphUtils::Region;

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

	void visit(const qReal::Id &id, const QList<LinkInfo> &links) override;
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

	semantics::SemanticTree *generate(const qReal::Id &initialNode = qReal::Id(), const QString &threadId = "main");

private:
	/// Implementation of generation process for structural generator.
	/// Important: the graph in the model would be traversed two or more times
	/// to build dominators tree and then perform structural analysis
	void performGeneration() override;



	bool isBlock(const qReal::Id &id, QVector<int> &region);
	bool isIfThen(const qReal::Id &id, QVector<int> &region);
	bool isIfThenElse(const qReal::Id &id, QVector<int> &region);
	bool isSwitch(const qReal::Id &id, QVector<int> &region);
	bool isSelfLoop(const qReal::Id &id, QVector<int> &region);
	bool isWhileLoop(const qReal::Id &id, QVector<int> &region);

	void updateVerteces(const qReal::Id &id, const QList<LinkInfo> &links);
	void dummyReduceFunction(int vertexLabel);
	void buildGraph();



	int mVerteces;
	bool isPerformingGeneration;
	QMap<qReal::Id, int> mMapIdToVertexLabel;
	QMap<int, qReal::Id> mMapVertexLabelToId;

	QMap<int, QVector<int> > mFollowers;
	QMap<int, QVector<int> > mPredecessor;

	QMap<int, semantics::SemanticNode *> mTrees;
	//QMap<VertexLabel, semantics::SemanticNode *> mFollowers;
};

}


