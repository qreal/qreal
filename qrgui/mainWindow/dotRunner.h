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
			const Id &diagramId
			, models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, const EditorManagerInterface &editorManagerProxy
			, const QString &absolutePathToDotFiles
			);

	bool run(const QString &algorithm);

public slots:
	void readFromProcess();

private:
	QString nameOfElement(const Id &id);
	void parseDOTCoordinates();
	void buildSubgraph(QTextStream &out, const Id &id, int &index);
	void writeGraphToDotFile(QTextStream &outFile, const Id &id);

	QProcess mProcess;
	QByteArray mData;
	Id mDiagramId;
	models::GraphicalModelAssistApi const &mGraphicalModelApi;
	models::LogicalModelAssistApi const &mLogicalModelApi;
	const qReal::EditorManagerInterface &mEditorManagerInterface;
	QHash <QString, Id> mElementNamesForDOT;
	QHash <Id, QPair<QPointF, QPair<qreal, qreal> > > mDOTCoordinatesOfElements;
	QString mAlgorithm;
	QString mAbsolutePathToDotFiles;
};
}
