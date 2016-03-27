/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <QtCore/QList>

#include "graphicType.h"
#include "label.h"

namespace qrmc {

/// Represents edge of a model graph (a link on a diagram).
class EdgeType : public GraphicType
{
public:
	/// Constructor.
	/// @param diagram - diagram this edge belongs to.
	/// @param api - repository with metamodel.
	/// @param id - id of an edge in repository.
	/// @param targetDirectory - directory to generate editor to.
	EdgeType(const Diagram &diagram, const qrRepo::LogicalRepoApi &api, const qReal::Id &id, const QString &targetDirectory);

	~EdgeType() override;

	Type* clone() const override;

	bool isGraphicalType() const override;

	QString generateIsNodeOrEdge(const QString &lineTemplate) const override;
	QString generateNodeClass(const QString &classTemplate) override;
	QString generateEdgeClass(const QString &classTemplate) const override;
	QString generateResourceLine(const QString &resourceTemplate) const override;
	QString generatePorts(const QStringList &portTypes) const;

	void print() override;

private:
	void initLabels();
	void generateSdf() const;
	void generateArrows(QString &edgeClass) const;
	void generateArrowEnd(QString &edgeClass, const QString &arrowEnd,
			const QString &customTag, const QString &brushTag) const;

	QString mBeginType;
	QString mEndType;
	QString mLineType;
	QStringList mFromPorts;

	/// All labels of this edge.
	/// @todo: refactor after #349 is closed
	/// Has ownership.
	QList<Label*> mLabels;
};

}
