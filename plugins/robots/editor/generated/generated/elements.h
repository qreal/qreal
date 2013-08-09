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
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/AbstractNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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

	class Balance : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/BalanceClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/BalancePorts.sdf"));
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
			title_1 = factory.createTitle(0.66, 1.2, "port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-1.14, 1.2, QString::fromUtf8("Порт гироскопа:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "forward", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1.3, 1.6, QString::fromUtf8("Значение forward:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 2, "turn", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-0.94, 2, QString::fromUtf8("Значение turn:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createTitle(0.66, 2.4, "gyroOffset", false);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setFlags(0);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createTitle(-2.16, 2.4, QString::fromUtf8("Значение для калибровки:"));
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setFlags(0);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
			title_9 = factory.createTitle(0.66, 2.8, "port1", false);
			title_9->setBackground(Qt::transparent);
			title_9->setScaling(false, false);
			title_9->setHard(false);
			title_9->setFlags(0);
			title_9->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_9);
			title_10 = factory.createTitle(-1.68, 2.8, QString::fromUtf8("Порт первого мотора:"));
			title_10->setBackground(Qt::transparent);
			title_10->setScaling(false, false);
			title_10->setHard(false);
			title_10->setFlags(0);
			title_10->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_10);
			title_11 = factory.createTitle(0.66, 3.2, "port2", false);
			title_11->setBackground(Qt::transparent);
			title_11->setScaling(false, false);
			title_11->setHard(false);
			title_11->setFlags(0);
			title_11->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_11);
			title_12 = factory.createTitle(-1.68, 3.2, QString::fromUtf8("Порт второго мотора:"));
			title_12->setBackground(Qt::transparent);
			title_12->setScaling(false, false);
			title_12->setHard(false);
			title_12->setFlags(0);
			title_12->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_12);
			title_13 = factory.createTitle(0.66, 3.6, "pwm1", false);
			title_13->setBackground(Qt::transparent);
			title_13->setScaling(false, false);
			title_13->setHard(false);
			title_13->setFlags(0);
			title_13->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_13);
			title_14 = factory.createTitle(-4.26, 3.6, QString::fromUtf8("Выходное значение мощности первого мотора:"));
			title_14->setBackground(Qt::transparent);
			title_14->setScaling(false, false);
			title_14->setHard(false);
			title_14->setFlags(0);
			title_14->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_14);
			title_15 = factory.createTitle(0.66, 4, "pwm2", false);
			title_15->setBackground(Qt::transparent);
			title_15->setScaling(false, false);
			title_15->setHard(false);
			title_15->setFlags(0);
			title_15->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_15);
			title_16 = factory.createTitle(-4.26, 4, QString::fromUtf8("Выходное значение мощности второго мотора:"));
			title_16->setBackground(Qt::transparent);
			title_16->setScaling(false, false);
			title_16->setHard(false);
			title_16->setFlags(0);
			title_16->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_16);
		}

		 ElementImpl *clone() { return NULL; }
		~Balance() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("forward")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("turn")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_7->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("gyroOffset")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_9->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("port1")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_11->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("port2")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_13->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("pwm1")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_15->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("pwm2")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
		ElementTitleInterface *title_7;
		ElementTitleInterface *title_8;
		ElementTitleInterface *title_9;
		ElementTitleInterface *title_10;
		ElementTitleInterface *title_11;
		ElementTitleInterface *title_12;
		ElementTitleInterface *title_13;
		ElementTitleInterface *title_14;
		ElementTitleInterface *title_15;
		ElementTitleInterface *title_16;
	};

	class BalanceInit : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/BalanceInitClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/BalanceInitPorts.sdf"));
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
		~BalanceInit() {}

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
			return false;
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

	class Beep : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/BeepClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(-0.7, -0.5, QString::fromUtf8("Громкость (%):"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-1.2, 1.2, QString::fromUtf8("Ждать завершения:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.7, -0.5, "Volume", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(true);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0.6, 1.2, "WaitForCompletion", false);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 ElementImpl *clone() { return NULL; }
		~Beep() {}

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
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Volume")).replace("\n", "<br>"));
			title_4->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("WaitForCompletion")).replace("\n", "<br>"));
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class ClearScreen : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ClearScreenClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/ClearScreenPorts.sdf"));
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
		~ClearScreen() {}

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
			return false;
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

	class CommentBlock : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/CommentBlockClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/CommentBlockPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(100);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.05, 0, 0.95);
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
				ln.line = QLineF(0.025, 0, 0.875, 0);
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
				ln.line = QLineF(1, 0.25, 1, 0.95);
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
				ln.line = QLineF(0.025, 1, 0.975, 1);
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
				ln.line = QLineF(0.9, 0, 1, 0.2);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 100;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.1, 0.2, "Comment", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~CommentBlock() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Comment")).replace("\n", "<br>"));
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
			title_1 = factory.createTitle(0, 0, "Guard", false);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Guard")).replace("\n", "<br>"));
		}

	private:
		ElementTitleInterface *title_1;
	};

	class DrawCircle : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/DrawCircleClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/DrawCirclePorts.sdf"));
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
			title_1 = factory.createTitle(0.72, 1.2, "XCoordinateCircle", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("X: "));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.72, 1.6, "YCoordinateCircle", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.6, QString::fromUtf8("Y: "));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.72, 2, "CircleRadius", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(0, 2, QString::fromUtf8("Радиус: "));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~DrawCircle() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("XCoordinateCircle")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("YCoordinateCircle")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("CircleRadius")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class DrawLine : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/DrawLineClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/DrawLinePorts.sdf"));
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
			title_1 = factory.createTitle(0.72, 1.2, "X1CoordinateLine", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("X1: "));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.72, 1.6, "Y1CoordinateLine", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.6, QString::fromUtf8("Y1: "));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.72, 2, "X2CoordinateLine", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(0, 2, QString::fromUtf8("X2: "));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createTitle(0.72, 2.4, "Y2CoordinateLine", false);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setFlags(0);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createTitle(0, 2.4, QString::fromUtf8("Y2: "));
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setFlags(0);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 ElementImpl *clone() { return NULL; }
		~DrawLine() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("X1CoordinateLine")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Y1CoordinateLine")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("X2CoordinateLine")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_7->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Y2CoordinateLine")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
		ElementTitleInterface *title_7;
		ElementTitleInterface *title_8;
	};

	class DrawPixel : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/DrawPixelClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/DrawPixelPorts.sdf"));
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
			title_1 = factory.createTitle(0.72, 1.2, "XCoordinatePix", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("X: "));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.72, 1.6, "YCoordinatePix", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.6, QString::fromUtf8("Y: "));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 ElementImpl *clone() { return NULL; }
		~DrawPixel() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("XCoordinatePix")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("YCoordinatePix")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class DrawRect : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/DrawRectClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/DrawRectPorts.sdf"));
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
			title_1 = factory.createTitle(0.8, 1.2, "XCoordinateRect", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("X: "));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.8, 1.6, "YCoordinateRect", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.6, QString::fromUtf8("Y: "));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.8, 2, "WidthRect", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(0, 2, QString::fromUtf8("Ширина: "));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createTitle(0.8, 2.4, "HeightRect", false);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setFlags(0);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createTitle(0, 2.4, QString::fromUtf8("Высота: "));
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setFlags(0);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 ElementImpl *clone() { return NULL; }
		~DrawRect() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("XCoordinateRect")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("YCoordinateRect")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("WidthRect")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_7->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("HeightRect")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
		ElementTitleInterface *title_7;
		ElementTitleInterface *title_8;
	};

	class EngineCommand : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EngineCommandClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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

	class EngineMovementCommand : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EngineMovementCommandClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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

	class EnginesBackward : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EnginesBackwardClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Ports", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.16, -0.5, QString::fromUtf8("Порты:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Power", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1, 1.2, QString::fromUtf8("Мощность (%):"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 ElementImpl *clone() { return NULL; }
		~EnginesBackward() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Ports")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Power")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class EnginesForward : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EnginesForwardClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Ports", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.16, -0.5, QString::fromUtf8("Порты:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Power", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1, 1.2, QString::fromUtf8("Мощность (%):"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 ElementImpl *clone() { return NULL; }
		~EnginesForward() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Ports")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Power")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class EnginesStop : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EnginesStopClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Ports", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.1, -0.5, QString::fromUtf8("Порты:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Ports")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
	};

	class FinalNode : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FinalNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			return false;
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

	class Fork : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ForkClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			return false;
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

	class Function : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/FunctionClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.3, 1.2, "Body", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
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
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Body")).replace("\n", "<br>"));
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
	};

	class IfBlock : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/IfBlockClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.5, 1.2, QString::fromUtf8("Условие:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
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
			title_1->setPlainText(repo->logicalProperty("Condition"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
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
			Q_UNUSED(pointPorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/InitialNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			return false;
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

	class Loop : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/LoopClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Iterations", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.74, -0.5, QString::fromUtf8("Итераций:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Iterations")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
	};

	class NullificationEncoder : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/NullificationEncoderClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Ports", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порты:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Ports")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
	};

	class PlayTone : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/PlayToneClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(-0.7, -0.5, QString::fromUtf8("Частота (Гц):"));
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.7, 1.2, QString::fromUtf8("Громкость (%):"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(-1.36, 1.6, QString::fromUtf8("Длительность (мс):"));
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1.36, 2, QString::fromUtf8("Ждать завершения:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.8, -0.5, "Frequency", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(true);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(0.9, 1.2, "Volume", false);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createTitle(0.9, 1.6, "Duration", false);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setFlags(0);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createTitle(0.9, 2, "WaitForCompletion", false);
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setFlags(0);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 ElementImpl *clone() { return NULL; }
		~PlayTone() {}

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
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Frequency")).replace("\n", "<br>"));
			title_6->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Volume")).replace("\n", "<br>"));
			title_7->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Duration")).replace("\n", "<br>"));
			title_8->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("WaitForCompletion")).replace("\n", "<br>"));
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
		ElementTitleInterface *title_7;
		ElementTitleInterface *title_8;
	};

	class PrintText : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/PrintTextClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/PrintTextPorts.sdf"));
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
			title_1 = factory.createTitle(0.72, 1.2, "XCoordinateText", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("X: "));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.72, 1.6, "YCoordinateText", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.6, QString::fromUtf8("Y: "));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.72, 2, "PrintText", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(0, 2, QString::fromUtf8("Текст: "));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~PrintText() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("XCoordinateText")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("YCoordinateText")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("PrintText")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(linePorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/RobotsDiagramNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1->setHard(false);
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

	class SensorBlock : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<StatPoint> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			Q_UNUSED(linePorts);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/SensorBlockClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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

	class SubprogramRobots : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/SubprogramRobotsClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/SubprogramRobotsPorts.sdf"));
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
			title_1 = factory.createTitle(0.1, 1.2, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~SubprogramRobots() {}

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
			return false;
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

	class Timer : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/TimerClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(1, -0.5, "Delay", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-0.6, -0.5, QString::fromUtf8("Задержка (мс):"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Delay")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
	};

	class VariableInit : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/VariableInitClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/VariableInitPorts.sdf"));
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
			title_1 = factory.createTitle(0.66, 1.2, "variable", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(-1.2, 1.2, QString::fromUtf8("Переменная:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.6, "value", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1.3, 1.6, QString::fromUtf8("Значение:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 ElementImpl *clone() { return NULL; }
		~VariableInit() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("variable")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("value")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class WaitForAccelerometer : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForAccelerometerClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/WaitForAccelerometerPorts.sdf"));
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Acceleration", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-0.52, 1.2, QString::fromUtf8("Ускорение:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 1.6, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 1.6, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createTitle(0.66, 2, "AccelerationAxis", false);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setFlags(0);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createTitle(-1.6, 2, QString::fromUtf8("Ускорение по оси:"));
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setFlags(0);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForAccelerometer() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Acceleration")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_7->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("AccelerationAxis")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
		ElementTitleInterface *title_7;
		ElementTitleInterface *title_8;
	};

	class WaitForButtons : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForButtonsClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/WaitForButtonsPorts.sdf"));
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
			title_1 = factory.createTitle(1.2, 1.2, "RightButtonClicks", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, 1.2, QString::fromUtf8("Правая: "));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(1.2, 1.6, "LeftButtonClicks", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.6, QString::fromUtf8("Левая: "));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(1.2, 2, "CentralButtonClicks", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(0, 2, QString::fromUtf8("Центральная: "));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createTitle(1.2, 2.4, "BottomButtonClicks", false);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setFlags(0);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createTitle(0, 2.4, QString::fromUtf8("Нижняя: "));
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setFlags(0);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForButtons() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("RightButtonClicks")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("LeftButtonClicks")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("CentralButtonClicks")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_7->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("BottomButtonClicks")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
		ElementTitleInterface *title_7;
		ElementTitleInterface *title_8;
	};

	class WaitForColor : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForColorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Color", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(0, 1.2, QString::fromUtf8("Цвет:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Color")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForColorIntensityClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Intensity", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1, 1.2, QString::fromUtf8("Интенсивность:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 1.6, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 1.6, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Intensity")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForEncoderClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "TachoLimit", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1.2, 1.2, QString::fromUtf8("Предел оборотов:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("TachoLimit")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
	};

	class WaitForGyroscope : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForGyroscopeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/WaitForGyroscopePorts.sdf"));
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Degrees", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-0.52, 1.2, QString::fromUtf8("Градусы:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 1.6, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 1.6, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForGyroscope() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Degrees")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class WaitForLight : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForLightClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/WaitForLightPorts.sdf"));
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Percents", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-0.52, 1.2, QString::fromUtf8("Проценты:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 1.6, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 1.6, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForLight() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Percents")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class WaitForSonarDistance : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForSonarDistanceClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Distance", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-1.1, 1.2, QString::fromUtf8("Расстояние (см):"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 1.6, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 1.6, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Distance")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
		ElementTitleInterface *title_3;
		ElementTitleInterface *title_4;
		ElementTitleInterface *title_5;
		ElementTitleInterface *title_6;
	};

	class WaitForSound : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/WaitForSoundClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			portRenderer->load(QString(":/generated/shapes/WaitForSoundPorts.sdf"));
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createTitle(0.66, 1.2, "Volume", false);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setFlags(0);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createTitle(-0.52, 1.2, QString::fromUtf8("Громкость:"));
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setFlags(0);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createTitle(0.66, 1.6, "Sign", false);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setFlags(0);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createTitle(-1.6, 1.6, QString::fromUtf8("Считанное значение:"));
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setFlags(0);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 ElementImpl *clone() { return NULL; }
		~WaitForSound() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_3->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Volume")).replace("\n", "<br>"));
			Q_UNUSED(repo);
			title_5->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Sign")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
							SdfRendererInterface *portRenderer, ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(pointPorts);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForTouchSensorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
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
			title_1 = factory.createTitle(0.66, -0.5, "Port", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0, -0.5, QString::fromUtf8("Порт:"));
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("Port")).replace("\n", "<br>"));
			Q_UNUSED(repo);
		}

		bool isNode() const
		{
			return true;
		}

		bool isResizeable() const
		{
			return false;
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
		ElementTitleInterface *title_2;
	};

