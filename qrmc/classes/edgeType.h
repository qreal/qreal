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
		QString generatePorts(QStringList const &portTypes) const;

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
		QList<Label*> mLabels; // refactor after #349 is closed
		QStringList mFromPorts;
	};
}
