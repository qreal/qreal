#include <QTextStream>
#include "emboxRobotGenerator.h"

#include <QDebug>

using namespace qReal;
using namespace generators;

EmboxRobotGenerator(qrRepo::LogicalRepoApi const &api, QString const &destinationPath)
	: mApi(api), mDestinationPath(destinationPath) {
}

void EmboxRobotGenerator::generate() {
	QLinkedList<Id> initialNodes = mApi.elementsByType("InitialNode");

	int curInitialNodeNumber = 0;
	foreach (Id curInitialNode, initialNodes) {
		QFile resultFile("prog" + QString::number(curInitialNodeNumber) + ".c");
		if (!resultFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			qDebug() << "cannot open \"" << resultFile.fileName() << "\"";

			QTextStream out(&resultFile);
			out << generateByElement(curInitialNode);
			resultFile.close();
		}

		curInitialNodeNumber++;
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

AbstractElementGenerator::AbstractElementGenerator(qrRepo::RepoApi const & api, qReal::Id elementId): mApi(api),
		mElementId(elementId) {	
}

/*
QLinkedList<QString> AbstractElementGenerator::generate() {
	QLinkedList<QString> result;

	return result;
}
*/

SimpleElementGenerator::SimpleElementGenerator(qrRepo::RepoApi const & api, qReal::Id elementId):
	AbstractElementGenerator(api, elementId) {
}

QLinkedList<QPair<QString, qReal::Id>> SimpleElementGenerator::simpleCode() {
	QLinkedList<QPair<QString, qReal::Id>> result;

	if (element.element() == "EnginesForward") {
		result.append(QPair<QString, qReal::Id>(
					"motor_set_power(MOTOR0, " + QString::number(360 * mApi.property(element, "Power").toInt()) + ");",
					mElementId));

	} else if (element.element() == "EnginesBackward") {
		result.append(QPair<QString, qReal::Id>(
					"motor_set_power(MOTOR0, " + QString::number(-360 * mApi.property(element, "Power").toInt()) + ");",
					mElementId));


	} else if (element.element() == "EnginesStop") {
		result.append(QPair<QString, qReal::Id>(
					"motor_set_power(MOTOR0, 0);",
					mElementId));

	} else if (element.element() == "Timer") {
		result.append(QPair<QString, qReal::Id>(
					"usleep(" + QString::number(mApi.property(element, "Delay").toInt()) + ")",
					mElementId));

	} else if (element.element() == "Beep")
		result.append(QPair<QString, qReal::Id>(
					"__BEEP__", //TODO
					mElementId));

	//for InitialNode and FinalNode returns empty list

	return result;
}

QLinkedList<QPair<QString, qReal::Id>> SimpleElementGenerator::generate() {
	QLinkedList<QPair<QString, qReal::Id>> result;

	if (mApi.incomingConnectedElements(mElementId).size() <= 1) {
		result = simpleCode();
		QList outgoingConnectedElements = mApi.outgoingConnectedElements();

		if ((outgoingConnectedElements.size() == 1) || 
				((mElementId.element() == "FinalNode") && (outgoingConnectedElements.size() == 0))) {
			AbstractElementGenerator* gen = ElementGeneratorFactory.generate(outgoingConnectedElements.at(0));
			return result.append(gen->generate());
		} else {
			if (outgoingConnectedElements.size() > 1) {
				qDebug() << "Error! There are more than 1 outgoing connected elements with simple robot" <<
				"element!";
				return QLinkedList<QPair<QString, qReal::Id>>();
			} else { 
				//case of error end of diagram
				qDebug() << "Error! There is no outgoing connected elements with no final node!";
				return QLinkedList<QPair<QString, qReal::Id>>();
			}
		}
	}

	//TODO: обработать случай прихода нескольких стрелок в элемент	

	return result;
}
