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

<<<<<<< HEAD
		virtual QString generateIsNodeOrEdge(const QString &lineTemplate) const;
		virtual QString generateNodeClass(const QString &classTemplate);
		virtual QString generateEdgeClass(const QString &classTemplate) const;
		virtual QString generateResourceLine(const QString &resourceTemplate) const;
=======
		virtual QString generateIsNodeOrEdge(QString const &lineTemplate) const;
		virtual QString generateNodeClass(QString const &classTemplate);
		virtual QString generateEdgeClass(QString const &classTemplate) const;
		virtual QString generateResourceLine(QString const &resourceTemplate) const;
		QString generatePorts(QStringList const &portTypes) const;
>>>>>>> 2a644e69703bdf2b2ea8122ede877834aceddde1

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
