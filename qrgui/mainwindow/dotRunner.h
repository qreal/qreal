#pragma once

#include <QtCore/QProcess>
#include <QtCore/QByteArray>
#include <QtCore/QPair>

#include <qrkernel/ids.h>

#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"
#include "plugins/pluginManager/editorManager.h"

namespace qReal {

// TODO: comments are required
// TODO: required indeed
class DotRunner : public QObject
{
	Q_OBJECT
public:
	explicit DotRunner(
			Id const &diagramId
			, models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, EditorManagerInterface const &editorManagerProxy
			, QString const &absolutePathToDotFiles
			);

	bool run(QString const &algorithm);

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
	qReal::EditorManagerInterface const &mEditorManagerInterface;
	QHash <QString, Id> mElementNamesForDOT;
	QHash <Id, QPair<QPointF, QPair<qreal, qreal> > > mDOTCoordinatesOfElements;
	QString mAlgorithm;
	QString mAbsolutePathToDotFiles;
};
}
