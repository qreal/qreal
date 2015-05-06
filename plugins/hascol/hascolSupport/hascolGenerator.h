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

#include <QtCore/QString>

#include <qrkernel/ids.h>
#include <qrutils/outFile.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace hascol {

/// Generator of Hascol source code
class HascolGenerator
{
public:
	/// Constructor
	/// @param api Non-mutable logical repo API, a source for generation
	/// @param errorReporter Where to put various error and warning messages
	HascolGenerator(qrRepo::LogicalRepoApi const &api, qReal::ErrorReporterInterface &errorReporter);

	/// Starts generation
	void generate();

private:
	/// Generates structure diagram
	/// @param id Id of a structure diagram that needs to be generated
	void generateDiagram(qReal::Id const &id);

	/// Generates hascol process on structure diagram
	/// @param id Id of a Process node
	/// @param out Output file
	void generateProcess(qReal::Id const &id, utils::OutFile &out);

	/// Generates hascol process contents --- data and plug declarations,
	/// let bindings, handlers (using Activity diagrams) and port maps for that process
	/// (using port mapping diagrams)
	/// @param id Id of a Process node
	/// @param out Output file
	void generateProcessTypeBody(qReal::Id const &id, utils::OutFile &out);

	/// Generates process operation declaration
	/// @param id Id of an operation node
	/// @param out Output file
	void generateProcessOperation(qReal::Id const &id, utils::OutFile &out);

	/// Generates let binding
	/// @param id Id of a LetBinding node
	/// @param out Output file
	void generateLetBinding(qReal::Id const &id, utils::OutFile &out);

	/// Generates resource declaration
	/// @param id Id of a Resource node
	/// @param first True, if it is a first resource in a sequence
	/// @param out Output file
	void generateResource(qReal::Id const &id, bool first, utils::OutFile &out);

	/// Generates handler body for a process
	/// @param id Id of an activity diagram with handler bodies
	/// @param out Output file
	void generateActivity(qReal::Id const &id, utils::OutFile &out);

	/// Generates body of a handler
	/// @param id Id of a HandlerStart node
	/// @param out Output file
	void generateHandler(qReal::Id const &id, utils::OutFile &out);

	/// Generates hascol functor on structure diagram
	/// @param id Id of a Functor node
	/// @param out Output file
	void generateFunctor(qReal::Id const &id, utils::OutFile &out);

	/// Generates formal parameter declaration for a functor
	/// @param id Id of a parameter node
	/// @param out Output file
	void generateFunctorFormalParameter(qReal::Id const &id, utils::OutFile &out);

	/// Generates port map for a hascol process ("with" clause) by port map diagram
	/// @param id Id of a port map diagram that needs to be generated
	/// @param out Output file
	void generatePortMap(qReal::Id const &id, utils::OutFile &out);

	/// Generates chain of successive statements
	/// @param id Id of a first statement in a chain
	/// @param out Output file
	qReal::Id generateChain(qReal::Id const &startNode, utils::OutFile &out);

	/// Generates one action
	/// @param id Id of an action
	/// @param out Output file
	void generateActivityNode(qReal::Id const &id, utils::OutFile &out);

	/// Generates If statement
	/// @param id Id of an If node
	/// @param out Output file
	qReal::Id generateIf(qReal::Id const &id, utils::OutFile &out);

	/// Logical repo API containing all information required by generator
	qrRepo::LogicalRepoApi const &mApi;

	/// ErrorReporter object to which error and warning messages are put
	qReal::ErrorReporterInterface &mErrorReporter;

	/// List of port mapping diagrams present in a project. Used to quickly find needed diagram
	qReal::IdList mPortMappingDiagrams;

	/// List of activity diagrams present in a project.
	qReal::IdList mActivityDiagrams;
};

}
