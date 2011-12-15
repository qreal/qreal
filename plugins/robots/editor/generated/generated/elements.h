#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../../../qrgui/editorPluginInterface/elementTitleHelpers.h"

	class AbstractNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/AbstractNodePorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~AbstractNode() {}

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

	class Beep : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/BeepClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/BeepPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(-1.8, 1.2, QString::fromUtf8("Ждать завершения:"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.6, 1.2, "WaitForCompletion", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~Beep() {}

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
			title_2->setHtml(QString("%1").arg(repo->logicalProperty("WaitForCompletion")).replace("\n", "<br>"));
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

	class ControlFlow : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			title_1 = factory.createTitle(0, 0, "Guard", false);
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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Guard")).replace("\n", "<br>"));
		}

	private:
		ElementTitleInterface *title_1;
	};

	class EngineCommand : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EngineCommandClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EngineCommandPorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~EngineCommand() {}

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

	class EngineMovementCommand : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EngineMovementCommandClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EngineMovementCommandPorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~EngineMovementCommand() {}

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

	class EnginesBackward : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EnginesBackwardClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EnginesBackwardPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Ports", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.1, 1.2, QString::fromUtf8("Порты:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "Power", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1, 1.6, QString::fromUtf8("Мощность (%):"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 2, "TachoLimit", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-2, 2, QString::fromUtf8("Предел оборотов (град):"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~EnginesBackward() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Ports")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("Power")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("%1").arg(repo->logicalProperty("TachoLimit")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class EnginesForward : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EnginesForwardClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EnginesForwardPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Ports", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.1, 1.2, QString::fromUtf8("Порты:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "Power", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1, 1.6, QString::fromUtf8("Мощность (%):"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 2, "TachoLimit", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-2, 2, QString::fromUtf8("Предел оборотов (град):"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~EnginesForward() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Ports")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("Power")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("%1").arg(repo->logicalProperty("TachoLimit")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class EnginesStop : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EnginesStopClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EnginesStopPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Ports", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.1, 1.2, QString::fromUtf8("Порты:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~EnginesStop() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Ports")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

	class FinalNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FinalNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/FinalNodePorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~FinalNode() {}

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
			return false;
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

	class Fork : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ForkClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ForkPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
		}

		 ElementImpl *clone() { return NULL; }
		~Fork() {}

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
			return false;
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

	class Function : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/FunctionClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/FunctionPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(-0.74, 1.2, QString::fromUtf8("Функция:"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.3, 1.2, "Body", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~Function() {}

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
			title_2->setHtml(QString("%1").arg(repo->logicalProperty("Body")).replace("\n", "<br>"));
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

	class IfBlock : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/IfBlockClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/IfBlockPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Condition", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.5, 1.2, QString::fromUtf8("Условие:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~IfBlock() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Condition")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

	class InitialBlock : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/InitialBlockClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/InitialBlockPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "port_1", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.2, 1.2, QString::fromUtf8("Порт 1:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "port_2", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-0.2, 1.6, QString::fromUtf8("Порт 2:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 2, "port_3", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-0.2, 2, QString::fromUtf8("Порт 3:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createTitle(0.66, 2.4, "port_4", false);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setFlags(0);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createTitle(-0.2, 2.4, QString::fromUtf8("Порт 4:"));
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setFlags(0);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 ElementImpl *clone() { return NULL; }
		~InitialBlock() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("port_1")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("port_2")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("%1").arg(repo->logicalProperty("port_3")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_7->setHtml(QString("%1").arg(repo->logicalProperty("port_4")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
		ElementTitleInterface *title_7;
		ElementTitleInterface *title_8;
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
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/InitialNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/InitialNodePorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
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
			return false;
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

	class Loop : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/LoopClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/LoopPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Iterations", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.74, 1.2, QString::fromUtf8("Итераций:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~Loop() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Iterations")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

	class NullificationEncoder : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/NullificationEncoderClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/NullificationEncoderPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~NullificationEncoder() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

	class PlayTone : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/PlayToneClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/PlayTonePorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(-1.34, 1.2, QString::fromUtf8("Частота (Гц):"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-1.36, 1.6, QString::fromUtf8("Длительность (мс):"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.9, 1.22, "Frequency", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0.9, 1.68, "Duration", false);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(-1.36, 2, QString::fromUtf8("Ждать завершения:"));
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(0.9, 2, "WaitForCompletion", false);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~PlayTone() {}

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
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("Frequency")).replace("\n", "<br>"));
			title_4->setHtml(QString("%1").arg(repo->logicalProperty("Duration")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_6->setHtml(QString("%1").arg(repo->logicalProperty("WaitForCompletion")).replace("\n", "<br>"));
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class RobotsDiagramNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/RobotsDiagramNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/RobotsDiagramNodePorts.sdf"));
			contents.setWidth(204);
			contents.setHeight(204);
			{
				StatPoint pt;
				pt.point = QPointF(0.00980392, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 204;
				pt.initHeight = 204;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0.00980392);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 204;
				pt.initHeight = 204;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.990196, 0.5);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 204;
				pt.initHeight = 204;
				pointPorts << pt;
			};
			{
				StatPoint pt;
				pt.point = QPointF(0.5, 0.990196);
				pt.prop_x = false;
				pt.prop_y = false; 
				pt.initWidth = 204;
				pt.initHeight = 204;
				pointPorts << pt;
			};
			title_1 = factory.createTitle(0.205882, 0.0588235, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~RobotsDiagramNode() {}

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
			title_1->setHtml(QString("%1").arg(repo->name()).replace("\n", "<br>"));
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
		ElementTitleInterface *title_1;
	};

	class SensorBlock : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/SensorBlockClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/SensorBlockPorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~SensorBlock() {}

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

	class Timer : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/TimerClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/TimerPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Delay", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.94, 1.2, QString::fromUtf8("Задержка (мс):"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~Timer() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Delay")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

	class WaitForColor : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForColorClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/WaitForColorPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "Color", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.6, QString::fromUtf8("Цвет:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForColor() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("Color")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class WaitForColorIntensity : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForColorIntensityClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/WaitForColorIntensityPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "Intensity", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1, 1.6, QString::fromUtf8("Интенсивность:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 2, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 2, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForColorIntensity() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("Intensity")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("%1").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class WaitForEncoder : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForEncoderClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/WaitForEncoderPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "TachoLimit", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1.2, 1.6, QString::fromUtf8("Предел оборотов:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForEncoder() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("TachoLimit")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class WaitForSonarDistance : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForSonarDistanceClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/WaitForSonarDistancePorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "Distance", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1.1, 1.6, QString::fromUtf8("Расстояние (см):"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 2, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 2, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForSonarDistance() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("%1").arg(repo->logicalProperty("Distance")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("%1").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class WaitForTouchSensor : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForTouchSensorClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/WaitForTouchSensorPorts.sdf"));
			contents.setWidth(50);
			contents.setHeight(50);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.1, 0, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 0, 0.9, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(1, 0.1, 1, 0.9);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.1, 1, 0.9, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 50;
				ln.initHeight = 50;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.66, 1.2, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForTouchSensor() {}

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
			title_1->setHtml(QString("%1").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode()
		{
			return true;
		}

		bool isResizeable()
		{
			return false;
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

