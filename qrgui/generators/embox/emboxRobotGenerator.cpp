#include <QTextStream>
#include <cmath>
#include <QtCore/QObject>
#include "emboxRobotGenerator.h"

#include <QDebug>

using namespace qReal;
using namespace generators;

EmboxRobotGenerator::EmboxRobotGenerator(qrRepo::RepoApi *api, QString const &destinationPath)
	:  mDestinationPath(destinationPath) {
		mIsNeedToDeleteMApi = false;
		mApi = api;
}

EmboxRobotGenerator::EmboxRobotGenerator(QString const &pathToRepo, QString const &destinationPath)
	:mDestinationPath(destinationPath) {
		mIsNeedToDeleteMApi = true;
		mApi = new qrRepo::RepoApi(pathToRepo);
}

EmboxRobotGenerator::~EmboxRobotGenerator() {
	if (mApi && mIsNeedToDeleteMApi)
		delete mApi;
}

void EmboxRobotGenerator::generate() {
	IdList initialNodes = mApi->elementsByType("InitialNode");

	int curInitialNodeNumber = 0;
	foreach (Id curInitialNode, initialNodes) {
		if (mApi->isGraphicalElement(curInitialNode)) {
			QFile resultFile("prog" + QString::number(curInitialNodeNumber) + ".c");
			if (!resultFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				qDebug() << "cannot open \"" << resultFile.fileName() << "\"";
				return;
			}

			QTextStream out(&resultFile);

			mGeneratedStringSet.clear();
			mElementToStringListNumbers.clear();
			
			AbstractElementGenerator* gen = ElementGeneratorFactory::generator(this, curInitialNode);
			mPreviousElement = curInitialNode;
			gen->generate();
			QList< QPair<QString, qReal::Id> > stringPairList;
			foreach (stringPairList, mGeneratedStringSet) {
				QPair<QString, qReal::Id> stringPair;
				foreach (stringPair, stringPairList) {
					out << stringPair.first << "\n";
				}
			} //TODO
			out.flush();
			delete gen;

			resultFile.close();

			curInitialNodeNumber++;
		}
	}
}

