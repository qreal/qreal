#include "hascolGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/roles.h"
#include "../../../qrrepo/repoApi.h"
#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using utils::OutFile;

HascolGenerator::HascolGenerator(qrRepo::RepoApi const &api)
	: mApi(api)
{
}

gui::ErrorReporter& HascolGenerator::generate()
{
        NewType repoId = ROOT_ID;
        TypeList rootDiagrams = mApi.children(repoId);

        foreach (NewType const diagram, rootDiagrams) {
		if (diagram.element() == "HascolPortMapping_HascolPortMappingDiagram")
			mPortMappingDiagrams.append(diagram);
		else if (diagram.element() == "HascolActivity_HascolActivityDiagram")
			mActivityDiagrams.append(diagram);
	}

        foreach (NewType const diagram, rootDiagrams) {
		qDebug() << diagram.element();
		if (diagram.element() == "HascolStructure_HascolStructureDiagram")
			generateDiagram(diagram);
	}

	return mErrorReporter;
}

void HascolGenerator::generateDiagram(NewType const &type)
{
        QString outputDirectory = mApi.stringProperty(type, "output directory");
	if (outputDirectory == "")
		outputDirectory = ".";
        OutFile out(outputDirectory + "/" + mApi.name(type) + ".md");

	out() << "using bincompl;\n";  // Сигнатура по умолчанию, определяет основные используемые типы и операции.
	out() << "\n";

        foreach (NewType const element, mApi.children(type)) {
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

void HascolGenerator::generateProcess(NewType const &type, OutFile &out)
{
        out() << "process " << mApi.name(type) << " =\n";
        generateProcessTypeBody(type, out);
}

void HascolGenerator::generateProcessTypeBody(NewType const &type, utils::OutFile &out)
{
	out() << "begin\n";

	out.incIndent();

        foreach (NewType const child, mApi.children(type))
		if (child.element() == "HascolStructure_ProcessOperation")
			generateProcessOperation(child, out);

	bool firstResource = true;
        foreach (NewType const child, mApi.children(type))
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

        foreach (NewType const child, mApi.children(type))
		if (child.element() == "HascolStructure_LetBinding")
			generateLetBinding(child, out);

        foreach (NewType const link, mApi.incomingLinks(type)) {
		if (link.element() == "HascolStructure_UsedProcessRelation") {
                        NewType const usedProcess = mApi.otherEntityFromLink(link, type);
			out() << "process " << mApi.name(link) << " = " << mApi.name(usedProcess) << ";\n";
		}
		if (link.element() == "HascolStructure_NestedProcessRelation") {
                        NewType const nestedProcess = mApi.otherEntityFromLink(link, type);
			out() << "process " << mApi.name(link) << " =\n";
			generateProcessTypeBody(nestedProcess, out);
		}
	}

        foreach (NewType const activity, mActivityDiagrams)
                if (mApi.name(activity) == mApi.name(type))
			generateActivity(activity, out);

        foreach (NewType const portMap, mPortMappingDiagrams)
                if (mApi.name(portMap) == mApi.name(type))
			generatePortMap(portMap, out);

	out.decIndent();

	out() << "end;\n";
}

void HascolGenerator::generatePortMap(NewType const &type, utils::OutFile &out)
{
        foreach (NewType const child, mApi.children(type)) {
		if (child.element() == "HascolPortMapping_ProcessInstance"
			|| child.element() == "HascolPortMapping_FunctorInstance")
		{
                        foreach (NewType const instanceChild, mApi.children(child)) {
				if (instanceChild.element() == "HascolPortMapping_ProcessInstance"
					|| instanceChild.element() == "HascolPortMapping_ProcessInstance")
				{
					out() << "process " << mApi.name(instanceChild).replace(":", "=") << " with\n";
					out.incIndent();
					bool first = true;
                                        foreach (NewType const port, mApi.children(instanceChild)) {
						if (port.element() == "HascolPortMapping_Port") {
							if (mApi.links(port).isEmpty()) {
								mErrorReporter.addWarning("Port without connections", port);
								continue;
							}

                                                        NewType const link = mApi.links(port).at(0);
                                                        NewType const mappedPort = mApi.otherEntityFromLink(link, port);

                                                        NewType const mappedPortParent = mApi.parents(mappedPort).at(0);
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

void HascolGenerator::generateFunctor(NewType const &type, OutFile &out)
{
        out() << "process " << mApi.name(type) << " (\n";

	out.incIndent();

        foreach (NewType const child, mApi.children(type))
		if (child.element() == "HascolStructure_FunctorFormalParameter")
			generateFunctorFormalParameter(child, out);

	out.decIndent();

	out() << "\t) =\n";
        generateProcessTypeBody(type, out);
}

void HascolGenerator::generateFunctorFormalParameter(NewType const &type, utils::OutFile &out)
{
        if (mApi.links(type).count() == 1) {
                out() << mApi.name(type) << " : ";
                NewType const parameterType = mApi.otherEntityFromLink(mApi.links(type).at(0), type);
		if (mApi.name(parameterType) != "") {
			out() << mApi.name(parameterType);
		} else {
			out() << "\n";
			out.incIndent();
			generateProcessTypeBody(parameterType, out);
			out.decIndent();
		}
	} else {
                out() << mApi.name(type) << "\n";
	}
}

void HascolGenerator::generateProcessOperation(NewType const &type, OutFile &out)
{
        out() << mApi.stringProperty(type, "direction") << " "
                << mApi.name(type) << ";\n";
}

void HascolGenerator::generateLetBinding(NewType const &type, utils::OutFile &out)
{
        out() << "let " << mApi.name(type) << ";\n";
}

void HascolGenerator::generateResource(NewType const &type, bool first, OutFile &out)
{
        out() << (first ? "" : ", ") << mApi.name(type) << "\n";
}

void HascolGenerator::generateActivity(NewType const &type, utils::OutFile &out)
{
        foreach (NewType const element, mApi.children(type)) {
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

void HascolGenerator::generateHandler(NewType const &type, utils::OutFile &out)
{
        out() << mApi.stringProperty(type, "trigger") << "\n";
	out() << "{\n";

	out.incIndent();
        generateChain(type, out);
	out.decIndent();

	out() << "}\n";
}

NewType HascolGenerator::generateChain(NewType const &startNode, utils::OutFile &out)
{
        NewType currentId = startNode;
	while (mApi.outgoingLinks(currentId).count() > 0) {

		if (mApi.incomingLinks(currentId).count() > 1)
			return currentId;

		if (currentId.element() == "HascolActivity_DecisionNode") {
			currentId = generateIf(currentId, out);
		} else if (mApi.outgoingLinks(currentId).count() > 1) {
			generateActivityNode(currentId, out);
                        NewType chainEndId;
			bool wasOutgoingLink = false;
                        foreach (NewType linkId, mApi.outgoingLinks(currentId)) {
				if (wasOutgoingLink)
					out() << "|\n";
				wasOutgoingLink = true;
                                NewType chainBeginId = mApi.otherEntityFromLink(linkId, currentId);
				chainEndId = generateChain(chainBeginId, out);
			}
			currentId = chainEndId;
		}

		generateActivityNode(currentId, out);

		if (mApi.outgoingLinks(currentId).count() > 0) {
                        NewType const link = mApi.outgoingLinks(currentId)[0];  // Последовательный участок цепочки.
			currentId = mApi.otherEntityFromLink(link, currentId);
		}
	}
	return currentId;
}

void HascolGenerator::generateActivityNode(NewType const &type, utils::OutFile &out)
{
        if (type.element() == "HascolActivity_SendSignalAction") {
                out() << "send " << mApi.name(type) << "\n";
        } else if (type.element() == "HascolActivity_InformSignalAction") {
                out() << "inform " << mApi.name(type) << "\n";
        } else if (type.element() == "HascolActivity_Action") {
                out() << mApi.name(type) << "\n";
	}
}

NewType HascolGenerator::generateIf(NewType const &type, utils::OutFile &out)
{
        NewType const thenLink = mApi.outgoingLinks(type)[0];
	out() << "if " << mApi.stringProperty(thenLink, "guard") << " then {\n";
	out.incIndent();
        NewType const thenChainEnd = generateChain(mApi.otherEntityFromLink(thenLink, type), out);
        NewType const elseLink = mApi.outgoingLinks(type)[1];
        NewType const elseNode = mApi.otherEntityFromLink(elseLink, type);
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
