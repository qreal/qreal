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
        Id repoId = ROOT_ID;
        IdList rootDiagrams = mApi.children(repoId);

        foreach (Id const diagram, rootDiagrams) {
                if (mApi.type(diagram).element() == "HascolPortMapping_HascolPortMappingDiagram")
			mPortMappingDiagrams.append(diagram);
                else if (mApi.type(diagram).element() == "HascolActivity_HascolActivityDiagram")
			mActivityDiagrams.append(diagram);
	}

        foreach (Id const diagram, rootDiagrams) {
                qDebug() << mApi.type(diagram).element();
                if (mApi.type(diagram).element() == "HascolStructure_HascolStructureDiagram")
			generateDiagram(diagram);
	}

	return mErrorReporter;
}

void HascolGenerator::generateDiagram(Id const &type)
{
        QString outputDirectory = mApi.stringProperty(type, "output directory");
	if (outputDirectory == "")
		outputDirectory = ".";
        OutFile out(outputDirectory + "/" + mApi.name(type) + ".md");

	out() << "using bincompl;\n";  // Сигнатура по умолчанию, определяет основные используемые типы и операции.
	out() << "\n";

        foreach (Id const element, mApi.children(type)) {
                if (mApi.type(element).element() == "HascolStructure_Process"
			&& !mApi.name(element).isEmpty())
		{
			generateProcess(element, out);
			out() << "\n";
                } else if (mApi.type(element).element() == "HascolStructure_Functor") {
			generateFunctor(element, out);
			out() << "\n";
		}
	}
}

void HascolGenerator::generateProcess(Id const &type, OutFile &out)
{
        out() << "process " << mApi.name(type) << " =\n";
        generateProcessTypeBody(type, out);
}

void HascolGenerator::generateProcessTypeBody(Id const &type, utils::OutFile &out)
{
	out() << "begin\n";

	out.incIndent();

        foreach (Id const child, mApi.children(type))
                if (mApi.type(child).element() == "HascolStructure_ProcessOperation")
			generateProcessOperation(child, out);

	bool firstResource = true;
        foreach (Id const child, mApi.children(type))
                if (mApi.type(child).element() == "HascolStructure_Resource") {
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

        foreach (Id const child, mApi.children(type))
                if (mApi.type(child).element() == "HascolStructure_LetBinding")
			generateLetBinding(child, out);

        foreach (Id const link, mApi.incomingLinks(type)) {
                if (mApi.type(link).element() == "HascolStructure_UsedProcessRelation") {
                        Id const usedProcess = mApi.otherEntityFromLink(link, type);
			out() << "process " << mApi.name(link) << " = " << mApi.name(usedProcess) << ";\n";
		}
                if (mApi.type(link).element() == "HascolStructure_NestedProcessRelation") {
                        Id const nestedProcess = mApi.otherEntityFromLink(link, type);
			out() << "process " << mApi.name(link) << " =\n";
			generateProcessTypeBody(nestedProcess, out);
		}
	}

        foreach (Id const activity, mActivityDiagrams)
                if (mApi.name(activity) == mApi.name(type))
			generateActivity(activity, out);

        foreach (Id const portMap, mPortMappingDiagrams)
                if (mApi.name(portMap) == mApi.name(type))
			generatePortMap(portMap, out);

	out.decIndent();

	out() << "end;\n";
}

void HascolGenerator::generatePortMap(Id const &type, utils::OutFile &out)
{
        foreach (Id const child, mApi.children(type)) {
                if (mApi.type(child).element() == "HascolPortMapping_ProcessInstance"
                        || mApi.type(child).element() == "HascolPortMapping_FunctorInstance")
		{
                        foreach (Id const instanceChild, mApi.children(child)) {
                                if (mApi.type(instanceChild).element() == "HascolPortMapping_ProcessInstance"
                                        || mApi.type(instanceChild).element() == "HascolPortMapping_ProcessInstance")
				{
					out() << "process " << mApi.name(instanceChild).replace(":", "=") << " with\n";
					out.incIndent();
					bool first = true;
                                        foreach (Id const port, mApi.children(instanceChild)) {
                                                if (mApi.type(port).element() == "HascolPortMapping_Port") {
							if (mApi.links(port).isEmpty()) {
								mErrorReporter.addWarning("Port without connections", port);
								continue;
							}

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

void HascolGenerator::generateFunctor(Id const &type, OutFile &out)
{
        out() << "process " << mApi.name(type) << " (\n";

	out.incIndent();

        foreach (Id const child, mApi.children(type))
                if (mApi.type(child).element() == "HascolStructure_FunctorFormalParameter")
			generateFunctorFormalParameter(child, out);

	out.decIndent();

	out() << "\t) =\n";
        generateProcessTypeBody(type, out);
}

void HascolGenerator::generateFunctorFormalParameter(Id const &type, utils::OutFile &out)
{
        if (mApi.links(type).count() == 1) {
                out() << mApi.name(type) << " : ";
                Id const parameterType = mApi.otherEntityFromLink(mApi.links(type).at(0), type);
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

void HascolGenerator::generateProcessOperation(Id const &type, OutFile &out)
{
        out() << mApi.stringProperty(type, "direction") << " "
                << mApi.name(type) << ";\n";
}

void HascolGenerator::generateLetBinding(Id const &type, utils::OutFile &out)
{
        out() << "let " << mApi.name(type) << ";\n";
}

void HascolGenerator::generateResource(Id const &type, bool first, OutFile &out)
{
        out() << (first ? "" : ", ") << mApi.name(type) << "\n";
}

void HascolGenerator::generateActivity(Id const &type, utils::OutFile &out)
{
        foreach (Id const element, mApi.children(type)) {
                if (mApi.type(element).element() == "HascolActivity_Group") {
			out() << "group {\n";
			out.incIndent();
			generateActivity(element, out);
			out.decIndent();
			out() << "}\n";
                } else if (mApi.type(element).element() == "HascolActivity_HandlerStart")
			generateHandler(element, out);
	}
}

void HascolGenerator::generateHandler(Id const &type, utils::OutFile &out)
{
        out() << mApi.stringProperty(type, "trigger") << "\n";
	out() << "{\n";

	out.incIndent();
        generateChain(type, out);
	out.decIndent();

	out() << "}\n";
}

Id HascolGenerator::generateChain(Id const &startNode, utils::OutFile &out)
{
        Id currentId = startNode;
	while (mApi.outgoingLinks(currentId).count() > 0) {

		if (mApi.incomingLinks(currentId).count() > 1)
			return currentId;

                if (mApi.type(currentId).element() == "HascolActivity_DecisionNode") {
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

void HascolGenerator::generateActivityNode(Id const &type, utils::OutFile &out)
{
        if (mApi.type(type).element() == "HascolActivity_SendSignalAction") {
                out() << "send " << mApi.name(type) << "\n";
        } else if (mApi.type(type).element() == "HascolActivity_InformSignalAction") {
                out() << "inform " << mApi.name(type) << "\n";
        } else if (mApi.type(type).element() == "HascolActivity_Action") {
                out() << mApi.name(type) << "\n";
	}
}

Id HascolGenerator::generateIf(Id const &type, utils::OutFile &out)
{
        Id const thenLink = mApi.outgoingLinks(type)[0];
	out() << "if " << mApi.stringProperty(thenLink, "guard") << " then {\n";
	out.incIndent();
        Id const thenChainEnd = generateChain(mApi.otherEntityFromLink(thenLink, type), out);
        Id const elseLink = mApi.outgoingLinks(type)[1];
        Id const elseNode = mApi.otherEntityFromLink(elseLink, type);
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
