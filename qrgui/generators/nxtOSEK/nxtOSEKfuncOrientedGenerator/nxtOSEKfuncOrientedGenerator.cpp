#include "../nxtOSEKfuncOrientedGenerator.h"
#include "elementGeneratorFactory.h"

#include <QTextStream>
#include <QDir>
#include <QQueue>
#include <QDebug>

using namespace qReal;
using namespace generators;
using namespace nxtOSEKgenerator; 

NxtOSEKfuncOrientedGenerator::NxtOSEKfuncOrientedGenerator(qrRepo::RepoControlInterface &api, QString const &destinationPath)
	: NxtOSEKgenerator(api, destinationPath) {
}

NxtOSEKfuncOrientedGenerator::NxtOSEKfuncOrientedGenerator(QString const &pathToRepo, QString const &destinationPath)
	: NxtOSEKgenerator(pathToRepo, destinationPath) {
}

void NxtOSEKfuncOrientedGenerator::writeGeneratedCodeToFile(QString const &resultCode, QString const &initNodeProcedureName,
		int initialNodeNumber) {
	QString projectName = "example" + QString::number(initialNodeNumber);
	QString projectDir = "nxt-tools/" + projectName;

	//Create project directory
	if (!QDir(projectDir).exists()) {
		if (!QDir("nxt-tools/").exists())
			QDir().mkdir("nxt-tools/");
		QDir().mkdir(projectDir);
	}

	/* Generate C file */
	QFile templateCFile(":/generators/nxtOSEK/nxtOSEKfuncOrientedGenerator/templates/template.c");
	if (!templateCFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateCFile.fileName() + "\"");
		return;
	}

	QTextStream templateCStream(&templateCFile);
	QString resultString = templateCStream.readAll();
	templateCFile.close();

	resultString.replace("@@PROJECT_NAME@@", projectName);
	resultString.replace("@@PROCEDURE_CODE@@", resultCode);
	resultString.replace("@@INIT_NODE_PROCEDURE_NAME@@", initNodeProcedureName);

	QFile resultCFile(projectDir + "/" + projectName + ".c");
	if (!resultCFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultCFile.fileName() + "\"");
		return;
	}

	QTextStream outC(&resultCFile);
	outC << resultString;
	outC.flush();
	resultCFile.close();
	/**/

	/* Generate OIL file */
	QFile templateOILFile(":/generators/nxtOSEK/nxtOSEKfuncOrientedGenerator/template/template.oil");
	if (!templateOILFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateOILFile.fileName() + "\"");
		return;
	}

	QFile resultOILFile(projectDir + "/" + projectName + ".oil");
	if (!resultOILFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultOILFile.fileName() + "\"");
		return;
	}

	QTextStream outOIL(&resultOILFile);
	outOIL << templateOILFile.readAll();
	templateOILFile.close();

	outOIL.flush();
	resultOILFile.close();

	/* Generate makefile */
	QFile templateMakeFile(":/generators/nxtOSEK/nxtOSEKfuncOrientedGenerator/templates/template.makefile");
	if (!templateMakeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateMakeFile.fileName() + "\"");
		return;
	}

	QFile resultMakeFile(projectDir + "/makefile");
	if (!resultMakeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultMakeFile.fileName() + "\"");
		return;
	}

	QTextStream outMake(&resultMakeFile);
	outMake << templateMakeFile.readAll().replace("@@PROJECT_NAME@@", projectName.toUtf8());
	templateMakeFile.close();

	outMake.flush();
	resultMakeFile.close();
	/**/
}

gui::ErrorReporter &NxtOSEKfuncOrientedGenerator::generate() 
{
	prepareIdToMethodNameMap();
	QList<Id> initialNodes = mApi->elementsByType("InitialNode");

	int curInitialNodeNumber = 0;
	foreach (Id curInitialNode, initialNodes) {
		if (!mApi->isLogicalElement(curInitialNode)) //? may be necessary to use graphical  elements
			continue;

		mVariables.clear();
		mGeneratedStrings.clear();
		mAlreadyGeneratedElements.clear();

		QQueue<Id> elementsToGenerate;
		elementsToGenerate.enqueue(curInitialNode);

		while (!elementsToGenerate.empty()) {
			Id curElement = elementsToGenerate.dequeue();
			AbstractElementGenerator* gen = ElementGeneratorFactory::generator(this, curElement);
			if (gen) {
				//changes mVariables, mGeneratedStrings, mAlreadyGeneratedElements
				gen->generate(); 
				delete gen;
			}

			foreach (Id nextElement, mApi->outgoingConnectedElements(curElement)) {
				if (!mAlreadyGeneratedElements.contains(nextElement) &&
						!elementsToGenerate.contains(nextElement) && 
						(nextElement.element() != "ROOT_ID"))
					elementsToGenerate.enqueue(nextElement);
			}

			mAlreadyGeneratedElements.insert(curElement);
		}

		QList<SmartLine> variableGeneratedStrings;
		foreach (SmartLine line, mVariables) {
			variableGeneratedStrings.append(SmartLine("int " + line.text() + ";", line.elementId()));
		}

		QString resultCode = smartLineListToString(variableGeneratedStrings, 0) +
			smartLineListToString(mGeneratedStrings, 0);

		writeGeneratedCodeToFile(resultCode, mIdToMethodNameMap[curInitialNode.toString()],
				curInitialNodeNumber);

		curInitialNodeNumber++;
	}

	return mErrorReporter; 
}

void NxtOSEKfuncOrientedGenerator::prepareIdToMethodNameMap()
{
	mIdToMethodNameMap.clear();

	IdList allNodes = mApi->logicalElements();
	foreach (Id curNode, allNodes) {
		AbstractElementGenerator* gen = ElementGeneratorFactory::generator(this, curNode);
		if (gen == 0)
			continue;

		mIdToMethodNameMap.insert(curNode.toString(), gen->nextMethodName());
		delete gen;
	}
}

QString NxtOSEKfuncOrientedGenerator::smartLineListToString(QList<SmartLine> list, int startIndentSize) {
	QString result;
	int curIndentSize = startIndentSize;
	foreach (SmartLine curLine, list) {
		if ( (curLine.indentLevelChange() == SmartLine::decrease)
				|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
			curIndentSize--;

		result += QString(curIndentSize, '\t') + curLine.text() + "\n";

		if ( (curLine.indentLevelChange() == SmartLine::increase)
				|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
			curIndentSize++;
	}

	return result;
}
