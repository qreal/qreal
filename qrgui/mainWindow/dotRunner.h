/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
			, const models::GraphicalModelAssistApi &graphicalModelApi
			, const models::LogicalModelAssistApi &logicalModelApi
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
	const models::GraphicalModelAssistApi &mGraphicalModelApi;
	const models::LogicalModelAssistApi &mLogicalModelApi;
	const qReal::EditorManagerInterface &mEditorManagerInterface;
	QHash <QString, Id> mElementNamesForDOT;
	QHash <Id, QPair<QPointF, QPair<qreal, qreal> > > mDOTCoordinatesOfElements;
	QString mAlgorithm;
	QString mAbsolutePathToDotFiles;
};
}
