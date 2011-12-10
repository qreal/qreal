#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../../qrgui/editorPluginInterface/elementTitleHelpers.h"

	class AbstractActivityNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/AbstractActivityNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/AbstractActivityNodePorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~AbstractActivityNode() {}

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

	class Action : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ActionClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ActionPorts.sdf"));
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
			title_1 = factory.createTitle(0.0625, 0.05, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Action() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class ActivityFinalNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/ActivityFinalNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ActivityFinalNodePorts.sdf"));
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
		~ActivityFinalNode() {}

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

	class ActualParameter : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ActualParameterClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ActualParameterPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(30);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~ActualParameter() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class ControlFlow : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			title_1 = factory.createTitle(0, 0, "guard", false);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		virtual ~ControlFlow() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("guard")).replace("\n", "<br>"));
		}

	private:
		ElementTitleInterface *title_1;
	};

	class DecisionNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/DecisionNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/DecisionNodePorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatPoint pt;
				pt.point = QPointF(0, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(1, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 1);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~DecisionNode() {}

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

	class FormalParameter : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FormalParameterClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/FormalParameterPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(20);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~FormalParameter() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class FormalParameters : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FormalParametersClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/FormalParametersPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 1, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0, 1, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 1, 0, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 1, 0, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~FormalParameters() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
			return true;
		}

		bool isSortingContainer()
		{
			return true;
		}

		int sizeOfForestalling()
		{
			return 25;
		}

		int sizeOfChildrenForestalling()
		{
			return 2;
		}

		bool hasMovableChildren()
		{
			return false;
		}

		bool minimizesToChildren()
		{
			return true;
		}

		bool maximizesChildren()
		{
			return true;
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
		ElementTitleInterface *title_1;
	};

	class FunctionCall : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FunctionCallClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/FunctionCallPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 1, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0, 1, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 1, 0, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 1, 0, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~FunctionCall() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
			return true;
		}

		bool isSortingContainer()
		{
			return true;
		}

		int sizeOfForestalling()
		{
			return 25;
		}

		int sizeOfChildrenForestalling()
		{
			return 2;
		}

		bool hasMovableChildren()
		{
			return false;
		}

		bool minimizesToChildren()
		{
			return true;
		}

		bool maximizesChildren()
		{
			return true;
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
		ElementTitleInterface *title_1;
	};

	class HandlerStart : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/HandlerStartClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/HandlerStartPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(40);
			{
				StatPoint pt;
				pt.point = QPointF(0.05, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(1, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 1);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 200;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			title_1 = factory.createTitle(0.075, 0.075, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~HandlerStart() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class InitialNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/InitialNodeClass.sdf"));
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

	class MergeNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/MergeNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MergeNodePorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatPoint pt;
				pt.point = QPointF(0, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(1, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 1);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 50;
				pt.initHeight = 50;
				pointPorts << pt;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~MergeNode() {}

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

	class ReturnAction : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ReturnActionClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ReturnActionPorts.sdf"));
			contents.setWidth(100);
			contents.setHeight(40);
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 100;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 1);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 100;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 100;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(1, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 100;
				pt.initHeight = 40;
				pointPorts << pt;
			};
			title_1 = factory.createTitle(0, 0, QString::fromUtf8("<b>return<b>"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.4, 0, "name", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~ReturnAction() {}

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
			title_2->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
		ElementTitleInterface *title_2;
	};

	class ReturnValue : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ReturnValueClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ReturnValuePorts.sdf"));
			contents.setWidth(100);
			contents.setHeight(40);
			title_1 = factory.createTitle(0.2, 0.075, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~ReturnValue() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class UbiqActivityComment : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/UbiqActivityCommentClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/UbiqActivityCommentPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(100);
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 0, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 1, 1, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 1, 1, 0.2);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0, 0, "body", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~UbiqActivityComment() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("body")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class UbiqActivityCommentLink : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~UbiqActivityCommentLink() {}

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
		int getPenWidth() { return 0; }
		QColor getPenColor() { return QColor(0,0,0); }
		Qt::PenStyle getPenStyle() { return Qt::DashLine; }
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
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		virtual void drawEndArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

	private:
	};

	class UbiqActivityDiagram : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/UbiqActivityDiagramClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/UbiqActivityDiagramPorts.sdf"));
			contents.setWidth(48);
			contents.setHeight(48);
		}

		 ElementImpl *clone() { return NULL; }
		~UbiqActivityDiagram() {}

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
			return true;
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

	class UbiqPackage : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/UbiqPackageClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/UbiqPackagePorts.sdf"));
			contents.setWidth(48);
			contents.setHeight(48);
		}

		 ElementImpl *clone() { return NULL; }
		~UbiqPackage() {}

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
			return true;
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

	class Comment : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/CommentClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/CommentPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(100);
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 0, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 1, 1, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 1, 1, 0.2);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0, 0, "body", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Comment() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("body")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class CommentLink : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~CommentLink() {}

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
		int getPenWidth() { return 0; }
		QColor getPenColor() { return QColor(0,0,0); }
		Qt::PenStyle getPenStyle() { return Qt::DashLine; }
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
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		virtual void drawEndArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

	private:
	};

	class CustomClass : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/CustomClassClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/CustomClassPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(50);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~CustomClass() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
			return true;
		}

		bool isSortingContainer()
		{
			return true;
		}

		int sizeOfForestalling()
		{
			return 5;
		}

		int sizeOfChildrenForestalling()
		{
			return 2;
		}

		bool hasMovableChildren()
		{
			return false;
		}

		bool minimizesToChildren()
		{
			return true;
		}

		bool maximizesChildren()
		{
			return true;
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
		ElementTitleInterface *title_1;
	};

	class DataStructuresDiagram : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/DataStructuresDiagramClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/DataStructuresDiagramPorts.sdf"));
			contents.setWidth(48);
			contents.setHeight(48);
		}

		 ElementImpl *clone() { return NULL; }
		~DataStructuresDiagram() {}

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
			return true;
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

	class EnumElement : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EnumElementClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EnumElementPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(20);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~EnumElement() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class ErrorCodes : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ErrorCodesClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ErrorCodesPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(50);
			title_1 = factory.createTitle(0, 0, QString::fromUtf8("<b>Error Codes</b>"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~ErrorCodes() {}

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
			return true;
		}

		bool isSortingContainer()
		{
			return true;
		}

		int sizeOfForestalling()
		{
			return 5;
		}

		int sizeOfChildrenForestalling()
		{
			return 2;
		}

		bool hasMovableChildren()
		{
			return false;
		}

		bool minimizesToChildren()
		{
			return true;
		}

		bool maximizesChildren()
		{
			return true;
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
		ElementTitleInterface *title_1;
	};

	class Field : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FieldClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/FieldPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(20);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Field() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class MessageClass : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/MessageClassClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MessageClassPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(50);
			title_1 = factory.createTitle(0, 0, QString::fromUtf8("<b>Message</b>"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~MessageClass() {}

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
			return true;
		}

		bool isSortingContainer()
		{
			return true;
		}

		int sizeOfForestalling()
		{
			return 5;
		}

		int sizeOfChildrenForestalling()
		{
			return 2;
		}

		bool hasMovableChildren()
		{
			return false;
		}

		bool minimizesToChildren()
		{
			return true;
		}

		bool maximizesChildren()
		{
			return true;
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
		ElementTitleInterface *title_1;
	};

	class MessageCodes : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/MessageCodesClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MessageCodesPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(50);
			title_1 = factory.createTitle(0, 0, QString::fromUtf8("<b>Message Codes</b>"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~MessageCodes() {}

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
			return true;
		}

		bool isSortingContainer()
		{
			return true;
		}

		int sizeOfForestalling()
		{
			return 5;
		}

		int sizeOfChildrenForestalling()
		{
			return 2;
		}

		bool hasMovableChildren()
		{
			return false;
		}

		bool minimizesToChildren()
		{
			return true;
		}

		bool maximizesChildren()
		{
			return true;
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
		ElementTitleInterface *title_1;
	};

	class Handler : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/HandlerClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/HandlerPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(20);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Handler() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class MasterDiagramComment : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/MasterDiagramCommentClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MasterDiagramCommentPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(100);
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 0, 0, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0, 1, 1, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 1, 1, 0.2);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0, 0, "body", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~MasterDiagramComment() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("body")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class MasterDiagramCommentLink : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~MasterDiagramCommentLink() {}

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
		int getPenWidth() { return 0; }
		QColor getPenColor() { return QColor(0,0,0); }
		Qt::PenStyle getPenStyle() { return Qt::DashLine; }
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
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		virtual void drawEndArrow(QPainter * painter) const
		{
			QBrush old = painter->brush();
			QBrush brush;
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			painter->setBrush(old);
		}

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

	private:
	};

	class MasterDiagramConstant : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/MasterDiagramConstantClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MasterDiagramConstantPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(20);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~MasterDiagramConstant() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class MasterDiagramField : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/MasterDiagramFieldClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MasterDiagramFieldPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(20);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~MasterDiagramField() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class MasterNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/MasterNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MasterNodePorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(50);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~MasterNode() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
			return true;
		}

		bool isSortingContainer()
		{
			return true;
		}

		int sizeOfForestalling()
		{
			return 5;
		}

		int sizeOfChildrenForestalling()
		{
			return 2;
		}

		bool hasMovableChildren()
		{
			return false;
		}

		bool minimizesToChildren()
		{
			return true;
		}

		bool maximizesChildren()
		{
			return true;
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
		ElementTitleInterface *title_1;
	};

	class Preprocessor : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/PreprocessorClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/PreprocessorPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(20);
			title_1 = factory.createTitle(0, 0, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Preprocessor() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("name")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class UbiqMasterDiagram : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/UbiqMasterDiagramClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/UbiqMasterDiagramPorts.sdf"));
			contents.setWidth(48);
			contents.setHeight(48);
		}

		 ElementImpl *clone() { return NULL; }
		~UbiqMasterDiagram() {}

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
			return true;
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

