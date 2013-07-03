#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../../qrgui/editorPluginInterface/elementTitleHelpers.h"

	class ActionElement : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ActionElementClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/ActionElementPorts.sdf"));
			contents.setWidth(80);
			contents.setHeight(60);
			{
				StatLine ln;
				ln.line = QLineF(0.9875, 0.166667, 0.9875, 0.833333);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 80;
				ln.initHeight = 60;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.0125, 0.166667, 0.0125, 0.833333);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 80;
				ln.initHeight = 60;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.125, 0.0166667, 0.875, 0.0166667);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 80;
				ln.initHeight = 60;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.125, 0.983333, 0.875, 0.983333);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 80;
				ln.initHeight = 60;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0, 0, "process", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~ActionElement() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }

		int getPenWidth() const { return 0; }

		QColor getPenColor() const { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts() const
		{
			return true;
		}

		void updateData(ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setHtml(QString("<center>%1</center>").arg(repo->logicalProperty("process")).replace("\n", "<br>"));
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return true;
		}

		bool isContainer() const
		{
			return false;
		}

		bool isSortingContainer() const
		{
			return false;
		}

		int sizeOfForestalling() const
		{
			return 0;
		}

		int sizeOfChildrenForestalling() const
		{
			return 0;
		}

		bool hasMovableChildren() const
		{
			return true;
		}

		bool minimizesToChildren() const
		{
			return false;
		}

		bool maximizesChildren() const
		{
			return false;
		}

		bool isDividable() const
		{
			return false;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}

		QStringList bonusContextMenuFields() const
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
		ElementTitleInterface *title_1;
	};

	class BlockDiagramNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/BlockDiagramNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/BlockDiagramNodePorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(200);
			{
				StatPoint pt;
				pt.point = QPointF(0, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 200;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 200;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(1, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 200;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 1);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 200;
				pointPorts << pt;
			};
			title_1 = factory.createTitle(0.2, 0.05, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~BlockDiagramNode() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }

		int getPenWidth() const { return 0; }

		QColor getPenColor() const { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts() const
		{
			return true;
		}

		void updateData(ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setHtml(QString("<b>%1</b>").arg(repo->name()).replace("\n", "<br>"));
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return true;
		}

		bool isContainer() const
		{
			return true;
		}

		bool isSortingContainer() const
		{
			return false;
		}

		int sizeOfForestalling() const
		{
			return 0;
		}

		int sizeOfChildrenForestalling() const
		{
			return 0;
		}

		bool hasMovableChildren() const
		{
			return true;
		}

		bool minimizesToChildren() const
		{
			return false;
		}

		bool maximizesChildren() const
		{
			return false;
		}

		bool isDividable() const
		{
			return false;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}

		QStringList bonusContextMenuFields() const
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
		ElementTitleInterface *title_1;
	};

	class BlockFinalNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/BlockFinalNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/BlockFinalNodePorts.sdf"));
			contents.setWidth(30);
			contents.setHeight(30);
			{
				StatPoint pt;
				pt.point = QPointF(0, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 1);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(1, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~BlockFinalNode() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }

		int getPenWidth() const { return 0; }

		QColor getPenColor() const { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts() const
		{
			return true;
		}

		void updateData(ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return true;
		}

		bool isContainer() const
		{
			return false;
		}

		bool isSortingContainer() const
		{
			return false;
		}

		int sizeOfForestalling() const
		{
			return 0;
		}

		int sizeOfChildrenForestalling() const
		{
			return 0;
		}

		bool hasMovableChildren() const
		{
			return true;
		}

		bool minimizesToChildren() const
		{
			return false;
		}

		bool maximizesChildren() const
		{
			return false;
		}

		bool isDividable() const
		{
			return false;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}

		QStringList bonusContextMenuFields() const
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
	};

	class ConditionControlFlow : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *, ElementRepoInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			title_1 = factory.createTitle(0, 0, "type", false);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		virtual ~ConditionControlFlow() {}

		ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
		bool isSortingContainer() const { return false; }
		int sizeOfForestalling() const { return 0; }
		int sizeOfChildrenForestalling() const { return 0; }
		bool hasMovableChildren() const { return false; }
		bool minimizesToChildren() const { return false; }
		bool maximizesChildren() const { return false; }
		bool isPort() const { return false; }
		bool hasPin() const { return false; }
		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}
		bool hasPorts() const { return false; }
		int getPenWidth() const { return 1; }
		QColor getPenColor() const { return QColor(0,0,0); }
		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }
		QStringList bonusContextMenuFields() const
		{
			return QStringList();
		}

	protected:
		virtual void drawStartArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		virtual void drawEndArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			static const QPointF points[] = {
				QPointF(-5,10),
				QPointF(0,0),
				QPointF(5,10)
			};
			painter->drawPolyline(points, 3);
			painter->setBrush(old);
		}

		void updateData(ElementRepoInterface *repo) const
		{
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("type")).replace("\n", "<br>"));
		}

	private:
		ElementTitleInterface *title_1;
	};

	class ConditionNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ConditionNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/ConditionNodePorts.sdf"));
			contents.setWidth(60);
			contents.setHeight(60);
			{
				StatPoint pt;
				pt.point = QPointF(0.0166667, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 60;
				pt.initHeight = 60;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0.983333);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 60;
				pt.initHeight = 60;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.983333, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 60;
				pt.initHeight = 60;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0.0166667);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 60;
				pt.initHeight = 60;
				pointPorts << pt;
			};
			title_1 = factory.createTitle(0, 0, "condition", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~ConditionNode() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }

		int getPenWidth() const { return 0; }

		QColor getPenColor() const { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts() const
		{
			return true;
		}

		void updateData(ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setHtml(QString("<center>%1</center>").arg(repo->logicalProperty("condition")).replace("\n", "<br>"));
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return true;
		}

		bool isContainer() const
		{
			return false;
		}

		bool isSortingContainer() const
		{
			return false;
		}

		int sizeOfForestalling() const
		{
			return 0;
		}

		int sizeOfChildrenForestalling() const
		{
			return 0;
		}

		bool hasMovableChildren() const
		{
			return true;
		}

		bool minimizesToChildren() const
		{
			return false;
		}

		bool maximizesChildren() const
		{
			return false;
		}

		bool isDividable() const
		{
			return false;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}

		QStringList bonusContextMenuFields() const
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
		ElementTitleInterface *title_1;
	};

	class ControlFlow : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *, ElementRepoInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~ControlFlow() {}

		ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
		bool isSortingContainer() const { return false; }
		int sizeOfForestalling() const { return 0; }
		int sizeOfChildrenForestalling() const { return 0; }
		bool hasMovableChildren() const { return false; }
		bool minimizesToChildren() const { return false; }
		bool maximizesChildren() const { return false; }
		bool isPort() const { return false; }
		bool hasPin() const { return false; }
		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}
		bool hasPorts() const { return false; }
		int getPenWidth() const { return 1; }
		QColor getPenColor() const { return QColor(0,0,0); }
		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }
		QStringList bonusContextMenuFields() const
		{
			return QStringList();
		}

	protected:
		virtual void drawStartArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		virtual void drawEndArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			static const QPointF points[] = {
				QPointF(-5,10),
				QPointF(0,0),
				QPointF(5,10)
			};
			painter->drawPolyline(points, 3);
			painter->setBrush(old);
		}

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

	private:
	};

	class InitialNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/InitialNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/InitialNodePorts.sdf"));
			contents.setWidth(30);
			contents.setHeight(30);
			{
				StatPoint pt;
				pt.point = QPointF(0, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(1, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 1);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 30;
				pt.initHeight = 30;
				pointPorts << pt;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~InitialNode() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }

		int getPenWidth() const { return 0; }

		QColor getPenColor() const { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts() const
		{
			return true;
		}

		void updateData(ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return true;
		}

		bool isContainer() const
		{
			return false;
		}

		bool isSortingContainer() const
		{
			return false;
		}

		int sizeOfForestalling() const
		{
			return 0;
		}

		int sizeOfChildrenForestalling() const
		{
			return 0;
		}

		bool hasMovableChildren() const
		{
			return true;
		}

		bool minimizesToChildren() const
		{
			return false;
		}

		bool maximizesChildren() const
		{
			return false;
		}

		bool isDividable() const
		{
			return false;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}

		QStringList bonusContextMenuFields() const
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
	};