/*
QString EmboxRobotGenerator::generateByElement(qReal::Id element, qReal::Id previousElement) {
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

EmboxRobotGenerator::AbstractElementGenerator::AbstractElementGenerator(EmboxRobotGenerator *emboxGen,
		qReal::Id elementId): mEmboxGen(emboxGen), mElementId(elementId) {	
}

void EmboxRobotGenerator::AbstractElementGenerator::createListsForIncomingConnections() {
	//connects string lists in mGeneratedStringSet with mElementId in mElementToStringListNumbers
	for (int i = 1; i < mEmboxGen->mApi->incomingConnectedElements(mElementId).size(); i++) {
		mEmboxGen->mGeneratedStringSet << QList< QPair<QString, qReal::Id> >();
		mEmboxGen->mElementToStringListNumbers[mElementId.toString()] << mEmboxGen->mGeneratedStringSet.size() - 1;
	}
}

EmboxRobotGenerator::SimpleElementGenerator::SimpleElementGenerator(EmboxRobotGenerator *emboxGen,
		qReal::Id elementId): 
	AbstractElementGenerator(emboxGen, elementId) {
}

EmboxRobotGenerator::LoopElementGenerator::LoopElementGenerator(EmboxRobotGenerator *emboxGen,
		qReal::Id elementId): 
	AbstractElementGenerator(emboxGen, elementId) {
}

EmboxRobotGenerator::IfElementGenerator::IfElementGenerator(EmboxRobotGenerator *emboxGen,
		qReal::Id elementId): 
	AbstractElementGenerator(emboxGen, elementId) {
}

QList< QPair<QString, qReal::Id> > EmboxRobotGenerator::SimpleElementGenerator::simpleCode() {
	QList< QPair<QString, qReal::Id> > result;

	qReal::Id logicElementId = mEmboxGen->mApi->logicalId(mElementId); //TODO

	if (mElementId.element() == "EnginesForward") {
		result.append(QPair<QString, qReal::Id>(
					"motor_set_power(MOTOR0, " + QString::number(360 * mEmboxGen->mApi->property(logicElementId, "Power").toInt()) 
					+ " / 100.0);",
					mElementId));

	} else if (mElementId.element() == "EnginesBackward") {
		result.append(QPair<QString, qReal::Id>(
					"motor_set_power(MOTOR0, " + QString::number(-360 * mEmboxGen->mApi->property(logicElementId, "Power").toInt()) 
					+ " / 100.0);",
					mElementId));


	} else if (mElementId.element() == "EnginesStop") {
		result.append(QPair<QString, qReal::Id>(
					"motor_set_power(MOTOR0, 0);",
					mElementId));

	} else if (mElementId.element() == "Timer") {
		result.append(QPair<QString, qReal::Id>(
					"usleep(" + QString::number(mEmboxGen->mApi->property(logicElementId, "Delay").toInt()) + ")",
					mElementId));

	} else if (mElementId.element() == "Beep") {
		result.append(QPair<QString, qReal::Id>(
					"__BEEP__", //TODO
					mElementId));

	} else if (mElementId.element() == "Function") {
		result.append(QPair<QString, qReal::Id>(
					"Function:  " + mEmboxGen->mApi->property(logicElementId, "Body").toString(),
					mElementId));

	} else if (mElementId.element() == "FinalNode") {
		result.append(QPair<QString, qReal::Id>(
					"return;",
					mElementId));
	}

	//for InitialNode returns empty list

	return result;
}
	
QList< QPair<QString, qReal::Id> > EmboxRobotGenerator::SimpleElementGenerator::loopPrefixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("while (true) {", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > EmboxRobotGenerator::SimpleElementGenerator::loopPostfixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > EmboxRobotGenerator::LoopElementGenerator::loopPrefixCode() {
	QList< QPair<QString, qReal::Id> > result;
	
	qReal::Id logicElementId = mEmboxGen->mApi->logicalId(mElementId); //TODO
	result << QPair<QString, qReal::Id>("for (int __iter__ = ; __iter__ < " +
			mEmboxGen->mApi->property(logicElementId, "Iterations").toString()
		       	+ "; __iter__++) {", mElementId); //TODO
	return result;
}

QList< QPair<QString, qReal::Id> > EmboxRobotGenerator::LoopElementGenerator::loopPostfixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

QList< QPair<QString, qReal::Id> > EmboxRobotGenerator::IfElementGenerator::loopPrefixCode() {
	QList< QPair<QString, qReal::Id> > result;
	
	qReal::Id logicElementId = mEmboxGen->mApi->logicalId(mElementId); //TODO
	result << QPair<QString, qReal::Id>("while (" +
			mEmboxGen->mApi->property(logicElementId, "Condition").toString()
		       	+ ") {", mElementId); //TODO
	return result;
}

QList< QPair<QString, qReal::Id> > EmboxRobotGenerator::IfElementGenerator::loopPostfixCode() {
	QList< QPair<QString, qReal::Id> > result;
	result << QPair<QString, qReal::Id>("}", mElementId);
	return result;
}

bool EmboxRobotGenerator::SimpleElementGenerator::preGenerationCheck() {
	IdList outgoingConnectedElements = mEmboxGen->mApi->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		//case of error in diagram
		qDebug() << "Error! There are more than 1 outgoing connected elements with simple robot" <<
			"element!";
		return false;
	}

	return true;
}

bool EmboxRobotGenerator::LoopElementGenerator::preGenerationCheck() {
	IdList outgoingLinks = mEmboxGen->mApi->outgoingLinks(mElementId);

	if ((outgoingLinks.size() != 2) ||
		( (mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString() == "Итерация") 
		  && 
		  (mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(1)), "Guard").toString() == "Итерация") )
	)
		return false;

	return true;
}

bool EmboxRobotGenerator::IfElementGenerator::preGenerationCheck() {
	IdList outgoingLinks = mEmboxGen->mApi->outgoingLinks(mElementId);

	if (outgoingLinks.size() != 2) //TODO: append checking arrows
		return false;

	return true;
}

bool EmboxRobotGenerator::SimpleElementGenerator::nextElementsGeneration() {
	IdList outgoingConnectedElements = mEmboxGen->mApi->outgoingConnectedElements(mElementId);
	mEmboxGen->mGeneratedStringSet << simpleCode();

	if (outgoingConnectedElements.size() == 1) {
		AbstractElementGenerator* gen = ElementGeneratorFactory::generator(mEmboxGen, outgoingConnectedElements.at(0));
		mEmboxGen->mPreviousElement = mElementId;
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

bool EmboxRobotGenerator::LoopElementGenerator::nextElementsGeneration() {
	IdList outgoingLinks = mEmboxGen->mApi->outgoingLinks(mElementId);
	// outgoingLinks.size() must be 2!

	int elementConnectedByIterationEdgeNumber;
	int afterLoopElementNumber;

	qDebug() << "DDD 0" << mEmboxGen->mApi->stringProperty(mEmboxGen->mApi->logicalId(outgoingLinks.at(0)), "Guard");
	qDebug() << "DDD 0" << (mEmboxGen->mApi->stringProperty(mEmboxGen->mApi->logicalId(outgoingLinks.at(0)), "Guard") == (QObject::tr("Iteration")));
	/*
	qDebug() << "DDD 0" << QString::fromUtf8(
			mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString().toAscii().data());
	qDebug() << "DDD 0" << mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString().toAscii().data();
	qDebug() << "DDD 1" << mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(1)), "Guard").toString();
	*/
	
	//Грязный хак! Почему-то неправильно читается русский
	//if (mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString() == "итерация") {
	if (mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(1)), "Guard").toString() == "") {
		elementConnectedByIterationEdgeNumber = 0;
		afterLoopElementNumber = 1;
	} else {
		elementConnectedByIterationEdgeNumber = 1;
		afterLoopElementNumber = 0;
	}

	//generate loop	
	AbstractElementGenerator* loopGen = ElementGeneratorFactory::generator(mEmboxGen, 
			mEmboxGen->mApi->to(outgoingLinks.at(elementConnectedByIterationEdgeNumber)));

	mEmboxGen->mPreviousElement = mElementId;
	mEmboxGen->mPreviousLoopElements.push(mElementId);
	if (!loopGen->generate())
		return false;
	delete loopGen;

	//generate next blocks
	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mEmboxGen, 
			mEmboxGen->mApi->to(outgoingLinks.at(afterLoopElementNumber)));
	
	mEmboxGen->mPreviousElement = mElementId;
	mEmboxGen->mPreviousLoopElements.push(mElementId);
	if (!nextBlocksGen->generate())
		return false;
	delete nextBlocksGen;

	return true;
}

