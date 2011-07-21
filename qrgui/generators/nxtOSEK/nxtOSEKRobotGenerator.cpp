#include <QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include "nxtOSEKRobotGenerator.h"

#include <QDebug>

using namespace qReal;
using namespace generators;

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(qrRepo::RepoApi *api, QString const &destinationPath)
	:  mDestinationPath(destinationPath) {
		mIsNeedToDeleteMApi = false;
		mApi = api;
}

NxtOSEKRobotGenerator::NxtOSEKRobotGenerator(QString const &pathToRepo, QString const &destinationPath)
	:mDestinationPath(destinationPath) {
		mIsNeedToDeleteMApi = true;
		mApi = new qrRepo::RepoApi(pathToRepo);
}

NxtOSEKRobotGenerator::~NxtOSEKRobotGenerator() {
	if (mApi && mIsNeedToDeleteMApi)
		delete mApi;
}

void NxtOSEKRobotGenerator::generate() {
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

			QFile templateFile("template.c");
			if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				qDebug() << "cannot open \"" << templateFile.fileName() << "\"";
				return;
			}

			QTextStream templateStream(&templateFile);
			QString resultString = templateStream.readAll();
			resultString.replace("@@TEST_NAME@@", "prog" + QString::number(curInitialNodeNumber));
			resultString.replace("@@CODE@@", resultCode);

			QFile resultFile("prog" + QString::number(curInitialNodeNumber) + ".c");
			if (!resultFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				qDebug() << "cannot open \"" << resultFile.fileName() << "\"";
				return;
			}

			QTextStream out(&resultFile);
			out << resultString;
			out.flush();
			
			resultFile.close();

			curInitialNodeNumber++;
		}
	}
}

/*
QString NxtOSEKRobotGenerator::generateByElement(qReal::Id element, qReal::Id previousElement) {
	//element.element() <- type of "Id element"
	if (element.element() == "InitialNode") {
		return "1";

	} else if (element.element() == "FinalNode") {
		return "";

	} else if (element.element() == "ControlFlow") {
		return "3";

	//} else if (element.element() == "EngineCommand") {
	//	return "4";

	} else if (element.element() == "EnginesForward") {
		return "motor_set_power(MOTOR0, " + QString::number(360 * mApi.property(element, "Power")) + ");";

	} else if (element.element() == "EnginesBackward") {
		return "motor_set_power(MOTOR0, " + QString::number(-360 * mApi.property(element, "Power").toInt()) + ");";

	} else if (element.element() == "EnginesStop") {
		return "motor_set_power(MOTOR0, 0);";

	} else if (element.element() == "Timer") {
		return "usleep(" + QString::number(mApi.property(element, "Delay").toInt()) + ")";

	} else if (element.element() == "WaitForTouchSensor") {
		return "9";

	//} else if (element.element() == "Beep") {
	//	return "10";

	} else if (element.element() == "WaitForCompletion") {
		return "11";

	} else if (element.element() == "Loop") {
		return "12";

	} else if (element.element() == "Iterations") {
		return "13";

	} else if (element.element() == "Fork") {
		return "14";

	//} else if (element.element() == "PlayTone") {
	//	return "15";

	} else if (element.element() == "WaitForSonarDistance") {
		return "16";

	} else if (element.element() == "SensorBlock") {
		return "17";

	//} else if (element.element() == "AbstractNode") {
	//	return "18";

	} else if (element.element() == "EngineMovementCommand") {
		return "19";

	} else if (element.element() == "WaitForColorIntensity") {
		return "20";

	} else if (element.element() == "WaitForColor") {
		return "21";

	} else if (element.element() == "Function") {
		return "22";

	} else if (element.element() == "IfBlock") {
		return "23";

	} else if (element.element() == "InitialBlock") {
		return "24";
	}
}
*/

NxtOSEKRobotGenerator::AbstractElementGenerator::AbstractElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): mNxtGen(emboxGen), mElementId(elementId) {	
}

void NxtOSEKRobotGenerator::AbstractElementGenerator::createListsForIncomingConnections() {
	//connects string lists in mGeneratedStringSet with mElementId in mElementToStringListNumbers
	for (int i = 1; i < mNxtGen->mApi->incomingConnectedElements(mElementId).size(); i++) {
		mNxtGen->mGeneratedStringSet << QList< QPair<QString, qReal::Id> >();
		mNxtGen->mElementToStringListNumbers[mElementId.toString()] << mNxtGen->mGeneratedStringSet.size() - 1;
	}
}

