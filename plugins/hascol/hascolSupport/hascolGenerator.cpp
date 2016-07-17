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

#include "hascolGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <qrkernel/roles.h>
#include <qrutils/outFile.h>

using namespace qReal;
using namespace hascol;
using utils::OutFile;

HascolGenerator::HascolGenerator(qrRepo::LogicalRepoApi const &api, qReal::ErrorReporterInterface &errorReporter)
		: mApi(api)
		, mErrorReporter(errorReporter)
{
}

void HascolGenerator::generate()
{
	Id const repoId = Id::rootId();
	IdList const rootDiagrams = mApi.children(repoId);

	foreach (Id const &diagram, rootDiagrams) {
		if (diagram.element() == "HascolPortMappingDiagram") {
			mPortMappingDiagrams.append(diagram);
		} else if (diagram.element() == "HascolActivityDiagram") {
			mActivityDiagrams.append(diagram);
		}
	}

	foreach (Id const &diagram, rootDiagrams) {
		if (diagram.element() == "HascolStructureDiagram") {
			generateDiagram(diagram);
		}
	}
}

void HascolGenerator::generateDiagram(Id const &id)
{
	QString outputDirectory = mApi.stringProperty(id, "output directory");
	if (outputDirectory == "") {
		outputDirectory = ".";
	}
	OutFile out(outputDirectory + "/" + mApi.name(id) + ".md");

	out() << "using bincompl;\n";  // Default signature, defines common types and operations
	out() << "\n";

	foreach (Id const &element, mApi.children(id)) {
		if (element.element() == "Process" && !mApi.name(element).isEmpty()) {
			generateProcess(element, out);
			out() << "\n";
		} else if (element.element() == "Functor") {
			generateFunctor(element, out);
			out() << "\n";
		}
	}
}

void HascolGenerator::generateProcess(Id const &id, OutFile &out)
{
	out() << "process " << mApi.name(id) << " =\n";
	generateProcessTypeBody(id, out);
}

void HascolGenerator::generateProcessTypeBody(Id const &id, utils::OutFile &out)
{
	out() << "begin\n";

	foreach (Id const &child, mApi.children(id)) {
		if (child.element() == "ProcessOperation") {
			generateProcessOperation(child, out);
		}
	}

	bool firstResource = true;
	foreach (Id const &child, mApi.children(id)) {
		if (child.element() == "Resource") {
			if (firstResource) {
				out() << "data\n";
			}

			generateResource(child, firstResource, out);
			firstResource = false;
		}
	}

	if (!firstResource) {
		out() << ";\n";
	}

	foreach (Id const &child, mApi.children(id)) {
		if (child.element() == "LetBinding") {
			generateLetBinding(child, out);
		}
	}

	foreach (Id const &link, mApi.incomingLinks(id)) {
		if (link.element() == "UsedProcessRelation") {
			Id const usedProcess = mApi.otherEntityFromLink(link, id);
			out() << "process " << mApi.name(link) << " = " << mApi.name(usedProcess) << ";\n";
		}
		if (link.element() == "NestedProcessRelation") {
			Id const nestedProcess = mApi.otherEntityFromLink(link, id);
			out() << "process " << mApi.name(link) << " =\n";
			generateProcessTypeBody(nestedProcess, out);
		}
	}

	foreach (Id const &activity, mActivityDiagrams) {
		if (mApi.name(activity) == mApi.name(id)) {
			generateActivity(activity, out);
		}
	}

	foreach (Id const &portMap, mPortMappingDiagrams) {
		if (mApi.name(portMap) == mApi.name(id)) {
			generatePortMap(portMap, out);
		}
	}

	out() << "end;\n";
}

void HascolGenerator::generatePortMap(Id const &id, utils::OutFile &out)
{
	foreach (Id const &child, mApi.children(id)) {
		if (child.element() == "ProcessInstance" || child.element() == "FunctorInstance") {
			foreach (Id const &instanceChild, mApi.children(child)) {
				if (instanceChild.element() == "ProcessInstance"
						|| instanceChild.element() == "ProcessInstance")
				{
					out() << "process " << mApi.name(instanceChild).replace(":", "=") << " with\n";
					bool first = true;
					foreach (Id const &port, mApi.children(instanceChild)) {
						if (port.element() == "Port") {
							if (mApi.links(port).isEmpty()) {
								mErrorReporter.addWarning("Port without connections", port);
								continue;
							}

							Id const link = mApi.links(port).at(0);
							Id const mappedPort = mApi.otherEntityFromLink(link, port);

							Id const mappedPortParent = mApi.parent(mappedPort);
							QString parentName;
							if (mappedPortParent == child) {
								parentName = "";
							} else {
								parentName = mApi.name(mappedPortParent);
								parentName.remove(parentName.indexOf(":"), parentName.length());
								parentName = parentName.trimmed();
								parentName += ".";
							}

							if (mApi.outgoingLinks(port).count() == 1
								|| mappedPortParent == child)
							{
								QString comma = !first ? ", " : "";
								first = false;

								out() << comma << mApi.name(port) << " = "
										<< parentName << mApi.name(mappedPort) << "\n";
							}
						}
					}

					out() << ";\n";
				}
			}
		}
	}
}

