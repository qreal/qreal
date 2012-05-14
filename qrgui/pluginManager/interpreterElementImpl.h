#pragma once

#include <QWidget>
#include <QList>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include "../editorPluginInterface/elementImpl.h"
#include "../editorPluginInterface/elementTitleHelpers.h"
#include "../editorPluginInterface/elementRepoInterface.h"
#include "../umllib/sdfRenderer.h"
#include "../../qrkernel/ids.h"
#include "../../qrrepo/repoApi.h"
#include "../../qrxc/graphicType.h"

typedef QPair<QPair<qReal::Id, qReal::Id>, QPair<bool, qReal::Id> > PossibleEdge;
typedef QPair<QPair<QString, QString>, QPair<bool, QString> > StringPossibleEdge;
typedef QPair<bool, qReal::Id> PossibleEdgeType;

namespace qReal {
	class InterpreterElementImpl : public ElementImpl
	{
	public:
		InterpreterElementImpl(qrRepo::RepoApi *repo, Id id);
		void init(QRectF &contents, QList<StatPoint> &pointPorts,
						  QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
						  QList<ElementTitleInterface*> &titles,
						  SdfRendererInterface *renderer, SdfRendererInterface *portRenderer);
		void init(ElementTitleFactoryInterface &factory,
						  QList<ElementTitleInterface*> &titles);
		void paint(QPainter *painter, QRectF &contents);
		void updateData(ElementRepoInterface *repo) const;
		bool isNode();
		bool hasPorts();
		bool isResizeable();
		Qt::PenStyle getPenStyle();
		int getPenWidth();
		QColor getPenColor();
		void drawStartArrow(QPainter *painter) const;
		void drawEndArrow(QPainter *painter) const;

		/*Container properties*/
		bool isContainer();
		bool isSortingContainer();
		int sizeOfForestalling();
		int sizeOfChildrenForestalling();
		bool hasMovableChildren();
		bool minimizesToChildren();
		bool maximizesChildren();

		bool isPort();
		bool hasPin();

		QList<double> border();

		QStringList bonusContextMenuFields();
	private:
		qrRepo::RepoApi* mEditorRepoApi;
		Id mId;
		SdfRendererInterface* mRenderer;
		QList<Label*> mLabels;
		QDomDocument mGraphics;
	};
}

