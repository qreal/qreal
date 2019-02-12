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

#include <functional>
#include <QtCore/QSet>
#include <QtCore/QList>
#include <QtCore/QQueue>
#include <QtCore/QString>

#include <generatorBase/gotoControlFlowGenerator.h>

#include "semanticTreeManager.h"

namespace pioneer {
namespace lua {

/// Generator for state machine for Pioneer quadcopter.
///
/// Programs for Pioneer are reactive, reacting on events from autopilot like reaching given point or given altitude.
/// Program can be in some state and by current state and current event can execute an event handler. Handler can send
/// some more commands to autopilot, wait (synchronously) and ultimately move system to a next state.
///
/// Generator requires some advanced logic because control-flow-based program shall be transformed into a set of
/// handlers, differentiating linear fragments that are executed synchronously and creating new states for nodes that
/// execute asynchronously, like "GoToPoint", who continues execution only when quadcopter reaches given point.
class PioneerStateMachineGenerator : public generatorBase::GotoControlFlowGenerator
{
	Q_OBJECT

public:
	PioneerStateMachineGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, generatorBase::GeneratorCustomizer &customizer
			, generatorBase::PrimaryControlFlowValidator &validator
			, const qReal::Id &diagramId
			, QObject *parent = nullptr
			, bool isThisDiagramMain = true);

	/// Registers a function that will be called when generator visits a node with given id.
	/// Allows to collect additional information during generation.
	void registerNodeHook(std::function<void(const qReal::Id)> hook);

private:
	void performGeneration() override;

	void visitRegular(const qReal::Id &id, const QList<generatorBase::LinkInfo> &links) override;

	void visitConditional(const qReal::Id &id, const QList<generatorBase::LinkInfo> &links) override;

	void visitFinal(const qReal::Id &id, const QList<generatorBase::LinkInfo> &links) override;

	void visit(const qReal::Id &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links) override;

	/// Copies a linear fragment starting from semantic node with id @p from and pastes it into semantic tree as a
	/// sibling of @p after node.
	/// @returns node that ends copied synchronous fragment (goto node) or nullptr if there was error.
	generatorBase::semantics::SemanticNode * copySynchronousFragment(
			generatorBase::semantics::SemanticNode *after
			, const qReal::Id &from
			, bool withLabel);

	/// Returns true if this node is asynchronous.
	bool isAsynchronous(const generatorBase::semantics::SemanticNode * const node) const;

	/// Creates synthetic node that denotes end of asynchronous handler.
	generatorBase::semantics::SemanticNode *produceEndOfHandlerNode();

	/// Logs an error and flags that there were errors.
	void reportError(const QString &message);
	/// Logs an information.
	void addInfo(const QString &message) const;
	void reportAndExplainConditions();

	/// Return true if this is an If node.
	static bool isIf(const generatorBase::semantics::SemanticNode * const node);

	/// Returns true if this is an end-of-handler node.
	static bool isEndOfHandler(const generatorBase::semantics::SemanticNode * const node);

	/// Returns nearest right end-of-handler sibling of a given node or nullptr if no such node exists.
	generatorBase::semantics::NonZoneNode *findEndOfHandler(generatorBase::semantics::SemanticNode * const from) const;

	/// Processes deferred goto generation requests (if any). Deferred goto generation is caused by trying to generate
	/// goto for asynchronous node that has not been visited yet, so we had no idea where to transfer control.
	void doDeferredGotoGeneration(const qReal::Id &nodeId, const qReal::Id &targetId);

	/// Visits parcitular node in a semantic tree.
	void processNode(generatorBase::semantics::NonZoneNode *thisNode, const qReal::Id &target);

	/// Logs given message if trace mode is on.
	static void trace(const QString &message);

	/// Node types that have asynchronous semantics: send a command to autopilot and continue execution when this
	/// command is completed (e.g. "GoToPoint").
	QSet<QString> mAsynchronousNodes;

	/// A set of asynchronous nodes who already have their distinct states and we can directly generate transition
	/// into them. Note that labeled nodes are not nessesarily asynchronous and not all asynchronous nodes are labeled.
	/// Actually, a node *after* asynchronous node shall be labeled, to allow to continue execution when asyncronous
	/// node will do its work.
	QSet<qReal::Id> mLabeledNodes;

	/// A list of functions that shall be called on visiting each node.
	QList<std::function<void(const qReal::Id)>> mNodeHooks;

	/// Helper object for more convenient work with semantic tree.
	QScopedPointer<SemanticTreeManager> mSemanticTreeManager;

	/// A storage for nodes that are waiting for generation of Goto statements when their target will be known.
	QMultiHash<qReal::Id, generatorBase::semantics::NonZoneNode *> mDeferredGotoNodes;

	/// Set to keep track of visited nodes by itself, because stock DFSer can visit some nodes twice (when node
	/// is a target for two non-trivial If branches, for example). Shall fix it in upstream DFS algorithm, but this
	/// can be breaking change.
	QSet<qReal::Id> mVisitedNodes;

	/// Queue to keep track conditional nodes.
	/// The boolean value indicates the fact that the asynchronous node in the branch leads to fi block.
	/// Zone nodes refer to branch nodes
	QQueue<std::tuple<
			generatorBase::semantics::SemanticNode *
			, bool
			, generatorBase::semantics::ZoneNode *
			, generatorBase::semantics::ZoneNode *>> mConditionZonesQueue;

	/// The storage show if the branch contains async node
	QHash<generatorBase::semantics::ZoneNode *, bool> mBranchAsyncMarkers;

	/// For assertion, that diagram has same number of Conditonal and End If blocks.
	int mConditionals;
	int mConditionalEnds;
};

}
}