void HascolGenerator::generateFunctor(Id const &id, OutFile &out)
{
	out() << "process " << mApi.name(id) << " (\n";

	foreach (Id const &child, mApi.children(id)) {
		if (child.element() == "FunctorFormalParameter") {
			generateFunctorFormalParameter(child, out);
		}
	}

	out() << "\t) =\n";
	generateProcessTypeBody(id, out);
}

void HascolGenerator::generateFunctorFormalParameter(Id const &id, utils::OutFile &out)
{
	if (mApi.links(id).count() == 1) {
		out() << mApi.name(id) << " : ";
		Id const parameterType = mApi.otherEntityFromLink(mApi.links(id).at(0), id);
		if (mApi.name(parameterType) != "") {
			out() << mApi.name(parameterType);
		} else {
			out() << "\n";
			generateProcessTypeBody(parameterType, out);
		}
	} else {
		out() << mApi.name(id) << "\n";
	}
}

void HascolGenerator::generateProcessOperation(Id const &id, utils::OutFile &out)
{
	out() << mApi.stringProperty(id, "direction") << " "
			<< mApi.name(id) << ";\n";
}

void HascolGenerator::generateLetBinding(Id const &id, utils::OutFile &out)
{
	out() << "let " << mApi.name(id) << ";\n";
}

void HascolGenerator::generateResource(Id const &id, bool first, OutFile &out)
{
	out() << (first ? "" : ", ") << mApi.name(id) << "\n";
}

void HascolGenerator::generateActivity(Id const &id, utils::OutFile &out)
{
	foreach (Id const &element, mApi.children(id)) {
		if (element.element() == "Group") {
			out() << "group {\n";
			generateActivity(element, out);
			out() << "}\n";
		} else if (element.element() == "HandlerStart")
			generateHandler(element, out);
	}
}

void HascolGenerator::generateHandler(Id const &id, utils::OutFile &out)
{
	out() << mApi.stringProperty(id, "trigger") << "\n";
	out() << "{\n";

	generateChain(id, out);

	out() << "}\n";
}

Id HascolGenerator::generateChain(Id const &startNode, utils::OutFile &out)
{
	Id currentId = startNode;
	while (mApi.outgoingLinks(currentId).count() > 0) {

		if (mApi.incomingLinks(currentId).count() > 1) {
			return currentId;
		}

		if (currentId.element() == "DecisionNode") {
			currentId = generateIf(currentId, out);
		} else if (mApi.outgoingLinks(currentId).count() > 1) {
			generateActivityNode(currentId, out);
			Id chainEndId;
			bool wasOutgoingLink = false;
			foreach (Id const &linkId, mApi.outgoingLinks(currentId)) {
				if (wasOutgoingLink) {
					out() << "|\n";
				}
				wasOutgoingLink = true;
				Id const chainBeginId = mApi.otherEntityFromLink(linkId, currentId);
				chainEndId = generateChain(chainBeginId, out);
			}
			currentId = chainEndId;
		}

		generateActivityNode(currentId, out);

		if (mApi.outgoingLinks(currentId).count() > 0) {
			Id const link = mApi.outgoingLinks(currentId)[0];  // Proceed to next statement in a chain
			currentId = mApi.otherEntityFromLink(link, currentId);
		}
	}

	return currentId;
}

void HascolGenerator::generateActivityNode(Id const &id, utils::OutFile &out)
{
	if (id.element() == "SendSignalAction") {
		out() << "send " << mApi.name(id) << "\n";
	} else if (id.element() == "InformSignalAction") {
		out() << "inform " << mApi.name(id) << "\n";
	} else if (id.element() == "Action") {
		out() << mApi.name(id) << "\n";
	}
}

Id HascolGenerator::generateIf(Id const &id, utils::OutFile &out)
{
	Id const thenLink = mApi.outgoingLinks(id)[0];
	out() << "if " << mApi.stringProperty(thenLink, "guard") << " then {\n";
	Id const thenChainEnd = generateChain(mApi.otherEntityFromLink(thenLink, id), out);
	Id const elseLink = mApi.outgoingLinks(id)[1];
	Id const elseNode = mApi.otherEntityFromLink(elseLink, id);
	if (elseNode != thenChainEnd) {
		out() << "} else {\n";
		generateChain(elseNode, out);
	}

	out() << "} fi\n";
	return thenChainEnd;
}
