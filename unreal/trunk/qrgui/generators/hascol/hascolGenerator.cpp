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
	OutFile out(outputDirectory + "/" + mApi.name(id));

	foreach (Id const element, mApi.children(id)) {
		if (element.element() == "HascolStructure_Process") {
			generateProcess(element, out);
			out() << "\n";
		}
	}
}

void HascolGenerator::generateProcess(Id const &id, OutFile &out)
{
	out() << "process " << mApi.name(id) << " =\n";
	out() << "begin\n";

	out.incIndent();

	foreach (Id const child, mApi.children(id))
		if (child.element() == "HascolStructure_ProcessOperation")
			generateProcessOperation(child, out);

	foreach (Id const activity, mActivityDiagrams)
		if (mApi.name(activity) == mApi.name(id))
			generateActivity(activity, out);

	out.decIndent();

	out() << "end\n";
}

void HascolGenerator::generateProcessOperation(Id const &id, OutFile &out)
{
	out() << mApi.stringProperty(id, "direction") << " "
		<< mApi.name(id) << ";\n";
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

	Id currentId = id;
	while (mApi.outgoingLinks(currentId).count() > 0) {
		Id const link = mApi.outgoingLinks(currentId)[0];  // Пока ничего паралельного не генерится.
		currentId = mApi.otherEntityFromLink(link, currentId);
		if (currentId.element() == "HascolActivity_SendSignalAction") {
			out() << mApi.name(currentId) << "\n";
		}
	}

	out.decIndent();

	out() << "}\n";
}
