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

#include "graphicType.h"
#include "label.h"

#include <QList>

namespace qrmc {
	class EdgeType : public GraphicType
	{
	public:
		EdgeType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id);
		virtual Type* clone() const;
		virtual ~EdgeType();

		virtual bool isGraphicalType() const;

		virtual QString generateIsNodeOrEdge(const QString &lineTemplate) const;
		virtual QString generateNodeClass(const QString &classTemplate);
		virtual QString generateEdgeClass(const QString &classTemplate) const;
		virtual QString generateResourceLine(const QString &resourceTemplate) const;
		QString generatePorts(const QStringList &portTypes) const;


		void print();

		void initLabels();
	private:

		void generateSdf() const;
		void generateArrows(QString &edgeClass) const;
		void generateArrowEnd(QString &edgeClass, const QString &arrowEnd,
							  const QString &customTag, const QString &brushTag) const;

		QString mBeginType;
		QString mEndType;
		QString mLineType;
		QStringList mFromPorts;
		QList<Label*> mLabels; // refactor after #349 is closed
	};
}
