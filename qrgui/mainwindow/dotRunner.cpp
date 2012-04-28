#include <QtCore/QDebug>

#include "dotRunner.h"

using namespace qReal;

DotRunner::DotRunner(Id const &diagramId,
					 models::GraphicalModelAssistApi const &graphicalModelApi,
					 models::LogicalModelAssistApi const &logicalModelApi,
					 EditorManager const &editorManager, QString const &absolutePathToDotFiles)
	: QObject(NULL)
	, mDiagramId(diagramId)
	, mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mEditorManager(editorManager)
	, mAlgorithm("")
	, mAbsoluteFilesToDotFiles(absolutePathToDotFiles)
{
	QObject::connect(&mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromProcess()));
}

void DotRunner::readFromProcess()
{
	mData = mProcess.readAllStandardOutput();
}

void DotRunner::run(const QString &algorithm)
{
	mAlgorithm = algorithm;
	QFile data(mAbsoluteFilesToDotFiles + "/graph1.dot");
	if (data.open(QFile::WriteOnly | QFile::Truncate)) {
		QTextStream outFile(&data);
		outFile << "digraph G { \n";
		if (mAlgorithm != "")
			outFile << QString("rankdir=%1; \n").arg(mAlgorithm);
		IdList const childrenId = mGraphicalModelApi.children(mDiagramId);
		int index = 1;
		foreach (Id id, childrenId) {
			if (mEditorManager.isGraphicalElementNode(id)) {
				buildSubgraph(outFile, id, index);
				writeGraphToDotFile(outFile, id);
			}
		}
		outFile << "}";
		data.close();
		mProcess.setWorkingDirectory(mAbsoluteFilesToDotFiles);
		mProcess.start("dot.exe graph1.dot");
		if (!mProcess.waitForFinished())
			return;
		parseDOTCoordinates();
	}
}

void DotRunner::writeGraphToDotFile(QTextStream &outFile, const Id &id)
{
	IdList outgoingLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
	if (outgoingLinks.isEmpty() && mLogicalModelApi.logicalRepoApi().incomingLinks(id).isEmpty())
		outFile << nameOfElement(id) << ";\n";
	foreach (Id linkId, outgoingLinks) {
		Id const elementId = mGraphicalModelApi.graphicalRepoApi().otherEntityFromLink(linkId, id);
		if (mEditorManager.isGraphicalElementNode(elementId)) {
			outFile << nameOfElement(id) << " -> " << nameOfElement(elementId) << ";\n";
		}
	}
}

void DotRunner::buildSubgraph(QTextStream &out, const Id &id, int &index)
{
	IdList const childrenId = mGraphicalModelApi.children(id);
	if (childrenId.isEmpty())
		return;
	out << QString("subgraph %1 {\n").arg(index);
	index++;
	foreach (Id childId, childrenId) {
		buildSubgraph(out, childId, index);
		writeGraphToDotFile(out, childId);
	}
	out << "}\n";
}

QString DotRunner::nameOfElement(Id const &id)
{
	QString idToString = id.toString();
	idToString.chop(1);
	idToString = idToString.mid(idToString.indexOf("{") + 1).remove("-"
	).replace("0", "a").replace("1", "b").replace("2", "c"
	).replace("3", "d").replace("4", "e").replace("5", "f"
	).replace("6", "g").replace("7", "h").replace("8", "i"
	).replace("9", "j");
	mElementNamesForDOT.insert(idToString, id);
	return idToString;
}

void DotRunner::parseDOTCoordinates()
{
	QString data = QString(mData);
	QStringList list = data.split("\n", QString::SkipEmptyParts);
	QRegExp regexp("\\s*(\\w+)\\s\\[pos=\"(\\d+\\,\\d+)\"\\,\\swidth=\"(\\d+\\.\\d+)\",\\sheight=\"(\\d+\\.\\d+)\"\\]" );

	foreach (QString const &string, list) {
		if (string.indexOf(regexp) == -1)
			continue;
		Id const id = mElementNamesForDOT[regexp.capturedTexts().at(1)];
		QStringList qpointFCoordinates = regexp.capturedTexts().at(2).split(",", QString::SkipEmptyParts);
		if (qpointFCoordinates.count() < 2)
			continue;
		QPointF pointF = QPointF(qpointFCoordinates.at(0).toDouble(), qpointFCoordinates.at(1).toDouble());
		QPair<qreal, qreal> pair = qMakePair(regexp.capturedTexts().at(3).toDouble(), regexp.capturedTexts().at(4).toDouble());
		mDOTCoordinatesOfElements.insert(id, qMakePair(pointF, pair));
	}
	foreach (Id const &id, mDOTCoordinatesOfElements.keys()) {
		QPolygon configuration = mGraphicalModelApi.configuration(id);
		int width = 0;
		int height = 0;
		if (!configuration.isEmpty()) {
		width = configuration.at(1).x() - configuration.at(0).x();
		height = configuration.at(3).y() - configuration.at(1).y();
		}
		else {
			width = 50; // because of bug about not saving configuration of element during it's creation
			height = 50;
		}
		qreal x = mDOTCoordinatesOfElements[id].first.x();
		qreal y = mDOTCoordinatesOfElements[id].first.y();
		if (mAlgorithm == "BT" || mAlgorithm == "TB") {
			x -= width / 4.0;
			y -= height / 4.0;
		}
		else if (mAlgorithm == "LR" || mAlgorithm == "RL") {
			x /= 3.0; // special for robots diagram
			x -= height / 4.0;
			y -= width / 4.0;
		}

		mGraphicalModelApi.mutableGraphicalRepoApi().setPosition(id, QPointF(x, y));
		mGraphicalModelApi.mutableGraphicalRepoApi().setConfiguration(id, QPolygon(QRect(x, y, width, height)));
	}
}
