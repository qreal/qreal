#include "nxtOSEKRobotGenerator.h"

#include <QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include <QDir>

#include "../../../../qrkernel/exception/exception.h"
#include "../../../../qrkernel/settingsManager.h"

#include "elementGeneratorFactory.h"

using namespace qReal;
using namespace robots::generator;

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(
		Id const &diagram
		, qrRepo::RepoControlInterface &api
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &destinationPath
		)
		: mDestinationPath(destinationPath)
		, mErrorReporter(errorReporter)
{
	mIsNeedToDeleteMApi = false;
	mApi = dynamic_cast<qrRepo::RepoApi *>(&api);  // TODO: remove unneeded dynamic_cast or provide strong argumentation why it is needed.
	mDiagram = diagram;
}

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(QString const &pathToRepo
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &destinationPath
		)
		: mDestinationPath(SettingsManager::value("temp", "").toString())
		, mErrorReporter(errorReporter)
{
	Q_UNUSED(destinationPath)
	mIsNeedToDeleteMApi = true;
	mApi = new qrRepo::RepoApi(pathToRepo);
}

NxtOSEKRobotGenerator::~NxtOSEKRobotGenerator()
{
	if (mApi && mIsNeedToDeleteMApi)
		delete mApi;
}

void NxtOSEKRobotGenerator::addToGeneratedStringSetVariableInit()
{
	/*foreach (SmartLine curVariable, mVariables) {
		mGeneratedStringSet[mVariablePlaceInGenStrSet].append(
				SmartLine("int " + curVariable.text() + ";", curVariable.elementId()));
	}*/
}

QString NxtOSEKRobotGenerator::generateVariableString() {
	QString res;
	foreach (SmartLine curVariable, mVariables) {
		res = res + "static float " + curVariable.text() + ";\n";
	}
	return res;
}

void NxtOSEKRobotGenerator::generate()
{
	if (mDiagram == Id()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return;
	}

	IdList toGenerate = mApi->elementsByType("InitialNode");
	toGenerate << mApi->elementsByType("InitialBlock");

	int curInitialNodeNumber = 0;
	foreach (Id curInitialNode, toGenerate) {
		if (!mApi->isGraphicalElement(curInitialNode)) {
			continue;
		}

		if (mApi->parent(curInitialNode) != mDiagram) {
			continue;
		}

		QString resultCode;
		mGeneratedStringSet.clear();
		mGeneratedStringSet.append(QList<SmartLine>()); //first list for variable initialization
		mVariablePlaceInGenStrSet = 0;

		mElementToStringListNumbers.clear();
		mVariables.clear();

		AbstractElementGenerator* gen = ElementGeneratorFactory::generator(this, curInitialNode, *mApi);
		mPreviousElement = curInitialNode;

		gen->generate(); //may throws a exception

		addToGeneratedStringSetVariableInit();

		int curTabNumber = 1;
		foreach (QList<SmartLine> lineList, mGeneratedStringSet) {
			foreach (SmartLine curLine, lineList) {
				if ( (curLine.indentLevelChange() == SmartLine::decrease)
						|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
					curTabNumber--;

				resultCode += QString(curTabNumber, '\t') + curLine.text() + "\n";

				if ( (curLine.indentLevelChange() == SmartLine::increase)
						|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
					curTabNumber++;
			}
		}
		delete gen;

		//QDir projectsDir; //TODO: use user path to projects

		QString projectName = "example" + QString::number(curInitialNodeNumber);
		QString projectDir = "nxt-tools/" + projectName;

		//Create project directory
		if (!QDir(projectDir).exists()) {
			if (!QDir("nxt-tools/").exists())
				QDir().mkdir("nxt-tools/");
			QDir().mkdir(projectDir);
		}

		/* Generate C file */
		QFile templateCFile(":/nxtOSEK/templates/template.c");
		if (!templateCFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			mErrorReporter.addError("cannot open \"" + templateCFile.fileName() + "\"");
			return;
		}

		QTextStream templateCStream(&templateCFile);
		QString resultString = templateCStream.readAll();
		templateCFile.close();

		QString resultInitCode;
		foreach (SmartLine curLine, mInitCode) {
			if ( (curLine.indentLevelChange() == SmartLine::decrease)
					|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
				curTabNumber--;

			resultInitCode += QString(curTabNumber, '\t') + curLine.text() + "\n";

			if ( (curLine.indentLevelChange() == SmartLine::increase)
					|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
				curTabNumber++;
		}

		resultString.replace("@@PROJECT_NAME@@", projectName);
		resultString.replace("@@CODE@@", resultCode);
		resultString.replace("@@VARIABLES@@", generateVariableString());
		resultString.replace("@@HOOKS@@", resultInitCode);

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
		QFile templateOILFile(":/nxtOSEK/templates/template.oil");
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
		QFile templateMakeFile(":/nxtOSEK/templates/template.makefile");
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

		curInitialNodeNumber++;
	}
	if (toGenerate.isEmpty()) {
		mErrorReporter.addError(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node or Initial Block"));
	}
}

QList<SmartLine> &NxtOSEKRobotGenerator::variables()
{
	return mVariables;
}

QList<SmartLine> &NxtOSEKRobotGenerator::initCode()
{
	return mInitCode;
}

qrRepo::RepoApi const * const NxtOSEKRobotGenerator::api() const
{
	return mApi;
}

QByteArray NxtOSEKRobotGenerator::portValue1() const
{
	return mPortValue1;
}

QByteArray NxtOSEKRobotGenerator::portValue2() const
{
	return mPortValue2;
}

QByteArray NxtOSEKRobotGenerator::portValue3() const
{
	return mPortValue3;
}

QByteArray NxtOSEKRobotGenerator::portValue4() const
{
	return mPortValue4;
}

ErrorReporterInterface &NxtOSEKRobotGenerator::errorReporter() const
{
	return mErrorReporter;
}

qReal::Id &NxtOSEKRobotGenerator::previousElement()
{
	return mPreviousElement;
}

QList<QList<SmartLine> > &NxtOSEKRobotGenerator::generatedStringSet()
{
	return mGeneratedStringSet;
}

QMap<QString, QStack<int> > &NxtOSEKRobotGenerator::elementToStringListNumbers()
{
	return mElementToStringListNumbers;
}

QStack<qReal::Id> &NxtOSEKRobotGenerator::previousLoopElements()
{
	return mPreviousLoopElements;
}
