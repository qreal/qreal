#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../../../qrgui/editorPluginInterface/labelFactoryInterface.h"
#include "../../../../../qrgui/editorPluginInterface/labelInterface.h"
#include "ports.h"

	class AbstractNode : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/AbstractNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class Balance : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/BalanceClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.66, 1.2, "port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -1.14, 1.2, QString::fromUtf8("Порт гироскопа:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.66, 1.6, "forward", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1.3, 1.6, QString::fromUtf8("Значение forward:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.66, 2, "turn", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -0.94, 2, QString::fromUtf8("Значение turn:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createLabel(7, 0.66, 2.4, "gyroOffset", false, 0);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createLabel(8, -2.16, 2.4, QString::fromUtf8("Значение для калибровки:"), 0);
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
			title_9 = factory.createLabel(9, 0.66, 2.8, "port1", false, 0);
			title_9->setBackground(Qt::transparent);
			title_9->setScaling(false, false);
			title_9->setHard(false);
			title_9->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_9);
			title_10 = factory.createLabel(10, -1.68, 2.8, QString::fromUtf8("Порт первого мотора:"), 0);
			title_10->setBackground(Qt::transparent);
			title_10->setScaling(false, false);
			title_10->setHard(false);
			title_10->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_10);
			title_11 = factory.createLabel(11, 0.66, 3.2, "port2", false, 0);
			title_11->setBackground(Qt::transparent);
			title_11->setScaling(false, false);
			title_11->setHard(false);
			title_11->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_11);
			title_12 = factory.createLabel(12, -1.68, 3.2, QString::fromUtf8("Порт второго мотора:"), 0);
			title_12->setBackground(Qt::transparent);
			title_12->setScaling(false, false);
			title_12->setHard(false);
			title_12->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_12);
			title_13 = factory.createLabel(13, 0.66, 3.6, "pwm1", false, 0);
			title_13->setBackground(Qt::transparent);
			title_13->setScaling(false, false);
			title_13->setHard(false);
			title_13->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_13);
			title_14 = factory.createLabel(14, -4.26, 3.6, QString::fromUtf8("Выходное значение мощности первого мотора:"), 0);
			title_14->setBackground(Qt::transparent);
			title_14->setScaling(false, false);
			title_14->setHard(false);
			title_14->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_14);
			title_15 = factory.createLabel(15, 0.66, 4, "pwm2", false, 0);
			title_15->setBackground(Qt::transparent);
			title_15->setScaling(false, false);
			title_15->setHard(false);
			title_15->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_15);
			title_16 = factory.createLabel(16, -4.26, 4, QString::fromUtf8("Выходное значение мощности второго мотора:"), 0);
			title_16->setBackground(Qt::transparent);
			title_16->setScaling(false, false);
			title_16->setHard(false);
			title_16->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_16);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("forward"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("turn"));
			Q_UNUSED(repo);
			title_7->setTextFromRepo(repo->logicalProperty("gyroOffset"));
			Q_UNUSED(repo);
			title_9->setTextFromRepo(repo->logicalProperty("port1"));
			Q_UNUSED(repo);
			title_11->setTextFromRepo(repo->logicalProperty("port2"));
			Q_UNUSED(repo);
			title_13->setTextFromRepo(repo->logicalProperty("pwm1"));
			Q_UNUSED(repo);
			title_15->setTextFromRepo(repo->logicalProperty("pwm2"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
		qReal::LabelInterface *title_7;
		qReal::LabelInterface *title_8;
		qReal::LabelInterface *title_9;
		qReal::LabelInterface *title_10;
		qReal::LabelInterface *title_11;
		qReal::LabelInterface *title_12;
		qReal::LabelInterface *title_13;
		qReal::LabelInterface *title_14;
		qReal::LabelInterface *title_15;
		qReal::LabelInterface *title_16;
	};

	class BalanceInit : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/BalanceInitClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class Beep : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/BeepClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, -0.7, -0.5, QString::fromUtf8("Громкость (%):"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -1.2, 1.2, QString::fromUtf8("Ждать завершения:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 1.2, -0.5, "Volume", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(true);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 1.4, 1.2, "WaitForCompletion", false, 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Volume"));
			title_4->setTextFromRepo(repo->logicalProperty("WaitForCompletion"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
	};

	class ClearScreen : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ClearScreenClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class CommentBlock : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/CommentBlockClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(200);
			contents.setHeight(100);
			ports << portFactory.createPort(QLineF(0, 0.05, 0, 0.95), false, false, false, false, 200, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.025, 0, 0.875, 0), false, false, false, false, 200, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.25, 1, 0.95), false, false, false, false, 200, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.025, 1, 0.975, 1), false, false, false, false, 200, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.9, 0, 1, 0.2), false, false, false, false, 200, 100, new NonTyped());
			title_1 = factory.createLabel(1, 0.1, 0.2, "Comment", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Comment"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
	};

	class ControlFlow : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			title_1 = factory.createLabel(1, 0, 0, "Guard", false, 0);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		virtual ~ControlFlow() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return true; }
		bool isSortingContainer() const { return false; }
		QVector<int> sizeOfForestalling() const { return QVector<int>(4, 0); }
		int sizeOfChildrenForestalling() const { return 0; }
		bool hasMovableChildren() const { return false; }
		bool minimizesToChildren() const { return false; }
		bool maximizesChildren() const { return false; }
		QStringList fromPortTypes() const
		{
			QStringList result;
			result << "NonTyped";
			return result;
		}
		QStringList toPortTypes() const
		{
			QStringList result;
			result << "NonTyped";
			return result;
		}
		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::broken;
		}
		bool isPort() const { return false; }
		bool hasPin() const { return false; }
		bool createChildrenFromMenu() const { return false; }
		QList<double> border() const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			title_1->setTextFromRepo(repo->logicalProperty("Guard"));
		}

	private:
		qReal::LabelInterface *title_1;
	};

	class DrawCircle : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/DrawCircleClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 1, 1.2, "XCoordinateCircle", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, 1.2, QString::fromUtf8("X: "), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 1, 1.6, "YCoordinateCircle", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0, 1.6, QString::fromUtf8("Y: "), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1, 2, "CircleRadius", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 0, 2, QString::fromUtf8("Радиус: "), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("XCoordinateCircle"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("YCoordinateCircle"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("CircleRadius"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class DrawLine : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/DrawLineClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.72, 1.2, "X1CoordinateLine", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, 1.2, QString::fromUtf8("X1: "), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.72, 1.6, "Y1CoordinateLine", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0, 1.6, QString::fromUtf8("Y1: "), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.72, 2, "X2CoordinateLine", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 0, 2, QString::fromUtf8("X2: "), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createLabel(7, 0.72, 2.4, "Y2CoordinateLine", false, 0);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createLabel(8, 0, 2.4, QString::fromUtf8("Y2: "), 0);
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("X1CoordinateLine"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Y1CoordinateLine"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("X2CoordinateLine"));
			Q_UNUSED(repo);
			title_7->setTextFromRepo(repo->logicalProperty("Y2CoordinateLine"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
		qReal::LabelInterface *title_7;
		qReal::LabelInterface *title_8;
	};

	class DrawPixel : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/DrawPixelClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.72, 1.2, "XCoordinatePix", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, 1.2, QString::fromUtf8("X: "), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.72, 1.6, "YCoordinatePix", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0, 1.6, QString::fromUtf8("Y: "), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("XCoordinatePix"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("YCoordinatePix"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
	};

	class DrawRect : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/DrawRectClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 1, 1.2, "XCoordinateRect", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.2, 1.2, QString::fromUtf8("X: "), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 1, 1.6, "YCoordinateRect", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -0.2, 1.6, QString::fromUtf8("Y: "), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1, 2, "WidthRect", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -0.2, 2, QString::fromUtf8("Ширина: "), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createLabel(7, 1, 2.4, "HeightRect", false, 0);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createLabel(8, -0.2, 2.4, QString::fromUtf8("Высота: "), 0);
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("XCoordinateRect"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("YCoordinateRect"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("WidthRect"));
			Q_UNUSED(repo);
			title_7->setTextFromRepo(repo->logicalProperty("HeightRect"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
		qReal::LabelInterface *title_7;
		qReal::LabelInterface *title_8;
	};

	class EngineCommand : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EngineCommandClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
		{
			return true;
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class EngineMovementCommand : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EngineMovementCommandClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class EnginesBackward : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EnginesBackwardClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Ports", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.16, -0.5, QString::fromUtf8("Порты:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.8, 1.2, "Power", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1, 1.2, QString::fromUtf8("Мощность (%):"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Ports"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Power"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
	};

	class EnginesForward : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EnginesForwardClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Ports", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.16, -0.5, QString::fromUtf8("Порты:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.8, 1.2, "Power", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1, 1.2, QString::fromUtf8("Мощность (%):"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Ports"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Power"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
	};

	class EnginesStop : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/EnginesStopClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Ports", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.2, -0.5, QString::fromUtf8("Порты:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Ports"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class FinalNode : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FinalNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class Fork : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ForkClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class Function : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/FunctionClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, -0.74, 1.2, QString::fromUtf8("Функция:"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.6, 1.2, "Body", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			Q_UNUSED(repo);
			title_2->setTextFromRepo(repo->logicalProperty("Body"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class IfBlock : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/IfBlockClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.7, 1.2, "Condition", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.5, 1.2, QString::fromUtf8("Условие:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class InitialNode : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/InitialNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class Loop : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/LoopClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Iterations", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.6, -0.5, QString::fromUtf8("Итераций:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Iterations"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class NullificationEncoder : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/NullificationEncoderClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Ports", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.2, -0.5, QString::fromUtf8("Порты:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Ports"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class PlayTone : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/PlayToneClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, -0.7, -0.5, QString::fromUtf8("Частота (Гц):"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.7, 1.2, QString::fromUtf8("Громкость (%):"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, -1.36, 1.6, QString::fromUtf8("Длительность (мс):"), 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1.36, 2, QString::fromUtf8("Ждать завершения:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1, -0.5, "Frequency", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(true);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 1.2, 1.2, "Volume", false, 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createLabel(7, 1.2, 1.6, "Duration", false, 0);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createLabel(8, 1.2, 2, "WaitForCompletion", false, 0);
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Frequency"));
			title_6->setTextFromRepo(repo->logicalProperty("Volume"));
			title_7->setTextFromRepo(repo->logicalProperty("Duration"));
			title_8->setTextFromRepo(repo->logicalProperty("WaitForCompletion"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
		qReal::LabelInterface *title_7;
		qReal::LabelInterface *title_8;
	};

	class PrintText : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/PrintTextClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 1, 1.2, "XCoordinateText", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, 1.2, QString::fromUtf8("X: "), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 1, 1.6, "YCoordinateText", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0, 1.6, QString::fromUtf8("Y: "), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1, 2, "PrintText", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 0, 2, QString::fromUtf8("Текст: "), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("XCoordinateText"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("YCoordinateText"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("PrintText"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class RobotsDiagramNode : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/RobotsDiagramNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(204);
			contents.setHeight(204);
			ports << portFactory.createPort(QPointF(0.00980392, 0.5), false, false, 204, 204, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.00980392), false, false, 204, 204, new NonTyped());
			ports << portFactory.createPort(QPointF(0.990196, 0.5), false, false, 204, 204, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.990196), false, false, 204, 204, new NonTyped());
			title_1 = factory.createLabel(1, 0.205882, 0.0588235, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->name());
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
		{
			return true;
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
	};

	class SensorBlock : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			Q_UNUSED(titles);
			Q_UNUSED(factory);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/SensorBlockClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
	};

	class Subprogram : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/SubprogramClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, -0.4, -0.5, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Subprogram() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }

		int getPenWidth() const { return 0; }

		QColor getPenColor() const { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->name());
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
	};

	class SubprogramDiagram : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/SubprogramDiagramClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(204);
			contents.setHeight(204);
			ports << portFactory.createPort(QPointF(0.00980392, 0.5), false, false, 204, 204, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.00980392), false, false, 204, 204, new NonTyped());
			ports << portFactory.createPort(QPointF(0.990196, 0.5), false, false, 204, 204, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.990196), false, false, 204, 204, new NonTyped());
			title_1 = factory.createLabel(1, 0.205882, 0.0588235, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~SubprogramDiagram() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() const { return Qt::SolidLine; }

		int getPenWidth() const { return 0; }

		QColor getPenColor() const { return QColor(); }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->name());
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
	};

	class Timer : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/TimerClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 1.3, -0.5, "Delay", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.6, -0.5, QString::fromUtf8("Задержка (мс):"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Delay"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class VariableInit : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/VariableInitClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.66, 1.2, "variable", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -1.2, 1.2, QString::fromUtf8("Переменная:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.66, 1.6, "value", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1.3, 1.6, QString::fromUtf8("Значение:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("variable"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("value"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
	};

	class WaitForAccelerometer : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForAccelerometerClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.66, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.66, 1.2, "Acceleration", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -0.52, 1.2, QString::fromUtf8("Ускорение:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.66, 1.6, "Sign", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -1.6, 1.6, QString::fromUtf8("Считанное значение:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createLabel(7, 0.66, 2, "AccelerationAxis", false, 0);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createLabel(8, -1.6, 2, QString::fromUtf8("Ускорение по оси:"), 0);
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Acceleration"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Sign"));
			Q_UNUSED(repo);
			title_7->setTextFromRepo(repo->logicalProperty("AccelerationAxis"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
		qReal::LabelInterface *title_7;
		qReal::LabelInterface *title_8;
	};

	class WaitForButtons : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForButtonsClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 1.4, 1.2, "RightButtonClicks", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, -0.4, 1.2, QString::fromUtf8("Правая: "), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 1.4, 1.6, "LeftButtonClicks", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -0.4, 1.6, QString::fromUtf8("Левая: "), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1.4, 2, "CentralButtonClicks", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -0.4, 2, QString::fromUtf8("Центральная: "), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
			title_7 = factory.createLabel(7, 1.4, 2.4, "BottomButtonClicks", false, 0);
			title_7->setBackground(Qt::transparent);
			title_7->setScaling(false, false);
			title_7->setHard(false);
			title_7->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_7);
			title_8 = factory.createLabel(8, -0.4, 2.4, QString::fromUtf8("Нижняя: "), 0);
			title_8->setBackground(Qt::transparent);
			title_8->setScaling(false, false);
			title_8->setHard(false);
			title_8->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_8);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("RightButtonClicks"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("LeftButtonClicks"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("CentralButtonClicks"));
			Q_UNUSED(repo);
			title_7->setTextFromRepo(repo->logicalProperty("BottomButtonClicks"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
		qReal::LabelInterface *title_7;
		qReal::LabelInterface *title_8;
	};

	class WaitForColor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForColorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.74, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.74, 1.2, "Color", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0, 1.2, QString::fromUtf8("Цвет:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Color"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
	};

	class WaitForColorIntensity : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForColorIntensityClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 1.1, 1.2, "Intensity", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1.1, 1.2, QString::fromUtf8("Интенсивность:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1.1, 1.6, "Sign", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -1.7, 1.6, QString::fromUtf8("Считанное значение:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Intensity"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Sign"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class WaitForEncoder : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForEncoderClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.8, 1.2, "TachoLimit", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1.8, 1.2, QString::fromUtf8("Предел оборотов:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1.2, 1.6, "Sign", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -1.8, 1.6, QString::fromUtf8("Считанное значение:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("TachoLimit"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Sign"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class WaitForGyroscope : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForGyroscopeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.7, 1.2, "Degrees", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -0.52, 1.2, QString::fromUtf8("Градусы:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1.1, 1.6, "Sign", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -1.8, 1.6, QString::fromUtf8("Считанное значение:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Degrees"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Sign"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class WaitForLight : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForLightClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.9, 1.2, "Percents", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -0.52, 1.2, QString::fromUtf8("Проценты:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1, 1.6, "Sign", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -1.8, 1.6, QString::fromUtf8("Считанное значение:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Percents"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Sign"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class WaitForSonarDistance : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForSonarDistanceClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 1, 1.2, "Distance", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -1.2, 1.2, QString::fromUtf8("Расстояние (см):"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1.1, 1.6, "Sign", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -1.8, 1.6, QString::fromUtf8("Считанное значение:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Distance"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Sign"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class WaitForSound : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForSoundClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.9, 1.2, "Volume", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, false);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, -0.6, 1.2, QString::fromUtf8("Громкость:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(false, false);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 1, 1.6, "Sign", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(false, false);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, -1.8, 1.6, QString::fromUtf8("Считанное значение:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(false, false);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
			Q_UNUSED(repo);
			title_3->setTextFromRepo(repo->logicalProperty("Volume"));
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("Sign"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class WaitForTouchSensor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitForTouchSensorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(50);
			contents.setHeight(50);
			ports << portFactory.createPort(QLineF(0, 0.1, 0, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 0, 0.9, 0), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(1, 0.1, 1, 0.9), false, false, false, false, 50, 50, new NonTyped());
			ports << portFactory.createPort(QLineF(0.1, 1, 0.9, 1), false, false, false, false, 50, 50, new NonTyped());
			title_1 = factory.createLabel(1, 0.8, -0.5, "Port", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setHard(true);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0, -0.5, QString::fromUtf8("Порт:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(true);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			mRenderer->setElementRepo(repo);
			title_1->setTextFromRepo(repo->logicalProperty("Port"));
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

		QVector<int> sizeOfForestalling() const
		{
			QVector<int> result;
			result << 0 << 0 << 0 << 0;
;
			return result;
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

		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isPort() const
		{
			return false;
		}

		bool hasPin() const
		{
			return false;
		}

		bool createChildrenFromMenu() const
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
		qReal::SdfRendererInterface *mRenderer;
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

