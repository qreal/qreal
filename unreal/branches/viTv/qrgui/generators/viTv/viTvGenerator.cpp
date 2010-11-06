#include "viTvGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QPolygon>

#include <QtCore/QDebug>

#include "../../kernel/roles.h"
#include "../../../qrrepo/repoApi.h"
#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using utils::OutFile;

ViTvGenerator::ViTvGenerator(qrRepo::RepoApi const &api)
	: mCurrentSupportStateIndex(0), mApi(api)
{
}

gui::ErrorReporter ViTvGenerator::generate()
{
	Id rootId = ROOT_ID;
	IdList rootDiagrams = mApi.children(rootId);

	foreach (Id const diagram, rootDiagrams) {
		if (diagram.element() == "StateMachineDiagram")
			generateDiagram(diagram);
	}

	return mErrorReporter;
}

void ViTvGenerator::generateDiagram(Id const &id)
{
	QString outputDirectory = ".";
	QString const directory = mApi.stringProperty(id, "outputDirectory");
	if (!directory.isEmpty())
		outputDirectory = directory;
	collectStates(id);
	generateHeader(outputDirectory, id);
	generateCpp(outputDirectory, id);
}

void ViTvGenerator::generateHeader(QString const &outputDirectory, Id const &id) const
{
	OutFile out(outputDirectory + "/" + "trackingStateMachine.h");
	out.setIndentOptions(OutFile::space, 4);

	out() << "#pragma once\n\n";
	out() << "#include \"abstractTrackingStateMachine.h\"\n\n";
	out() << "class TrackingStateMachine: public AbstractTrackingStateMachine\n";
	out() << "{\n";

	out.incIndent();
	out() << "Q_OBJECT\n\n";
	out.decIndent();

	out() << "public:\n";

	out.incIndent();
	out() << "TrackingStateMachine();\n\n";
	out() << "void addMeasure(Point3d const &point);\n";
	out() << "void moveTimersToCorrectThread();\n\n";
	out.decIndent();

	out() << "private:\n";

	out.incIndent();
	out() << "enum State\n";
	out() << "{\n";
	out.incIndent();
	out() << "init\n";
	foreach (QString const &state, mStates) {
		out() << ", " + state + "\n";
	}
	out.decIndent();

	out() << "};\n\n";

	out() << "State mState;\n\n";
	out() << "QString stateToString(State state) const;\n";
	out() << "void setState(State state);\n";
	out.decIndent();
	out() << "};\n";
}

void ViTvGenerator::generateCpp(QString const &outputDirectory, Id const &id)
{
	OutFile out(outputDirectory + "/" + "trackingStateMachine.cpp");
	out.setIndentOptions(OutFile::space, 4);

	out() << "#include \"trackingStateMachine.h\"\n\n";
	out() << "#include <QtCore/QDebug>\n\n";

	out() << "TrackingStateMachine::TrackingStateMachine()\n";

	out.incIndent();
	out() << ": AbstractTrackingStateMachine(), mState(init)\n";
	out.decIndent();

	out() << "{\n";
	out() << "}\n\n";

	out() << "void TrackingStateMachine::moveTimersToCorrectThread()\n";
	out() << "{\n";
	out.incIndent();
	out() << "AbstractTrackingStateMachine::moveTimersToCorrectThread();\n";
	out.decIndent();

	out() << "}\n\n";

	out() << "void TrackingStateMachine::addMeasure(Point3d const &point)\n";
	out() << "{\n";

	out.incIndent();
	out() << "qDebug() << \"==========================================================\";\n";
	out() << "qDebug() << \"added measure: \" << point.toString();\n";
	out() << "qDebug() << \"Current state:\" << stateToString(mState);\n\n";
	out() << "Direction movementDirection = direction(point);\n";

	out() << "bool processingFinished = false;\n";
	out() << "while (!processingFinished)\n";
	out() << "{\n";
	out.incIndent();
	out() << "processingFinished = true;\n";
	out() << "switch (mState)\n";
	out() << "{\n";

	foreach (Id const &element, mApi.children(id))
		if (element.element() == "Initial" || element.element() == "State")
			generateStateTransitions(out, element);

	out() << "}\n";
	out.decIndent();
	out() << "}\n";
	out.decIndent();
	out() << "}\n\n";

	out() << "QString TrackingStateMachine::stateToString(State state) const\n";
	out() << "{\n";
	out.incIndent();

	out() << "if (state == init)\n";
	out() << "{\n";
	out.incIndent();
	out() << "return \"init\";\n";
	out.decIndent();
	out() << "}\n";

	foreach (QString state, mStates) {
		out() << "if (state == " << state << ")\n";
		out() << "{\n";
		out.incIndent();
		out() << "return \"" << state << "\";\n";
		out.decIndent();
		out() << "}\n";
	}
	out() << "return \"\";\n";

	out.decIndent();
	out() << "}\n\n";

	out() << "void TrackingStateMachine::setState(State state)\n";
	out() << "{\n";
	out.incIndent();

	out() << "qDebug() << \"Changing state from \" << stateToString(mState) << \"to\"\n";
	out.incIndent();
	out.incIndent();
	out() << "<< stateToString(state);\n";
	out.decIndent();
	out.decIndent();
	out() << "mState = state;\n";
	out.decIndent();
	out() << "}\n";
}

