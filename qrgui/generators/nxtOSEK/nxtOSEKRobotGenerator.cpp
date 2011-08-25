#include <QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include <QDir>
#include "nxtOSEKRobotGenerator.h"

#include <QDebug>

using namespace qReal;
using namespace generators;

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(qrRepo::RepoApi *api, QString const &destinationPath)
	:  mDestinationPath(destinationPath)
{
		mIsNeedToDeleteMApi = false;
		mApi = api;
}

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(QString const &pathToRepo, QString const &destinationPath)
	:mDestinationPath(destinationPath)
{
		mIsNeedToDeleteMApi = true;
		mApi = new qrRepo::RepoApi(pathToRepo);
}

NxtOSEKRobotGenerator::~NxtOSEKRobotGenerator()
{
	if (mApi && mIsNeedToDeleteMApi)
		delete mApi;
}

gui::ErrorReporter &NxtOSEKRobotGenerator::generate()
{
	IdList initialNodes = mApi->elementsByType("InitialNode");

	int curInitialNodeNumber = 0;
	foreach (Id curInitialNode, initialNodes) {
		if (mApi->isGraphicalElement(curInitialNode)) {
			QString resultCode;

			mGeneratedStringSet.clear();
			mElementToStringListNumbers.clear();

			AbstractElementGenerator* gen = ElementGeneratorFactory::generator(this, curInitialNode);
			mPreviousElement = curInitialNode;
			gen->generate();
			QList< QPair<QString, qReal::Id> > stringPairList;
			foreach (stringPairList, mGeneratedStringSet) {
				QPair<QString, qReal::Id> stringPair;
				foreach (stringPair, stringPairList) {
					resultCode += stringPair.first + "\n";
				}
			} //TODO
			delete gen;

			//QDir projectsDir; //TODO: use user path to projects

			QString projectName = "example" + QString::number(curInitialNodeNumber);

			//Create project directory
			if (!QDir(projectName).exists())
				QDir().mkdir(projectName);

			/* Generate C file */
			QFile templateCFile(":/generators/nxtOSEK/templates/template.c");
			if (!templateCFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				mErrorReporter.addError("cannot open \"" + templateCFile.fileName() + "\"");
				return mErrorReporter;
			}

			QTextStream templateCStream(&templateCFile);
			QString resultString = templateCStream.readAll();
			templateCFile.close();

			resultString.replace("@@PROJECT_NAME@@", projectName);
			resultString.replace("@@CODE@@", resultCode);

			QFile resultCFile(projectName + "/" + projectName + ".c");
			if (!resultCFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				mErrorReporter.addError("cannot open \"" + resultCFile.fileName() + "\"");
				return mErrorReporter;
			}

			QTextStream outC(&resultCFile);
			outC << resultString;
			outC.flush();
			resultCFile.close();
			/**/

			/* Generate OIL file */
			QFile templateOILFile(":/generators/nxtOSEK/templates/template.oil");
			if (!templateOILFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				mErrorReporter.addError("cannot open \"" + templateOILFile.fileName() + "\"");
				return mErrorReporter;
			}

			QFile resultOILFile(projectName + "/" + projectName + ".oil");
			if (!resultOILFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				mErrorReporter.addError("cannot open \"" + resultOILFile.fileName() + "\"");
				return mErrorReporter;
			}

			QTextStream outOIL(&resultOILFile);
			outOIL << templateOILFile.readAll();
			templateOILFile.close();

			outOIL.flush();
			resultOILFile.close();

			/* Generate makefile */
			QFile templateMakeFile(":/generators/nxtOSEK/templates/template.makefile");
			if (!templateMakeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				mErrorReporter.addError("cannot open \"" + templateMakeFile.fileName() + "\"");
				return mErrorReporter;
			}

			QFile resultMakeFile(projectName + "/makefile");
			if (!resultMakeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				mErrorReporter.addError("cannot open \"" + resultMakeFile.fileName() + "\"");
				return mErrorReporter;
			}

			QTextStream outMake(&resultMakeFile);
			outMake << templateMakeFile.readAll().replace("@@PROJECT_NAME@@", projectName.toUtf8());
			templateMakeFile.close();

			outMake.flush();
			resultMakeFile.close();
			/**/

			curInitialNodeNumber++;
		}
	}
	return mErrorReporter;
}

