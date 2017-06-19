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

#include <QtCore/QSet>
#include <QtCore/QString>

#include <generatorBase/gotoControlFlowGenerator.h>

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
public:
	PioneerStateMachineGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, generatorBase::GeneratorCustomizer &customizer
			, generatorBase::PrimaryControlFlowValidator &validator
			, const qReal::Id &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);

private:
	void visitRegular(const qReal::Id &id, const QList<generatorBase::LinkInfo> &links) override;

	/// Copies a linear fragment starting from semantic node with id @p from and pastes it into semantic tree as a
	/// sibling of @p after node.
	void copySynchronousFragment(generatorBase::semantics::SemanticNode *after, const qReal::Id from, bool withLabel);

	/// Copies right siblings of the @p node until first labeled node, or until end.
	const QLinkedList<generatorBase::semantics::SemanticNode *> copyRightSiblingsUntilAsynchronous(
			generatorBase::semantics::NonZoneNode *node);

	/// Creates node with label for a given id. Does not transfer ownership.
	generatorBase::semantics::NonZoneNode *produceLabeledNode(const qReal::Id block);

	/// Returns true if this node is asynchronous.
	bool isAsynchronous(const generatorBase::semantics::SemanticNode * const node) const;

	/// Returns true if this node is synthetic, i.e. does not have corresponding block on a diagram.
	bool isSynthetic(const generatorBase::semantics::SemanticNode * const node) const;

	/// Finds first sibling of a given node that corresponds to asynchronous block. Returns nullptr if there is no
	/// such node.
	generatorBase::semantics::SemanticNode * findAsynchronousSibling(
			generatorBase::semantics::NonZoneNode *node) const;

	/// Returns first non-synthetic right sibling of a given node. Returns nullptr if there is no such sibling.
	generatorBase::semantics::SemanticNode *findRightSibling(
			generatorBase::semantics::NonZoneNode * const node) const;

	/// Node types that have asynchronous semantics: send a command to autopilot and continue execution when this
	/// command is completed (e.g. "GoToPoint").
	QSet<QString> mAsynchronousNodes;

	/// A set of asynchronous nodes who already have their distinct states and we can directly generate transition
	/// into them. Note that labeled nodes are not nessesarily asynchronous and not all asynchronous nodes are labeled.
	/// Actually, a node *after* asynchronous node shall be labeled, to allow to continue execution when asyncronous
	/// node will do its work.
	QSet<qReal::Id> mLabeledNodes;
};

}
}
