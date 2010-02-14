#include "hascolGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/definitions.h"
#include "../../../qrrepo/repoApi.h"
#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using utils::OutFile;

HascolGenerator::HascolGenerator(qrRepo::RepoApi const &api)
	: mApi(api)
{
}

QString HascolGenerator::generate()
{
	mErrorText = "";

	Id repoId = ROOT_ID;
	IdList rootDiagrams = mApi.children(repoId);

	foreach (Id const diagram, rootDiagrams) {
		if (diagram.element() == "HascolPortMapping_HascolPortMappingDiagram")
			mPortMappingDiagrams.append(diagram);
		else if (diagram.element() == "HascolActivity_HascolActivityDiagram")
			mActivityDiagrams.append(diagram);
	}

	foreach (Id const diagram, rootDiagrams) {
		qDebug() << diagram.element();
		if (diagram.element() == "HascolStructure_HascolStructureDiagram")
			generateDiagram(diagram);
	}

	return mErrorText;
}

void HascolGenerator::generateDiagram(Id const &id)
{
	QString outputDirectory = mApi.stringProperty(id, "output directory");
	if (outputDirectory == "")
		outputDirectory = ".";
	OutFile out(outputDirectory + "/" + mApi.name(id) + ".md");

	out() << "using bincompl;\n";  // Сигнатура по умолчанию, определяет основные используемые типы и операции.
	out() << "\n";

	foreach (Id const element, mApi.children(id)) {
		if (element.element() == "HascolStructure_Process"
			&& !mApi.name(element).isEmpty())
		{
			generateProcess(element, out);
			out() << "\n";
		} else if (element.element() == "HascolStructure_Functor") {
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

	out.incIndent();

	foreach (Id const child, mApi.children(id))
		if (child.element() == "HascolStructure_ProcessOperation")
			generateProcessOperation(child, out);

	bool firstResource = true;
	foreach (Id const child, mApi.children(id))
		if (child.element() == "HascolStructure_Resource") {
			if (firstResource) {
				out() << "data\n";
				out.incIndent();
			}
			generateResource(child, firstResource, out);
			firstResource = false;
		}
	if (!firstResource) {
		out.decIndent();
		out() << ";\n";
	}

	foreach (Id const child, mApi.children(id))
		if (child.element() == "HascolStructure_LetBinding")
			generateLetBinding(child, out);

	foreach (Id const link, mApi.incomingLinks(id)) {
		if (link.element() == "HascolStructure_UsedProcessRelation") {
			Id const usedProcess = mApi.otherEntityFromLink(link, id);
			out() << "process " << mApi.name(link) << " = " << mApi.name(usedProcess) << ";\n";
		}
		if (link.element() == "HascolStructure_NestedProcessRelation") {
			Id const nestedProcess = mApi.otherEntityFromLink(link, id);
			out() << "process " << mApi.name(link) << " =\n";
			generateProcessTypeBody(nestedProcess, out);
		}
	}

	foreach (Id const activity, mActivityDiagrams)
		if (mApi.name(activity) == mApi.name(id))
			generateActivity(activity, out);

	foreach (Id const portMap, mPortMappingDiagrams)
		if (mApi.name(portMap) == mApi.name(id))
			generatePortMap(portMap, out);

	out.decIndent();

	out() << "end;\n";
}

void HascolGenerator::generatePortMap(Id const &id, utils::OutFile &out)
{
	foreach (Id const child, mApi.children(id)) {
		if (child.element() == "HascolPortMapping_ProcessInstance"
			|| child.element() == "HascolPortMapping_FunctorInstance")
		{
			foreach (Id const instanceChild, mApi.children(child)) {
				if (instanceChild.element() == "HascolPortMapping_ProcessInstance"
					|| instanceChild.element() == "HascolPortMapping_ProcessInstance")
				{
					out() << "process " << mApi.name(instanceChild).replace(":", "=") << " with\n";
					out.incIndent();
					bool first = true;
					foreach (Id const port, mApi.children(instanceChild)) {
						if (port.element() == "HascolPortMapping_Port") {
							Id const link = mApi.links(port).at(0);
							Id const mappedPort = mApi.otherEntityFromLink(link, port);

							Id const mappedPortParent = mApi.parents(mappedPort).at(0);
							QString parentName;
							if (mappedPortParent == child)
								parentName = "";
							else {
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

								out() << comma << mApi.name(port) << " = " << parentName << mApi.name(mappedPort) << "\n";
							}
						}
					}
					out.decIndent();
					out() << ";\n";
				}
			}
		}
	}
}

void HascolGenerator::generateFunctor(Id const &id, OutFile &out)
{
	out() << "process " << mApi.name(id) << " (\n";

	out.incIndent();

	foreach (Id const child, mApi.children(id))
		if (child.element() == "HascolStructure_FunctorFormalParameter")
			generateFunctorFormalParameter(child, out);

	out.decIndent();

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
			out.incIndent();
			generateProcessTypeBody(parameterType, out);
			out.decIndent();
		}
	} else {
		out() << mApi.name(id) << "\n";
	}
}