NxtOSEKRobotGenerator::AbstractElementGenerator::AbstractElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): mNxtGen(emboxGen), mElementId(elementId)
{
}

void NxtOSEKRobotGenerator::AbstractElementGenerator::createListsForIncomingConnections()
{
	//connects string lists in mGeneratedStringSet with mElementId in mElementToStringListNumbers
	for (int i = 1; i < mNxtGen->mApi->incomingConnectedElements(mElementId).size(); i++) {
		mNxtGen->mGeneratedStringSet << QList< QPair<QString, qReal::Id> >();
		mNxtGen->mElementToStringListNumbers[mElementId.toString()] << mNxtGen->mGeneratedStringSet.size() - 1;
	}
}

NxtOSEKRobotGenerator::SimpleElementGenerator::SimpleElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

NxtOSEKRobotGenerator::LoopElementGenerator::LoopElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

NxtOSEKRobotGenerator::IfElementGenerator::IfElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

QList<QString> NxtOSEKRobotGenerator::SimpleElementGenerator::portsToEngineNames(QString const &portsProperty)
{
	QList<QString> result;

	//port {A, B, C} -> NXT_PORT_{A, B, C}
	if (portsProperty.contains("A"))
		result.append("NXT_PORT_A");
	if (portsProperty.contains("B"))
		result.append("NXT_PORT_B");
	if (portsProperty.contains("C"))
		result.append("NXT_PORT_C");

	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::SimpleElementGenerator::simpleCode()
{
	QList< QPair<QString, qReal::Id> > result;

	qReal::Id logicElementId = mNxtGen->mApi->logicalId(mElementId); //TODO

	//TODO: to make "break mode" to do smth
	if (mElementId.element() == "EnginesForward") {
		QStringList cmds = mNxtGen->mApi->stringProperty(logicElementId, "Power").split(";", QString::SkipEmptyParts);
		for (int i = 0; i < cmds.size() - 1; ++i)
			result.append(QPair<QString, qReal::Id>(cmds.at(i) + ";", mElementId));
		foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(logicElementId, "Ports"))) {
			result.append(QPair<QString, qReal::Id>(
						"nxt_motor_set_speed(" + enginePort + ", " + cmds.last() + ", 0);",
						mElementId));
		}

	} else if (mElementId.element() == "EnginesBackward") {
		foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(logicElementId, "Ports"))) {
			result.append(QPair<QString, qReal::Id>(
						"nxt_motor_set_speed(" + enginePort + ", "
							+ "-" + mNxtGen->mApi->stringProperty(logicElementId, "Power") + ", 0);"
						, mElementId));
		}

	} else if (mElementId.element() == "EnginesStop") {
		foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(logicElementId, "Ports"))) {
			result.append(QPair<QString, qReal::Id>(
						"nxt_motor_set_speed(" + enginePort + ", 0, 0);"
						, mElementId));
		}

	} else if (mElementId.element() == "Timer") {
		result.append(QPair<QString, qReal::Id>(
					"systick_wait_ms(" + mNxtGen->mApi->stringProperty(logicElementId, "Delay") + ");",
					mElementId));

	} else if (mElementId.element() == "Beep") {
		result.append(QPair<QString, qReal::Id>(
					"ecrobot_sound_tone(1000, 100, 50)", //TODO: change sound to smth
					mElementId));

	} else if (mElementId.element() == "PlayTone") {
		result.append(QPair<QString, qReal::Id>(
					"ecrobot_sound_tone(" + mNxtGen->mApi->stringProperty(logicElementId, "Frequency") + ", "
						+ mNxtGen->mApi->stringProperty(logicElementId, "Duration") + ", 50)", //50 - volume of a sound
					mElementId));

	} else if (mElementId.element() == "Function") {
		QByteArray byteFuncCode = mNxtGen->mApi->stringProperty(logicElementId, "Body").toUtf8();
		byteFuncCode.replace("Сенсор1", "ecrobot_get_sonar_sensor(NXT_PORT_S1)");
		byteFuncCode.replace("Сенсор2", "ecrobot_get_sonar_sensor(NXT_PORT_S2)");
		byteFuncCode.replace("Сенсор3", "ecrobot_get_sonar_sensor(NXT_PORT_S3)");
		byteFuncCode.replace("Сенсор4", "ecrobot_get_sonar_sensor(NXT_PORT_S4)");

		QString funcCode = QString::fromUtf8(byteFuncCode) + ";";

		result.append(QPair<QString, qReal::Id>(funcCode, mElementId));

	} else if (mElementId.element() == "FinalNode") {
		result.append(QPair<QString, qReal::Id>(
					"return;",
					mElementId));

	} else if (mElementId.element() == "InitialBlock") {
		for (int i = 1; i <= 4; i++) {
			//4 - number of ports on nxt robot
			QString curPort = "port_" + QString::number(i);
			QByteArray portValue = mNxtGen->mApi->stringProperty(logicElementId, curPort).toUtf8();

			if (portValue == "Ультразвуковой сенсор") {
				result.append(QPair<QString, qReal::Id>(
						"ecrobot_init_sonar_sensor(NXT_PORT_S" + QString::number(i) + ")",
						mElementId));

			//in nxtOSEK there are no instructions to initiate touch sensors
			//} else if (portValue == "Сенсор нажатия (булево значение)") {
			//} else if (portValue == "Сенсор нажатия (сырое значение)") {

			} else if (portValue == "Сенсор цвета (полные цвета)") {
				result.append(QPair<QString, qReal::Id>(
						"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_WHITE)",
						mElementId));

			} else if (portValue == "Сенсор цвета (красный)") {
				result.append(QPair<QString, qReal::Id>(
						"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_RED)",
						mElementId));

			} else if (portValue == "Сенсор цвета (зеленый)") {
				result.append(QPair<QString, qReal::Id>(
						"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_GREEN)",
						mElementId));

			} else if (portValue == "Сенсор цвета (синий)") {
				result.append(QPair<QString, qReal::Id>(
						"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_LIGHTSENSOR_BLUE)",
						mElementId));

			} else if (portValue == "Сенсор цвета (пассивный)") {
				result.append(QPair<QString, qReal::Id>(
						"ecrobot_init_nxtcolorsensor(NXT_PORT_S" + QString::number(i) + ", NXT_COLORSENSOR)",
						mElementId));

			}
		}

	} else if (mElementId.element() == "WaitForColor") {
		int port = mNxtGen->mApi->stringProperty(logicElementId, "Port").toInt();
		QByteArray colorStr = mNxtGen->mApi->stringProperty(logicElementId, "Color").toUtf8();

		QString colorNxtType;

		if (colorStr == "Красный") {
			colorNxtType = "NXT_COLOR_RED";

		} else if (colorStr == "Зелёный") {
			colorNxtType = "NXT_COLOR_GREEN";

		} else if (colorStr == "Синий") {
			colorNxtType = "NXT_COLOR_BLUE";

		} else if (colorStr == "Чёрный") {
			colorNxtType = "NXT_COLOR_BLACK";

		} else if (colorStr == "Жёлтый") {
			colorNxtType = "NXT_COLOR_YELLOW";

		} else if (colorStr == "Белый") {
			colorNxtType = "NXT_COLOR_WHITE";
		}

		if (!colorNxtType.isEmpty())
			result.append(QPair<QString, qReal::Id>(
					"while (ecrobot_get_nxtcolorsensor_id(NXT_PORT_S" + QString::number(port)
						+ ") != " + colorNxtType + ") { }",
					mElementId));

	} else if (mElementId.element() == "WaitForColorIntensity") {
		int port = mNxtGen->mApi->stringProperty(logicElementId, "Port").toInt();
		QString intensity = mNxtGen->mApi->stringProperty(logicElementId,  "Intensity");
		QString inequalitySign = mNxtGen->mApi->stringProperty(logicElementId, "Sign");

		QString condition = inequalitySign + " " + intensity;

		result.append(QPair<QString, qReal::Id>(
				"while (!(ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + QString::number(port)
					+ ") " + condition + ")) \n{\n}",
				mElementId));

	} else if (mElementId.element() == "WaitForTouchSensor") {
		int port = mNxtGen->mApi->stringProperty(logicElementId, "Port").toInt();

		result.append(QPair<QString, qReal::Id>(
				"while (!ecrobot_get_touch_sensor(NXT_PORT_S" + QString::number(port) + ")) \n{\n}",
				mElementId));

	} else if (mElementId.element() == "WaitForSonarDistance") {
		int port = mNxtGen->mApi->stringProperty(logicElementId, "Port").toInt();
		QString distance = mNxtGen->mApi->stringProperty(logicElementId, "Distance");
		QString inequalitySign = mNxtGen->mApi->stringProperty(logicElementId, "Sign");

		if (inequalitySign == "&lt;")
			inequalitySign = "<";
		else if (inequalitySign == "=")
			inequalitySign = "==";

		QString condition = inequalitySign + " " + distance;

		result.append(QPair<QString, qReal::Id>(
				"while (!(ecrobot_get_sonar_sensor(NXT_PORT_S" + QString::number(port) + ") " + condition + ")) \n{\n}",
				mElementId));
	}

	//for InitialNode returns empty list

	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::SimpleElementGenerator::loopPrefixCode()
{
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("while (true) {", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::SimpleElementGenerator::loopPostfixCode()
{
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::LoopElementGenerator::loopPrefixCode()
{
	QList< QPair<QString, qReal::Id> > result;

	qReal::Id logicElementId = mNxtGen->mApi->logicalId(mElementId); //TODO
	result << QPair<QString, qReal::Id>("for (int __iter__ = ; __iter__ < " +
			mNxtGen->mApi->property(logicElementId, "Iterations").toString()
				+ "; __iter__++) {", mElementId); //TODO
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::LoopElementGenerator::loopPostfixCode()
{
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::IfElementGenerator::loopPrefixCode()
{
	QList< QPair<QString, qReal::Id> > result;

	qReal::Id logicElementId = mNxtGen->mApi->logicalId(mElementId); //TODO
	result << QPair<QString, qReal::Id>("while (" +
			mNxtGen->mApi->property(logicElementId, "Condition").toString()
				+ ") {", mElementId); //TODO
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::IfElementGenerator::loopPostfixCode()
{
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

bool NxtOSEKRobotGenerator::SimpleElementGenerator::preGenerationCheck()
{
	IdList outgoingConnectedElements = mNxtGen->mApi->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		//case of error in diagram
		qDebug() << "Error! There are more than 1 outgoing connected elements with simple robot" <<
			"element!";
		return false;
	}

	return true;
}

bool NxtOSEKRobotGenerator::LoopElementGenerator::preGenerationCheck()
{
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);

	if ((outgoingLinks.size() != 2) ||
		( (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString() == "Итерация")
		  &&
		  (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(1)), "Guard").toString() == "Итерация") )
	)
		return false;

	return true;
}

bool NxtOSEKRobotGenerator::IfElementGenerator::preGenerationCheck()
{
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);

	//TODO: append checking arrows
	return (outgoingLinks.size() == 2);
}

bool NxtOSEKRobotGenerator::SimpleElementGenerator::nextElementsGeneration()
{
	IdList outgoingConnectedElements = mNxtGen->mApi->outgoingConnectedElements(mElementId);
	mNxtGen->mGeneratedStringSet << simpleCode();

	if (outgoingConnectedElements.size() == 1) {
		AbstractElementGenerator* gen = ElementGeneratorFactory::generator(mNxtGen, outgoingConnectedElements.at(0));
		mNxtGen->mPreviousElement = mElementId;
		gen->generate();
		delete gen;
		return true;
	} else if ((mElementId.element() == "FinalNode") && (outgoingConnectedElements.size() == 0)) {
		return true;
	} else {
		//case of error end of diagram
		qDebug() << "Error! There is no outgoing connected elements with no final node!";
		return false;
	}

	return true;
}

bool NxtOSEKRobotGenerator::LoopElementGenerator::nextElementsGeneration()
{
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);
	// outgoingLinks.size() must be 2!

	int elementConnectedByIterationEdgeNumber = -1;
	int afterLoopElementNumber = -1;

	//Грязный хак! Почему-то неправильно читается русский
	if (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString().toUtf8() == "итерация") {
		elementConnectedByIterationEdgeNumber = 0;
		afterLoopElementNumber = 1;
	} else {
		elementConnectedByIterationEdgeNumber = 1;
		afterLoopElementNumber = 0;
	}

	//generate loop
	AbstractElementGenerator* loopGen = ElementGeneratorFactory::generator(mNxtGen,
			mNxtGen->mApi->to(outgoingLinks.at(elementConnectedByIterationEdgeNumber)));

	mNxtGen->mPreviousElement = mElementId;
	mNxtGen->mPreviousLoopElements.push(mElementId);
	if (!loopGen->generate())
		return false;
	delete loopGen;

	//generate next blocks
	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mNxtGen,
			mNxtGen->mApi->to(outgoingLinks.at(afterLoopElementNumber)));

	mNxtGen->mPreviousElement = mElementId;
	mNxtGen->mPreviousLoopElements.push(mElementId);
	if (!nextBlocksGen->generate())
		return false;
	delete nextBlocksGen;

	return true;
}

