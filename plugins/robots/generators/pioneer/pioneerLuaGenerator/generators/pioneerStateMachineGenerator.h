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

#include <generatorBase/gotoControlFlowGenerator.h>

namespace pioneer {
namespace lua {

/// Generator for state machine for Pioneer quadcopter.
///
/// Programs for Pioneer are reactive, reacting on events from autopilot like reaching given point or given altitude.
/// Program can be in some state and by current state and current event can execute an event handler. Handler can send
/// some more commands to autopilot, wait (synchronously? Specs are not clear about it) and ultimately move system
/// to a next state.
///
/// Generator requires some advanced logic because control-flow-based program shall be transformed into a set of
/// handlers, differentiating linear fragments that are executed synchronously and creating new states for nodes that
/// execute asynchronously, like "Fly to the point".
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
};

}
}
