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

	struct Link {
	public:
		Link();
		Link(const QString &text);
		QString mLinkName;
		bool mFromSwitch;
		bool mFromIf;
	};

private:
	/// Implementation of generation process for structural generator.
	/// Important: the graph in the model would be traversed two or more times
	/// to build dominators tree and then perform structural analysis
	void performGeneration() override;

	/// methods to identify patterns for structural analysis
	bool isBlock(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	bool isIfThenElse(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
//	bool isIfThenElse(const qReal::Id &id, Region &region);
//	bool isSwitch(const qReal::Id &id, Region &region);
//	bool isSelfLoop(const qReal::Id &id, Region &region);
//	bool isWhileLoop(const qReal::Id &id, Region &region);
//	bool isDoWhileLoop(const qReal::Id &id, Region &region);

	/// Creation of a new abstract node and maping it with a corresponding Id
	void initVerteces(const qReal::Id &id, const QList<LinkInfo> &links);

	/// method that tries consequentially identify patterns and reduce them for a given id
	//void identifyPatterns(const qReal::Id &id);

	/// Building abstract graph view for further analysis
	void buildGraph();
	void findDominators();
	void createInitialSemanticNodes();
	void findStartVertex();
	void dfs(int v, int &currentTime);

	int numberOfOutgoingEdges(int v);
	int numberOfIncomingEdges(int v);
	/// Replacing some verteces with a new one and proper maintenance of edges
	void replace(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &vertecesRoles);
	void updateEdges(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void updatePostOrder(int newNodeNumber, QSet<int> &verteces);
	void updateDominators(int newNodeNumber, QSet<int> &verteces);
	void updateVerteces(int newNodeNumber, QSet<int> &verteces);

	bool containsEdgeWithoutGuard(int v, int u);
	/// methods for creating a valid Semantic nodes for particular pattern
	void reduceBlock(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	//void reduceIfThenElse(const qReal::Id &id, Region &region);
	//void reduceSelfLoop(const qReal::Id &id, Region &region);
	//void reduceWhileLoop(const qReal::Id &id, Region &region);
	//void reduceDoWhileLoop(const qReal::Id &id, Region &region);
	//void reduceSwitch(const qReal::Id &id, Region &region, Region &guards);

	QString getCondition(const qReal::Id &id, const QString &edgeText);
	int getTrueLink(int v);

	QMap<int, bool> mUsed;
	bool mSomethingChanged;
	int mStartVertex;
	int mMaxPostOrderTime;
	QMap<int, int> mPostOrder;

	QMap<int, Link> mEdges;
	QSet<int> mVerteces;
	QMap<int, QMap<int, QVector<int>>> mFollowers2;
	QMap<int, QMap<int, QVector<int>>> mPredecessors2;

	int mVertecesNumber;
	int mEdgesNumber;
	bool isPerformingGeneration;
	QMap<int, QSet<int>> mDominators;

	QMap<qReal::Id, int> mMapVertexLabel;
	QMap<int, qReal::Id> mMapId;

	QMap<int, semantics::SemanticNode *> mTrees;
	bool mCantBeGeneratedIntoStructuredCode;
};

}