bool NxtOSEKRobotGenerator::IfElementGenerator::generateBranch(int branchNumber)
{
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);

	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mNxtGen,
			mNxtGen->mApi->to(outgoingLinks.at(branchNumber)));

	mNxtGen->mPreviousElement = mElementId;

	if (!nextBlocksGen->generate())
		return false;
	delete nextBlocksGen;

	return true;
}

bool NxtOSEKRobotGenerator::IfElementGenerator::nextElementsGeneration()
{
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);
	Q_ASSERT(outgoingLinks.size() == 2);

	//we search for arrow with condition
	int conditionArrowNum =
			mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString().isEmpty()
			? 1 : 0;

	qReal::Id logicElementId = mNxtGen->mApi->logicalId(mElementId); //TODO

	//TODO: save number of new created list
	QList< QPair<QString, qReal::Id> > ifBlockPrefix;
	QString condition = mNxtGen->mApi->property(logicElementId, "Condition").toString();

	//Грязное место!
	if (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(conditionArrowNum)), "Guard") == "меньше 0") {
		condition += " < 0";
	} else if (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(conditionArrowNum)), "Guard") == "больше 0") {
		condition += " > 0";
	} else {
		condition += " == 0";
	}

	ifBlockPrefix << QPair<QString, qReal::Id>("if (" + condition + ") {", mElementId);
	mNxtGen->mGeneratedStringSet << ifBlockPrefix;

	//generate true/false blocks
	generateBranch(conditionArrowNum);
	QList< QPair<QString, qReal::Id> > elseBlock;
	elseBlock << QPair<QString, qReal::Id>("} else {", mElementId);
	mNxtGen->mGeneratedStringSet << elseBlock;
	generateBranch(1 - conditionArrowNum);

	QList< QPair<QString, qReal::Id> > ifBlockPostfix;
	ifBlockPostfix << QPair<QString, qReal::Id>("}", mElementId);
	mNxtGen->mGeneratedStringSet << ifBlockPostfix;

	return true;
}

bool NxtOSEKRobotGenerator::AbstractElementGenerator::generate()
{
	if (!preGenerationCheck())
		return false;

	if (mNxtGen->mElementToStringListNumbers.contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection

		qReal::Id loopElement = mElementId;
		if (!mNxtGen->mPreviousLoopElements.empty())
			loopElement = mNxtGen->mPreviousLoopElements.pop();

		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mNxtGen, loopElement);

		mNxtGen->mGeneratedStringSet[mNxtGen->mElementToStringListNumbers[loopElement.toString()].pop()]
			+= loopElementGen->loopPrefixCode();

		mNxtGen->mGeneratedStringSet << loopElementGen->loopPostfixCode();

		return true;
	}

	//in case element has more than 1 incoming connection
	//means that element has incoming connections from another elements, we haven`t already observed
	createListsForIncomingConnections();

	return nextElementsGeneration();
}