NxtOSEKRobotGenerator::SimpleElementGenerator::SimpleElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): 
	AbstractElementGenerator(emboxGen, elementId) {
}

NxtOSEKRobotGenerator::LoopElementGenerator::LoopElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): 
	AbstractElementGenerator(emboxGen, elementId) {
}

NxtOSEKRobotGenerator::IfElementGenerator::IfElementGenerator(NxtOSEKRobotGenerator *emboxGen,
		qReal::Id elementId): 
	AbstractElementGenerator(emboxGen, elementId) {
}

QList<QString> NxtOSEKRobotGenerator::SimpleElementGenerator::portsToEngineNames(QString const &portsProperty) {
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

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::SimpleElementGenerator::simpleCode() {
	QList< QPair<QString, qReal::Id> > result;

	qReal::Id logicElementId = mNxtGen->mApi->logicalId(mElementId); //TODO

	//TODO: to make "break mode" to do smth
	if (mElementId.element() == "EnginesForward") {
		foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(logicElementId, "Ports"))) {
			result.append(QPair<QString, qReal::Id>(
						"nxt_motor_set_speed(" + enginePort + ", " + mNxtGen->mApi->stringProperty(logicElementId, "Power") + ", 0);",
						mElementId));
		}

	} else if (mElementId.element() == "EnginesBackward") {
		foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(logicElementId, "Ports"))) {
			result.append(QPair<QString, qReal::Id>(
						"nxt_motor_set_speed(" + enginePort + ", " + mNxtGen->mApi->stringProperty(logicElementId, "Power") + ", 0);",
						mElementId));
		}

	} else if (mElementId.element() == "EnginesStop") {
		foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(logicElementId, "Ports"))) {
			result.append(QPair<QString, qReal::Id>(
						"nxt_motor_set_speed(" + enginePort + ", 0, 0);",
						mElementId));
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
		
		QString funcCode = QString::fromUtf8(byteFuncCode);

		result.append(QPair<QString, qReal::Id>(
					//funcCode.right(funcCode.length() - 4), //TODO: just delete "var "
					funcCode, 
					mElementId));

	} else if (mElementId.element() == "FinalNode") {
		result.append(QPair<QString, qReal::Id>(
					"return;",
					mElementId));
	} else if (mElementId.element() == "InitialBlock") {
		for (int i = 0; i < 4; i++) {
			//4 - number of ports on nxt robot
			QString curPort = "port_" + QString::number(i);
			QByteArray portValue = mNxtGen->mApi->stringProperty(logicElementId, curPort).toUtf8();

			if (portValue == "Ультразвуковой сенсор") {
				result.append("ecrobot_init_sonar_sensor(NXT_PORT_S" + QString::number(i) + ")");

			} else if (portValue == "Сенсор нажатия (булево значение)") {
			
			} else if (portValue == "Сенсор нажатия (сырое значение)") {

			} else if (portValue == "Сенсор цвета (полные цвета)") {
			} else if (portValue == "Сенсор цвета (красный)") {
			} else if (portValue == "Сенсор цвета (зеленый)") {
			} else if (portValue == "Сенсор цвета (синий)") {
			} else if (portValue == "Сенсор цвета (пассивный)") {
			}
		}
	}

	//for InitialNode returns empty list

	return result;
}
	
QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::SimpleElementGenerator::loopPrefixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("while (true) {", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::SimpleElementGenerator::loopPostfixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::LoopElementGenerator::loopPrefixCode() {
	QList< QPair<QString, qReal::Id> > result;
	
	qReal::Id logicElementId = mNxtGen->mApi->logicalId(mElementId); //TODO
	result << QPair<QString, qReal::Id>("for (int __iter__ = ; __iter__ < " +
			mNxtGen->mApi->property(logicElementId, "Iterations").toString()
		       	+ "; __iter__++) {", mElementId); //TODO
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::LoopElementGenerator::loopPostfixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::IfElementGenerator::loopPrefixCode() {
	QList< QPair<QString, qReal::Id> > result;
	
	qReal::Id logicElementId = mNxtGen->mApi->logicalId(mElementId); //TODO
	result << QPair<QString, qReal::Id>("while (" +
			mNxtGen->mApi->property(logicElementId, "Condition").toString()
		       	+ ") {", mElementId); //TODO
	return result;
}

QList< QPair<QString, qReal::Id> > NxtOSEKRobotGenerator::IfElementGenerator::loopPostfixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

bool NxtOSEKRobotGenerator::SimpleElementGenerator::preGenerationCheck() {
	IdList outgoingConnectedElements = mNxtGen->mApi->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		//case of error in diagram
		qDebug() << "Error! There are more than 1 outgoing connected elements with simple robot" <<
			"element!";
		return false;
	}

	return true;
}

bool NxtOSEKRobotGenerator::LoopElementGenerator::preGenerationCheck() {
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);

	if ((outgoingLinks.size() != 2) ||
		( (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString() == "Итерация") 
		  && 
		  (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(1)), "Guard").toString() == "Итерация") )
	)
		return false;

	return true;
}

