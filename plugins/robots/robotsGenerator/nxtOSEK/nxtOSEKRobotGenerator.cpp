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
// mmm!!!
void NxtOSEKRobotGenerator::addToGeneratedStringSetVariableInit()
{
	/*foreach (SmartLine curVariable, mVariables) {
		mGeneratedStringSet[mVariablePlaceInGenStrSet].append(
				SmartLine("int " + curVariable.text() + ";", curVariable.elementId()));
	}*/
}

QString NxtOSEKRobotGenerator::generateVariableString()
{
	QString res;
	foreach (SmartLine curVariable, mVariables) {
		if (!curVariable.text().contains(" ")) {
			res = res + "static int " + curVariable.text() + ";\n";
		}
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

	QString resultInitCode;
	QString resultTerminateCode;

	//QDir projectsDir; //TODO: use user path to projects
	QString projectName = "example" + QString::number(curInitialNodeNumber); // прилично назвать
	QString projectDir = "nxt-tools/" + projectName;

	//Create project directory
	if (!QDir(projectDir).exists()) {
		if (!QDir("nxt-tools/").exists())
			QDir().mkdir("nxt-tools/");
		QDir().mkdir(projectDir);
	}

	// Template C file
	QFile templateCFile(":/nxtOSEK/templates/template.c");
	if (!templateCFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateCFile.fileName() + "\"");
		return;
	}

	QTextStream templateCStream(&templateCFile);
	QString resultString = templateCStream.readAll();
	templateCFile.close();

	// Generate OIL file
	QFile resultOILFile(projectDir + "/" + projectName + ".oil");
	if (!resultOILFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultOILFile.fileName() + "\"");
		return;
	}

	// Template OIL file
	QFile templateOILFile(":/nxtOSEK/templates/template.oil");
	if (!templateOILFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateOILFile.fileName() + "\"");
		return;
	}

	QString resultOIL = templateOILFile.readAll();
	templateOILFile.close();

	// Task template file
	QFile taskTemplateFile(":/nxtOSEK/templates/taskTemplate.oil");
	if (!taskTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + taskTemplateFile.fileName() + "\"");
		return;
	}

	QString resultTaskTemplate = taskTemplateFile.readAll();
	taskTemplateFile.close();

	foreach (Id curInitialNode, toGenerate) {

		QString taskTemplate = resultTaskTemplate;
		if (!mApi->isGraphicalElement(curInitialNode)) {
			continue;
		}

		if (mApi->parent(curInitialNode) != mDiagram) {
			continue;
		}

		mGeneratedStringSet.clear();
		mGeneratedStringSet.append(QList<SmartLine>()); //first list for variable initialization
		mVariablePlaceInGenStrSet = 0;

		mElementToStringListNumbers.clear();
		mVariables.clear();

		AbstractElementGenerator* gen = ElementGeneratorFactory::generator(this, curInitialNode, *mApi);
		mPreviousElement = curInitialNode;

		gen->generate(); //may throws a exception
		delete gen;
		addToGeneratedStringSetVariableInit();

		// Result code in .c file
		QString resultCode;
		int curTabNumber = 1;
		foreach (QList<SmartLine> lineList, mGeneratedStringSet) {
			foreach (SmartLine curLine, lineList) {
				if ( (curLine.indentLevelChange() == SmartLine::decrease)
						|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
				{
					curTabNumber--;
				}

				resultCode += QString(curTabNumber, '\t') + curLine.text() + "\n";

				if ( (curLine.indentLevelChange() == SmartLine::increase)
						|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
				{
					curTabNumber++;
				}
			}
		}

		// Code init block in .c file
		foreach (SmartLine curLine, mInitCode) {
			if ( (curLine.indentLevelChange() == SmartLine::decrease)
					|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
			{
				curTabNumber--;
			}

			resultInitCode += QString(curTabNumber, '\t') + curLine.text() + "\n";

			if ( (curLine.indentLevelChange() == SmartLine::increase)
					|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
			{
				curTabNumber++;
			}
		}

		// Code init block in .c file
		foreach (SmartLine curLine, mTerminateCode) {
			if ( (curLine.indentLevelChange() == SmartLine::decrease)
					|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
			{
				curTabNumber--;
			}

			resultTerminateCode += QString(curTabNumber, '\t') + curLine.text() + "\n";

			if ( (curLine.indentLevelChange() == SmartLine::increase)
					|| (curLine.indentLevelChange() == SmartLine::increaseDecrease) )
			{
				curTabNumber++;
			}
		}

		resultCode = "TASK(OSEK_Task_Number_" + QString::number(curInitialNodeNumber) +")\n{\n" + resultCode + "}";
		resultString.replace("@@CODE@@", resultCode +"\n" + "@@CODE@@");

		taskTemplate.replace("@@NUMBER@@", QString::number(curInitialNodeNumber));
		resultOIL.replace("@@TASK@@", taskTemplate + "\n" + "@@TASK@@");

		resultString.replace("@@VARIABLES@@", generateVariableString() + "\n" + "@@VARIABLES@@");
		resultString.replace("@@INITHOOKS@@", resultInitCode);
		resultString.replace("@@TERMINATEHOOKS@@", resultTerminateCode);

		curInitialNodeNumber++;
	}

	resultOIL.replace("@@TASK@@", "");
	resultString.replace("@@VARIABLES@@", "");
	resultString.replace("@@CODE@@", "");
	resultString.replace("@@PROJECT_NAME@@", projectName);



	QFile resultCFile(projectDir + "/" + projectName + ".c");
	if (!resultCFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultCFile.fileName() + "\"");
		return;
	}

	//Output in the .c and .oil file
	QTextStream outC(&resultCFile);
	outC << resultString;
	outC.flush();
	resultCFile.close();

	QTextStream outOIL(&resultOILFile);
	outOIL << resultOIL;
	outOIL.flush();
	resultOILFile.close();

	/* Generate files for balancer */ // rfrf
	QFile::copy(":/nxtOSEK/templates/balancer/balancer_param.c", projectDir + "/" + "balancer_param.c");
	QFile::copy(":/nxtOSEK/templates/balancer/balancer.h", projectDir + "/" +"balancer.h");
	QFile::copy(":/nxtOSEK/templates/balancer/balancer_types.h", projectDir + "/" + "balancer_types.h");
	QFile::copy(":/nxtOSEK/templates/balancer/libnxtway_gs_balancer.a", projectDir + "/" + "libnxtway_gs_balancer.a");
	QFile::copy(":/nxtOSEK/templates/balancer/rt_SATURATE.h", projectDir + "/" + "rt_SATURATE.h");
	QFile::copy(":/nxtOSEK/templates/balancer/rtwtypes.h", projectDir + "/" + "rtwtypes.h");
	/**/

	// Generate makefile
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

	if (toGenerate.isEmpty()) {
		mErrorReporter.addError(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node or Initial Block"));
	}

	// End generate makefile
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
