#pragma once

#include "graphicType.h"
#include "label.h"

#include <QList>

namespace qrmc {
	class EdgeType : public GraphicType
	{
	public:
		EdgeType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const &id);
		virtual Type* clone() const;
		virtual ~EdgeType();

		virtual bool isGraphicalType() const;

		virtual QString generateIsNodeOrEdge(QString const &lineTemplate) const;
		virtual QString generateNodeClass(QString const &classTemplate);
		virtual QString generateEdgeClass(QString const &classTemplate) const;
		virtual QString generateResourceLine(QString const &resourceTemplate) const;

		void print();

		void initLabels();
	private:

		void generateSdf() const;
		void generateArrows(QString &edgeClass) const;
		void generateArrowEnd(QString &edgeClass, QString const &arrowEnd,
							  QString const &customTag, QString const &brushTag) const;

		QString mBeginType;
		QString mEndType;
		QString mLineType;
		QList<Label*> mLabels; // refactor after #349 is closed
	};
}