bool NxtOSEKRobotGenerator::IfElementGenerator::preGenerationCheck() {
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);

	if (outgoingLinks.size() != 2) //TODO: append checking arrows
		return false;

	return true;
}

bool NxtOSEKRobotGenerator::SimpleElementGenerator::nextElementsGeneration() {
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

bool NxtOSEKRobotGenerator::LoopElementGenerator::nextElementsGeneration() {
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);
	// outgoingLinks.size() must be 2!

	int elementConnectedByIterationEdgeNumber = -1;
	int afterLoopElementNumber = -1;

	/*
	qDebug() << "DDD 0" << mNxtGen->mApi->stringProperty(mNxtGen->mApi->logicalId(outgoingLinks.at(0)), "Guard");
	qDebug() << "DDD 0" << mNxtGen->mApi->logicalId(outgoingLinks.at(0)).toString();
	
	qDebug() << "DDD 1" << mNxtGen->mApi->stringProperty(mNxtGen->mApi->logicalId(outgoingLinks.at(1)), "Guard");
	qDebug() << "DDD 1" << QString::fromLocal8Bit(mNxtGen->mApi->stringProperty(mNxtGen->mApi->logicalId(outgoingLinks.at(1)), "Guard").toAscii());
	qDebug() << "DDD 1" << (mNxtGen->mApi->stringProperty(mNxtGen->mApi->logicalId(outgoingLinks.at(1)), "Guard") == "ппп");
	qDebug() << "DDD 1" << mNxtGen->mApi->logicalId(outgoingLinks.at(1)).toString();
	*/
	
	//Грязный хак! Почему-то неправильно читается русский
	if (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString().toUtf8() == "итерация") {
	//if (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(1)), "Guard").toString() == "") {
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

bool NxtOSEKRobotGenerator::IfElementGenerator::generateBranch(int branchNumber) {
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);

	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mNxtGen, 
			mNxtGen->mApi->to(outgoingLinks.at(branchNumber)));

	mNxtGen->mPreviousElement = mElementId;
	//mNxtGen->mPreviousLoopElements.push(mElementId);
	if (!nextBlocksGen->generate())
		return false;
	delete nextBlocksGen;

	return true;
}

bool NxtOSEKRobotGenerator::IfElementGenerator::nextElementsGeneration() {
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);
	// outgoingLinks.size() must be 2!
	
	int conditionArrowNum = -1;

	//we search for arrow with condition
	if (mNxtGen->mApi->property(mNxtGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString() == "") {
		conditionArrowNum = 1;
	} else {
		conditionArrowNum = 0;
	}

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

bool NxtOSEKRobotGenerator::AbstractElementGenerator::generate() {
	if (!preGenerationCheck()) {
		return false;
	}

	if (mNxtGen->mElementToStringListNumbers.contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection
		
		qReal::Id loopElement = mElementId;
		if (!mNxtGen->mPreviousLoopElements.empty()) {
			loopElement = mNxtGen->mPreviousLoopElements.pop();
		}

		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mNxtGen, loopElement);

		mNxtGen->mGeneratedStringSet[mNxtGen->mElementToStringListNumbers[loopElement.toString()].pop()]
			+= loopElementGen->loopPrefixCode();

		mNxtGen->mGeneratedStringSet << loopElementGen->loopPostfixCode();

		return true;
	}

	//in case element has more then 1 incoming connection
	//means that element has incoming connections from another elements, we haven`t already observed
	createListsForIncomingConnections();

	if (!nextElementsGeneration()) {
		return false;
	}

	return true;
}
