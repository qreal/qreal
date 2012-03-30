#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../qrgui/editorPluginInterface/elementTitleHelpers.h"

	class Diagram : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/DiagramClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/DiagramPorts.sdf"));
			contents.setWidth(296);
			contents.setHeight(277);
			{
				StatPoint pt;
				pt.point = QPointF(0.00675676, 0.465704);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 296;
				pt.initHeight = 277;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.476351, 0.00722022);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 296;
				pt.initHeight = 277;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.993243, 0.505415);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 296;
				pt.initHeight = 277;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0.99278);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 296;
				pt.initHeight = 277;
				pointPorts << pt;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~Diagram() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() { return Qt::SolidLine; }

		int getPenWidth() { return 0; }

		QColor getPenColor() { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts()
		{
			return true;
		}

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return true;
		}

		bool isContainer()
		{
			return false;
		}

		bool isSortingContainer()
		{
			return false;
		}

		int sizeOfForestalling()
		{
			return 0;
		}

		int sizeOfChildrenForestalling()
		{
			return 0;
		}

		bool hasMovableChildren()
		{
			return true;
		}

		bool minimizesToChildren()
		{
			return false;
		}

		bool maximizesChildren()
		{
			return false;
		}

		bool isPort()
		{
			return false;
		}

		bool hasPin()
		{
			return false;
		}

		QList<double> border()
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}

		QStringList bonusContextMenuFields()
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
	};

	class Link : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~Link() {}

		ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() { return false; }
		bool isResizeable() { return true; }
		bool isContainer() { return false; }
		bool isSortingContainer() { return false; }
		int sizeOfForestalling() { return 0; }
		int sizeOfChildrenForestalling() { return 0; }
		bool hasMovableChildren() { return false; }
		bool minimizesToChildren() { return false; }
		bool maximizesChildren() { return false; }
		bool isPort() { return false; }
		bool hasPin() { return false; }
		QList<double> border()
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}
		bool hasPorts() { return false; }
		int getPenWidth() { return 1; }
		QColor getPenColor() { return QColor(0,0,0); }
		Qt::PenStyle getPenStyle() { return Qt::SolidLine; }
		QStringList bonusContextMenuFields()
		{
			return QStringList();
		}

	protected:
		virtual void drawStartArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(Qt::black);
			painter->setBrush(brush);
			static const QPointF points[] = {
				QPointF(0,0),
				QPointF(-5,10),
				QPointF(5,10)
			};
			painter->drawPolygon(points, 3);
			painter->setBrush(old);
		}

		virtual void drawEndArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(Qt::black);
			painter->setBrush(brush);
			static const QPointF points[] = {
				QPointF(0,0),
				QPointF(-5,10),
				QPointF(5,10)
			};
			painter->drawPolygon(points, 3);
			painter->setBrush(old);
		}

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

	private:
	};

	class Node : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/NodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/NodePorts.sdf"));
			contents.setWidth(195);
			contents.setHeight(136);
			{
				StatPoint pt;
				pt.point = QPointF(0.507692, 0.558824);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 195;
				pt.initHeight = 136;
				pointPorts << pt;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~Node() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() { return Qt::SolidLine; }

		int getPenWidth() { return 0; }

		QColor getPenColor() { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts()
		{
			return true;
		}

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return true;
		}

		bool isContainer()
		{
			return false;
		}

		bool isSortingContainer()
		{
			return false;
		}

		int sizeOfForestalling()
		{
			return 0;
		}

		int sizeOfChildrenForestalling()
		{
			return 0;
		}

		bool hasMovableChildren()
		{
			return true;
		}

		bool minimizesToChildren()
		{
			return false;
		}

		bool maximizesChildren()
		{
			return false;
		}

		bool isPort()
		{
			return false;
		}

		bool hasPin()
		{
			return false;
		}

		QList<double> border()
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}

		QStringList bonusContextMenuFields()
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
	};

