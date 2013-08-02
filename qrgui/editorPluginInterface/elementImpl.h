#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QList>

#include "labelHelpers.h"
#include "elementRepoInterface.h"
#include "sdfRendererInterface.h"
#include "../../qrkernel/ids.h"
#include "portImpl.h"

typedef QPair<QPair<qReal::Id, qReal::Id>, QPair<bool, qReal::Id> > PossibleEdge;
typedef QPair<QPair<QString, QString>, QPair<bool, QString> > StringPossibleEdge;
typedef QPair<bool, qReal::Id> PossibleEdgeType;

/** @class ElementImpl
  *	@brief base class for generated stuff in plugins
  *	TODO: split into NodeElementImpl and EdgeElementImpl
  * */
class ElementImpl {
public:
	virtual ~ElementImpl() {}
	virtual void init(QRectF &contents, QList<StatPoint> &pointPorts
					  , QList<StatLine> &linePorts, LabelFactoryInterface &factory
					  , QList<LabelInterface*> &title
					  , SdfRendererInterface *renderer, SdfRendererInterface *portRenderer
					  , ElementRepoInterface *elementRepo = 0) = 0;
	virtual void init(LabelFactoryInterface &factory,
					  QList<LabelInterface*> &titles) = 0;
	virtual void paint(QPainter *painter, QRectF &contents) = 0;
	virtual void updateData(ElementRepoInterface *repo) const = 0;
	virtual bool isNode() const = 0;
	virtual bool hasPorts() const = 0;
	virtual bool isResizeable() const = 0;
	virtual Qt::PenStyle getPenStyle() const = 0;
	virtual int getPenWidth() const = 0; //
	virtual QColor getPenColor() const = 0;
	virtual void drawStartArrow(QPainter *painter) const = 0;
	virtual void drawEndArrow(QPainter *painter) const = 0;
	virtual bool isDividable() const = 0;

	/*Container properties*/
	virtual bool isContainer() const = 0;
	virtual bool isSortingContainer() const = 0;
	virtual QVector<int> sizeOfForestalling() const = 0;
	virtual int sizeOfChildrenForestalling() const = 0;
	virtual bool hasMovableChildren() const = 0;
	virtual bool minimizesToChildren() const = 0;
	virtual bool maximizesChildren() const = 0;

	virtual QStringList fromPortTypes() const = 0;
	virtual QStringList toPortTypes() const = 0;

	virtual bool isPort() const = 0;
	virtual bool hasPin() const = 0;

	virtual QList<double> border() const = 0;

	virtual QStringList bonusContextMenuFields() const = 0;

	/// Update shape of an element. Does nothing in case of generated editors, used by metamodel interpreter.
	virtual void updateRendererContent(QString const &shape)
	{
		Q_UNUSED(shape);
	}
};