void HascolGenerator::generateProcessOperation(Id const &id, OutFile &out)
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
	foreach (Id const element, mApi.children(id)) {
		if (element.element() == "HascolActivity_Group") {
			out() << "group {\n";
			out.incIndent();
			generateActivity(element, out);
			out.decIndent();
			out() << "}\n";
		} else if (element.element() == "HascolActivity_HandlerStart")
			generateHandler(element, out);
	}
}

void HascolGenerator::generateHandler(Id const &id, utils::OutFile &out)
{
	out() << mApi.stringProperty(id, "trigger") << "\n";
	out() << "{\n";

	out.incIndent();
	generateChain(id, out);
	out.decIndent();

	out() << "}\n";
}

Id HascolGenerator::generateChain(Id const &startNode, utils::OutFile &out)
{
	Id currentId = startNode;
	while (mApi.outgoingLinks(currentId).count() > 0) {

		if (mApi.incomingLinks(currentId).count() > 1)
			return currentId;

		if (currentId.element() == "HascolActivity_DecisionNode") {
			currentId = generateIf(currentId, out);
		} else if (mApi.outgoingLinks(currentId).count() > 1) {
			generateActivityNode(currentId, out);
			Id chainEndId;
			bool wasOutgoingLink = false;
			foreach (Id linkId, mApi.outgoingLinks(currentId)) {
				if (wasOutgoingLink)
					out() << "|\n";
				wasOutgoingLink = true;
				Id chainBeginId = mApi.otherEntityFromLink(linkId, currentId);
				chainEndId = generateChain(chainBeginId, out);
			}
			currentId = chainEndId;
		}

		generateActivityNode(currentId, out);

		if (mApi.outgoingLinks(currentId).count() > 0) {
			Id const link = mApi.outgoingLinks(currentId)[0];  // Последовательный участок цепочки.
			currentId = mApi.otherEntityFromLink(link, currentId);
		}
	}
	return currentId;
}

void HascolGenerator::generateActivityNode(Id const &id, utils::OutFile &out)
{
	if (id.element() == "HascolActivity_SendSignalAction") {
		out() << "send " << mApi.name(id) << "\n";
	} else if (id.element() == "HascolActivity_InformSignalAction") {
		out() << "inform " << mApi.name(id) << "\n";
	} else if (id.element() == "HascolActivity_Action") {
		out() << mApi.name(id) << "\n";
	}
}

Id HascolGenerator::generateIf(Id const &id, utils::OutFile &out)
{
	Id const thenLink = mApi.outgoingLinks(id)[0];
	out() << "if " << mApi.stringProperty(thenLink, "guard") << " then {\n";
	out.incIndent();
	Id const thenChainEnd = generateChain(mApi.otherEntityFromLink(thenLink, id), out);
	Id const elseLink = mApi.outgoingLinks(id)[1];
	Id const elseNode = mApi.otherEntityFromLink(elseLink, id);
	if (elseNode != thenChainEnd) {
		out.decIndent();
		out() << "} else {\n";
		out.incIndent();
		generateChain(elseNode, out);
	}
	out.decIndent();
	out() << "} fi\n";
	return thenChainEnd;
}