bool EmboxRobotGenerator::IfElementGenerator::generateBranch(int branchNumber) {
	IdList outgoingLinks = mEmboxGen->mApi->outgoingLinks(mElementId);

	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mEmboxGen, 
			mEmboxGen->mApi->to(outgoingLinks.at(branchNumber)));

	mEmboxGen->mPreviousElement = mElementId;
	//mEmboxGen->mPreviousLoopElements.push(mElementId);
	if (!nextBlocksGen->generate())
		return false;
	delete nextBlocksGen;

	return true;
}

bool EmboxRobotGenerator::IfElementGenerator::nextElementsGeneration() {
	IdList outgoingLinks = mEmboxGen->mApi->outgoingLinks(mElementId);
	// outgoingLinks.size() must be 2!
	
	int conditionArrowNum;

	//we search for arrow with condition
	if (mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(0)), "Guard").toString() == "") {
		conditionArrowNum = 1;
	} else {
		conditionArrowNum = 0;
	}

	qReal::Id logicElementId = mEmboxGen->mApi->logicalId(mElementId); //TODO

	//TODO: save number of new created list
	QList< QPair<QString, qReal::Id> > ifBlockPrefix;
	QString condition = mEmboxGen->mApi->property(logicElementId, "Condition").toString();

	//Грязное место!
	if (mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(conditionArrowNum)), "Guard") == "меньше 0") {
		condition += " < 0";
	} else if (mEmboxGen->mApi->property(mEmboxGen->mApi->logicalId(outgoingLinks.at(conditionArrowNum)), "Guard") == "больше 0") {
		condition += " > 0";
	} else {
		condition += " == 0";
	}

	ifBlockPrefix << QPair<QString, qReal::Id>("if (" + condition + ") {", mElementId);
	mEmboxGen->mGeneratedStringSet << ifBlockPrefix;

	//generate true/false blocks
	generateBranch(conditionArrowNum);
	QList< QPair<QString, qReal::Id> > elseBlock;
	elseBlock << QPair<QString, qReal::Id>("} else {", mElementId);
	mEmboxGen->mGeneratedStringSet << elseBlock;
	generateBranch(1 - conditionArrowNum);

	QList< QPair<QString, qReal::Id> > ifBlockPostfix;
	ifBlockPostfix << QPair<QString, qReal::Id>("}", mElementId);
	mEmboxGen->mGeneratedStringSet << ifBlockPostfix;

	return true;
}

bool EmboxRobotGenerator::AbstractElementGenerator::generate() {
	if (!preGenerationCheck()) {
		return false;
	}

	if (mEmboxGen->mElementToStringListNumbers.contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection
		
		qReal::Id loopElement = mElementId;
		if (!mEmboxGen->mPreviousLoopElements.empty()) {
			loopElement = mEmboxGen->mPreviousLoopElements.pop();
		}

		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mEmboxGen, loopElement);

		mEmboxGen->mGeneratedStringSet[mEmboxGen->mElementToStringListNumbers[loopElement.toString()].pop()]
			+= loopElementGen->loopPrefixCode();

		mEmboxGen->mGeneratedStringSet << loopElementGen->loopPostfixCode();

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
