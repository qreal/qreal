#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QList>
#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtGui/QPainter>
#include "elementTitleHelpers.h"
#include "elementRepoInterface.h"
#include "sdfRendererInterface.h"
#include "../../qrkernel/ids.h"

typedef QPair<QPair<qReal::Id, qReal::Id>, QPair<bool, qReal::Id> > PossibleEdge;
typedef QPair<QPair<QString, QString>, QPair<bool, QString> > StringPossibleEdge;
typedef QPair<bool, qReal::Id> PossibleEdgeType;

/** @brief point port description */
struct StatPoint
{
	QPointF point;

	/// Denote is the port need to be scaled on X dimension or not.
	/// 'true' - means that the port isn't need to be scaled.
	/// 'false' - means that the port is need to be scaled.
	bool prop_x;

	/// Denote is the port need to be scaled on Y dimension or not.
	/// 'true' - means that the port isn't need to be scaled.
	/// 'false' - means that the port is need to be scaled.
	bool prop_y;

	int initWidth;
	int initHeight;

	StatPoint() : point(QPointF(0, 0)), prop_x(false), prop_y(false),
		initWidth(1), initHeight(1) {}

	operator QPointF () const
	{
		return point;
	}

	void operator = (QPointF const &p)
	{
		point = p;
		prop_x = false;
		prop_y = false;
		initHeight = 1;
		initWidth = 1;
	}
};

/** @brief line port description */
struct StatLine
{
	QLineF line;
	bool prop_x1;
	bool prop_y1;
	bool prop_x2;
	bool prop_y2;
	int initWidth;
	int initHeight;

	StatLine() : line(QLineF(0, 0, 0, 0)), prop_x1(false), prop_y1(false),
		prop_x2(false), prop_y2(false), initWidth(1), initHeight(1) {}

	operator QLineF () const
	{
		return line;
	}


	void operator = (QLineF const &l)
	{
		line = l;
		prop_x1 = false;
		prop_x2 = false;
		prop_y1 = false;
		prop_y2 = false;
		initHeight = 1;
		initWidth = 1;
	}
};

/** @class ElementImpl
  *	@brief base class for generated stuff in plugins
  *	TODO: split into NodeElementImpl and EdgeElementImpl
  * */
class ElementImpl {
public:
	virtual ~ElementImpl() {}
	virtual void init(QRectF &contents, QList<StatPoint> &pointPorts
					  , QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory
					  , QList<ElementTitleInterface*> &title
					  , SdfRendererInterface *renderer, SdfRendererInterface *portRenderer
					  , ElementRepoInterface *elementRepo = 0) = 0;
	virtual void init(ElementTitleFactoryInterface &factory,
					  QList<ElementTitleInterface*> &titles) = 0;
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
	virtual int sizeOfForestalling() const = 0;
	virtual int sizeOfChildrenForestalling() const = 0;
	virtual bool hasMovableChildren() const = 0;
	virtual bool minimizesToChildren() const = 0;
	virtual bool maximizesChildren() const = 0;

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
