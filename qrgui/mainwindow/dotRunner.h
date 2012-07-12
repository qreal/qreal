#pragma once

#include <QtCore/QProcess>
#include <QtCore/QByteArray>
#include <QtCore/QPair>
#include "../models/graphicalModelAssistApi.h"
#include "../models/logicalModelAssistApi.h"
#include "../pluginManager/editorManager.h"
#include "../../qrkernel/ids.h"

namespace qReal {

// TODO: comments are required
class DotRunner : public QObject
{
	Q_OBJECT
public:
	explicit DotRunner(Id const &diagramId
			, models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, EditorManager const &editorManager, QString const &absolutePathToDotFiles);

	void run(QString const &algorithm);

public slots:
	void readFromProcess();

private:
	QString nameOfElement(Id const &id);
	void parseDOTCoordinates();
	void buildSubgraph(QTextStream &out, Id const &id, int &index);
	void writeGraphToDotFile(QTextStream &outFile, Id const &id);

	QProcess mProcess;
	QByteArray mData;
	Id mDiagramId;
	models::GraphicalModelAssistApi const &mGraphicalModelApi;
	models::LogicalModelAssistApi const &mLogicalModelApi;
	EditorManager const &mEditorManager;
	QHash <QString, Id> mElementNamesForDOT;
	QHash <Id, QPair<QPointF, QPair<qreal, qreal> > > mDOTCoordinatesOfElements;
	QString mAlgorithm;
	QString mAbsolutePathToDotFiles;
};
}