void ViTvGenerator::generateStateTransitions(OutFile &out, Id const &id)
{
	out.incIndent();

	if (id.element() == "Initial")
		out() << "case init:\n";
	else
		out() << "case " << mApi.name(id) << ":\n";
	out() << "{\n";

	QList<SupportState> supportStates;

	IdList const outgoingLinks = mApi.outgoingLinks(id);
	foreach (Id const &link, outgoingLinks) {
		if (link.element() == "ConditionalTransition") {
			out.incIndent();
			out() << "if (" << mApi.stringProperty(link, "condition") << ")\n";
			out() << "{\n";

			out.incIndent();

			QString const event = mApi.stringProperty(link, "event");
			if (!event.isEmpty())
				out() << "emit gestureDetected(" << event << ");\n";

			QString const nextState = mApi.name(mApi.otherEntityFromLink(link, id));
			out() << "setState(" << nextState << ");\n";

			QString const isAccepted = mApi.stringProperty(link, "isAccepted");
			if (isAccepted == "false")
				out() << "processingFinished = false;\n";

			out.decIndent();

			out() << "}\n";
			out.decIndent();
		} else if (link.element() == "Movement") {
			out.incIndent();

			out() << "if (movementDirection == " << movementDirection(link) << ")\n";
			out() << "{\n";

			out.incIndent();

			out() << "startMovement();\n";

			QString const beginEvent = mApi.stringProperty(link, "beginEvent");
			if (!beginEvent.isEmpty())
				out() << "emit gestureDetected(" << beginEvent << ")\n";

			QString const supportStateId = "movementState" + QString::number(++mCurrentSupportStateIndex);
			out() << "setState(" << supportStateId << ");\n";

			out.decIndent();

			out() << "}\n";

			out.decIndent();

			SupportState supportState;
			supportState.stateId = supportStateId;
			supportState.direction = movementDirection(link);
			supportState.endEvent = mApi.stringProperty(link, "endEvent");
			supportState.sourceState = mApi.name(id);
			supportState.targetState = mApi.name(mApi.otherEntityFromLink(link, id));

			supportStates.append(supportState);
		} else if (link.element() == "ForwardBackwardMovement") {
			out.incIndent();

			out() << "if (movementDirection == " << forwardBackwardMovementDirection(link) << ")\n";
			out() << "{\n";

			out.incIndent();

			out() << "startMovement();\n";

			QString const beginEvent = mApi.stringProperty(link, "beginEvent");
			if (!beginEvent.isEmpty())
				out() << "emit gestureDetected(" << beginEvent << ")\n";

			QString const supportStateId = "movementState" + QString::number(++mCurrentSupportStateIndex);
			out() << "setState(" << supportStateId << ");\n";

			out.decIndent();

			out() << "}\n";

			out.decIndent();

			SupportState supportState;
			supportState.stateId = supportStateId;
			supportState.direction = forwardBackwardMovementDirection(link);
			supportState.endEvent = mApi.stringProperty(link, "endEvent");
			supportState.sourceState = mApi.name(id);
			supportState.targetState = mApi.name(mApi.otherEntityFromLink(link, id));
			supportState.minDistance = mApi.stringProperty(link, "minDistance");

			supportStates.append(supportState);
		}
	}

	out.incIndent();
	out() << "break;\n";
	out.decIndent();
	out() << "}\n";

	foreach (SupportState state, supportStates) {
		out() << "case " << state.stateId << ":\n";
		out() << "{\n";
		out.incIndent();
		out() << "if (movementDirection != " << state.direction << ")\n";
		out() << "{\n";
		out.incIndent();

		if (!state.minDistance.isEmpty()) {
			out() << "if (movementDistance() < " << state.minDistance << ")\n";
			out() << "{\n";
			out.incIndent();
		}

		if (!state.endEvent.isEmpty())
			out() << "emit gestureDetected(" << state.endEvent << ");\n";

		out() << "setState(" << state.targetState << ");\n";
		out() << "processingFinished = false;\n";

		if (!state.minDistance.isEmpty()) {
			out.decIndent();
			out() << "}\n";
			out() << "else\n";
			out() << "{\n";
			out.incIndent();
			out() << "setState(" << state.sourceState << ");\n";
			out.decIndent();
			out() << "}\n";
		}

		out.decIndent();
		out() << "}\n";

		out() << "break;\n";
		out.decIndent();
		out() << "}\n";
	}

	out.decIndent();
}

void ViTvGenerator::collectStates(Id const &id)
{
	IdList elements = mApi.children(id);

	foreach (Id const element, elements)
		if (element.element() == "State")
			mStates.append(mApi.name(element));

	int generatedStatesCount = 1;
	foreach (Id const element, elements) {
		if (element.element() == "Movement" || element.element() == "ForwardBackwardMovement") {
			mStates.append("movementState" + QString::number(generatedStatesCount));
			++generatedStatesCount;
		}
	}
}

QPoint ViTvGenerator::linkVector(Id const &link) const
{
	Id const source = mApi.from(link);
	Id const destination = mApi.to(link);

	QPoint sourceCoords = mApi.property(source, "position").toPoint();
	sourceCoords += mApi.property(source, "configuration").value<QPolygon>().boundingRect().center();
	QPoint destinationCoords = mApi.property(destination, "position").toPoint();
	destinationCoords += mApi.property(destination, "configuration").value<QPolygon>().boundingRect().center();
	return destinationCoords - sourceCoords;
}

QString ViTvGenerator::movementDirection(Id const &movement) const
{
	QPoint const vector = linkVector(movement);
	if (qAbs(vector.x()) > qAbs(vector.y()))
		return vector.x() < 0 ? "left" : "right";
	if (qAbs(vector.x()) < qAbs(vector.y()))
		return vector.y() < 0 ? "up" : "down";
	return "none";
}

QString ViTvGenerator::forwardBackwardMovementDirection(Id const &movement) const
{
	QPoint const vector = linkVector(movement);
	return vector.y() < 0 ? "forward" : "backward";
}
