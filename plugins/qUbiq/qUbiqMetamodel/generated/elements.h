#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../../qrgui/editorPluginInterface/labelFactoryInterface.h"
#include "../../../../qrgui/editorPluginInterface/labelInterface.h"
#include "ports.h"

	class AbstractConditionalNode : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractConditionalNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~AbstractConditionalNode() {}

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

	class AbstractGridOrListNode : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractGridOrListNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~AbstractGridOrListNode() {}

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

	class BeginNode : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/BeginNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(38);
			contents.setHeight(38);
			ports << portFactory.createPort(QPointF(0.0526316, 0.5), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.947368, 0.5), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.947368), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.0526316), false, false, 38, 38, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~BeginNode() {}

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

	class Change : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ChangeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(157);
			contents.setHeight(100);
			ports << portFactory.createPort(QLineF(0.0636943, 0.99, 0.88535, 1), false, true, false, true, 157, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0636943, -0.01, 0.88535, 0), false, true, false, true, 157, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.949045, 0.1, 0.955414, 0.89), false, true, false, true, 157, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00636943, 0.1, 0, 0.89), false, true, false, true, 157, 100, new NonTyped());
			title_1 = factory.createLabel(1, 0.0636943, 0.1, QString::fromUtf8("CHANGE"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.44586, 0.1, "elementId", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.509554, 0.4, "propertyName", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0.509554, 0.7, "newValue", false, 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(true, true);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.0636943, 0.4, QString::fromUtf8("value name:"), 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(true, true);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 0.0636943, 0.7, QString::fromUtf8("new value:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(true, true);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Change() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("elementId"));
			title_3->setTextFromRepo(repo->logicalProperty("propertyName"));
			title_4->setTextFromRepo(repo->logicalProperty("newValue"));
			Q_UNUSED(repo);
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class ConditionLink : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			title_1 = factory.createLabel(1, 0, 0, "boolValueStereotype", false, 0);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		virtual ~ConditionLink() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
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
			return enums::linkShape::square;
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
			brush.setColor(Qt::white);
			painter->setBrush(brush);
			static const QPointF points[] = {
				QPointF(0,0),
				QPointF(-5,10),
				QPointF(5,10)
			};
			painter->drawPolygon(points, 3);
			painter->setBrush(old);
		}

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			title_1->setTextFromRepo(repo->logicalProperty("boolValueStereotype"));
		}

	private:
		qReal::LabelInterface *title_1;
	};

	class CyclicLink : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			title_1 = factory.createLabel(1, 0, 0, "cyclicValueStereotype", false, 0);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		virtual ~CyclicLink() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
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
			return enums::linkShape::square;
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			title_1->setTextFromRepo(repo->logicalProperty("cyclicValueStereotype"));
		}

	private:
		qReal::LabelInterface *title_1;
	};

	class ForNode : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ForNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(104);
			contents.setHeight(74);
			ports << portFactory.createPort(QPointF(0.5, 0.027027), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.0192308, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.980769, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.972973), false, true, 104, 74, new NonTyped());
			title_1 = factory.createLabel(1, 0.211538, 0.432432, "gridOrListId", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.403846, 0.0945946, QString::fromUtf8("FOR"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~ForNode() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("gridOrListId"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class IfNode : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/IfNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(104);
			contents.setHeight(74);
			ports << portFactory.createPort(QPointF(0.5, 0.027027), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.0192308, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.980769, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.972973), false, true, 104, 74, new NonTyped());
			title_1 = factory.createLabel(1, 0.259615, 0.432432, "condition", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.451923, 0.0945946, QString::fromUtf8("IF"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~IfNode() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("condition"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class Link : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~Link() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
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
			return enums::linkShape::square;
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
			Q_UNUSED(repo);
		}

	private:
	};

	class OutputSplitter : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/OutputSplitterClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(80);
			contents.setHeight(15);
			ports << portFactory.createPort(QLineF(0.125, -0.0666667, 0.8625, 0), false, true, false, true, 80, 15, new NonTyped());
			ports << portFactory.createPort(QLineF(0.125, 0.933333, 0.8625, 1), false, true, false, true, 80, 15, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~OutputSplitter() {}

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

	class QUbiqConditionDiagram : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/QUbiqConditionDiagramClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(176);
			contents.setHeight(133);
			title_1 = factory.createLabel(1, 0.0397727, 0.0526316, QString::fromUtf8("Condition Diagram"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0454545, 0.255639, "name", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~QUbiqConditionDiagram() {}

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
			title_2->setTextFromRepo(repo->name());
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
		qReal::LabelInterface *title_2;
	};

	class Return : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ReturnClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(80);
			contents.setHeight(75);
			ports << portFactory.createPort(QLineF(0.125, -0.0133333, 0.8625, 0), true, true, true, true, 80, 75, new NonTyped());
			ports << portFactory.createPort(QLineF(0.9875, 0.0666667, 1, 0.32), true, true, true, true, 80, 75, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.0125, 0.0666667, 0, 0.32), true, true, true, true, 80, 75, new NonTyped());
			title_1 = factory.createLabel(1, 0.1875, 0.0666667, QString::fromUtf8("RETURN"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.3125, 0.466667, "value", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Return() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("value"));
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
		qReal::LabelInterface *title_2;
	};

	class TakeGridOrListElements : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/TakeGridOrListElementsClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(224);
			contents.setHeight(100);
			ports << portFactory.createPort(QLineF(0.0446429, 0.99, 0.933036, 1), false, true, false, true, 224, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0446429, -0.01, 0.933036, 0), false, true, false, true, 224, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.977679, 0.1, 0.982143, 0.89), false, true, false, true, 224, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00446429, 0.1, 0, 0.89), false, true, false, true, 224, 100, new NonTyped());
			title_1 = factory.createLabel(1, 0.0446429, 0.1, QString::fromUtf8("GRID or LIST ELEMENTS of"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.714286, 0.1, "gridOrListId", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.0446429, 0.4, QString::fromUtf8("by condition:"), 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0.0446429, 0.7, QString::fromUtf8("saved in:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(true, true);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.446429, 0.4, "condition", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(true, true);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 0.446429, 0.7, "variableId", false, 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(true, true);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~TakeGridOrListElements() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("gridOrListId"));
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("condition"));
			title_6->setTextFromRepo(repo->logicalProperty("variableId"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class Variable : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/VariableClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(81);
			contents.setHeight(85);
			title_1 = factory.createLabel(1, 0.271605, 0.188235, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.283951, 0.552941, "type", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Variable() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("type"));
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
		qReal::LabelInterface *title_2;
	};

	class While : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WhileClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(104);
			contents.setHeight(74);
			ports << portFactory.createPort(QPointF(0.5, 0.027027), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.0192308, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.980769, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.972973), false, true, 104, 74, new NonTyped());
			title_1 = factory.createLabel(1, 0.259615, 0.432432, "condition", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.336538, 0.0945946, QString::fromUtf8("WHILE"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~While() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("condition"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class AbstractMessage : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractMessageClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~AbstractMessage() {}

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

	class BeginNodeInLogicEditor : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/BeginNodeInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(38);
			contents.setHeight(38);
			ports << portFactory.createPort(QPointF(0.0526316, 0.5), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.947368, 0.5), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.947368), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.0526316), false, false, 38, 38, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~BeginNodeInLogicEditor() {}

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

	class ChangeInLogicEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ChangeInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(157);
			contents.setHeight(100);
			ports << portFactory.createPort(QLineF(0.0636943, 0.99, 0.88535, 1), false, true, false, true, 157, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0636943, -0.01, 0.88535, 0), false, true, false, true, 157, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.949045, 0.1, 0.955414, 0.89), false, true, false, true, 157, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00636943, 0.1, 0, 0.89), false, true, false, true, 157, 100, new NonTyped());
			title_1 = factory.createLabel(1, 0.0636943, 0.1, QString::fromUtf8("CHANGE"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.44586, 0.1, "elementId", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.509554, 0.4, "propertyName", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0.509554, 0.7, "newValue", false, 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(true, true);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.0636943, 0.4, QString::fromUtf8("value name:"), 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(true, true);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 0.0636943, 0.7, QString::fromUtf8("new value:"), 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(true, true);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~ChangeInLogicEditor() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("elementId"));
			title_3->setTextFromRepo(repo->logicalProperty("propertyName"));
			title_4->setTextFromRepo(repo->logicalProperty("newValue"));
			Q_UNUSED(repo);
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class ConditionLinkInLogicEditor : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			title_1 = factory.createLabel(1, 0, 0, "boolValueStereotype", false, 0);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		virtual ~ConditionLinkInLogicEditor() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
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
			return enums::linkShape::square;
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
			brush.setColor(Qt::white);
			painter->setBrush(brush);
			static const QPointF points[] = {
				QPointF(0,0),
				QPointF(-5,10),
				QPointF(5,10)
			};
			painter->drawPolygon(points, 3);
			painter->setBrush(old);
		}

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			title_1->setTextFromRepo(repo->logicalProperty("boolValueStereotype"));
		}

	private:
		qReal::LabelInterface *title_1;
	};

	class CyclicLinkInLogicEditor : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			title_1 = factory.createLabel(1, 0, 0, "cyclicValueStereotype", false, 0);
			title_1->setBackground(Qt::white);
			title_1->setScaling(false, false);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		virtual ~CyclicLinkInLogicEditor() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
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
			return enums::linkShape::square;
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

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			title_1->setTextFromRepo(repo->logicalProperty("cyclicValueStereotype"));
		}

	private:
		qReal::LabelInterface *title_1;
	};

	class ForNodeInLogicEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ForNodeInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(104);
			contents.setHeight(74);
			ports << portFactory.createPort(QPointF(0.5, 0.027027), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.0192308, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.980769, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.972973), false, true, 104, 74, new NonTyped());
			title_1 = factory.createLabel(1, 0.211538, 0.432432, "gridOrListId", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.403846, 0.0945946, QString::fromUtf8("FOR"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~ForNodeInLogicEditor() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("gridOrListId"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class Go : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/GoClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(110);
			contents.setHeight(40);
			ports << portFactory.createPort(QLineF(0.0909091, 0.975, 0.9, 1), false, true, false, true, 110, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(0.990909, 0.225, 1, 0.25), false, true, false, true, 110, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0909091, -0.025, 0.9, 0), false, true, false, true, 110, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00909091, 0.25, 0, 0.725), false, true, false, true, 110, 40, new NonTyped());
			title_1 = factory.createLabel(1, 0.454545, 0.25, "goto", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0909091, 0.25, QString::fromUtf8("GO to:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Go() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("goto"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class IfNodeInLogicEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/IfNodeInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(104);
			contents.setHeight(74);
			ports << portFactory.createPort(QPointF(0.5, 0.027027), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.0192308, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.980769, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.972973), false, true, 104, 74, new NonTyped());
			title_1 = factory.createLabel(1, 0.259615, 0.432432, "condition", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.451923, 0.0945946, QString::fromUtf8("IF"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~IfNodeInLogicEditor() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("condition"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class LinkInLogicEditor : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~LinkInLogicEditor() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
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
			return enums::linkShape::square;
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
			Q_UNUSED(repo);
		}

	private:
	};

	class Message : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/MessageClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(140);
			contents.setHeight(100);
			ports << portFactory.createPort(QLineF(0.0714286, 0.99, 0.921429, 1), false, true, false, true, 140, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0714286, -0.01, 0.921429, 0), false, true, false, true, 140, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.992857, 0.1, 1, 0.89), false, true, false, true, 140, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00714286, 0.1, 0, 0.89), false, true, false, true, 140, 100, new NonTyped());
			title_1 = factory.createLabel(1, 0.0714286, 0.1, QString::fromUtf8("MESSAGE"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0714286, 0.4, QString::fromUtf8("type:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.0714286, 0.7, QString::fromUtf8("to:"), 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0.357143, 0.4, "messageType", false, 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(true, true);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.357143, 0.7, "sentTo", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(true, true);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Message() {}

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
			title_4->setTextFromRepo(repo->logicalProperty("messageType"));
			title_5->setTextFromRepo(repo->logicalProperty("sentTo"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
	};

	class OutputSplitterInLogicEditor : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/OutputSplitterInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(80);
			contents.setHeight(15);
			ports << portFactory.createPort(QLineF(0.125, -0.0666667, 0.8625, 0), false, true, false, true, 80, 15, new NonTyped());
			ports << portFactory.createPort(QLineF(0.125, 0.933333, 0.8625, 1), false, true, false, true, 80, 15, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~OutputSplitterInLogicEditor() {}

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

	class QUbiqLogicDiagram : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/QUbiqLogicDiagramClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(170);
			contents.setHeight(130);
			title_1 = factory.createLabel(1, 0.0352941, 0.0461538, QString::fromUtf8("Logic Diagram"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0352941, 0.230769, "handlerType", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.529412, 0.0461538, "name", false, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~QUbiqLogicDiagram() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("handlerType"));
			title_3->setTextFromRepo(repo->name());
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
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
	};

	class ReturnInLogicEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ReturnInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(80);
			contents.setHeight(75);
			ports << portFactory.createPort(QLineF(0.125, -0.0133333, 0.8625, 0), true, true, true, true, 80, 75, new NonTyped());
			ports << portFactory.createPort(QLineF(0.9875, 0.0666667, 1, 0.32), true, true, true, true, 80, 75, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.0125, 0.0666667, 0, 0.32), true, true, true, true, 80, 75, new NonTyped());
			title_1 = factory.createLabel(1, 0.1875, 0.0666667, QString::fromUtf8("RETURN"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.3125, 0.466667, "value", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~ReturnInLogicEditor() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("value"));
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
		qReal::LabelInterface *title_2;
	};

	class Take : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/TakeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(150);
			contents.setHeight(90);
			ports << portFactory.createPort(QLineF(0.0666667, 0.988889, 0.926667, 1), false, true, false, true, 150, 90, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0666667, -0.0111111, 0.926667, 0), false, true, false, true, 150, 90, new NonTyped());
			ports << portFactory.createPort(QLineF(0.993333, 0.111111, 1, 0.877778), false, true, false, true, 150, 90, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00666667, 0.111111, 0, 0.877778), false, true, false, true, 150, 90, new NonTyped());
			title_1 = factory.createLabel(1, 0.0666667, 0.111111, QString::fromUtf8("TAKE"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0666667, 0.388889, QString::fromUtf8("from:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.0666667, 0.666667, QString::fromUtf8("to:"), 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0.333333, 0.388889, "takeFrom", false, 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(true, true);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.333333, 0.666667, "takeTo", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(true, true);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Take() {}

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
			title_4->setTextFromRepo(repo->logicalProperty("takeFrom"));
			title_5->setTextFromRepo(repo->logicalProperty("takeTo"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
	};

	class TakeGridOrListElementsInLogicEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/TakeGridOrListElementsInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(224);
			contents.setHeight(100);
			ports << portFactory.createPort(QLineF(0.0446429, 0.99, 0.933036, 1), false, true, false, true, 224, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0446429, -0.01, 0.933036, 0), false, true, false, true, 224, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.977679, 0.1, 0.982143, 0.89), false, true, false, true, 224, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00446429, 0.1, 0, 0.89), false, true, false, true, 224, 100, new NonTyped());
			title_1 = factory.createLabel(1, 0.0446429, 0.1, QString::fromUtf8("GRID or LIST ELEMENTS of"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.714286, 0.1, "gridOrListId", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.0446429, 0.4, QString::fromUtf8("by condition:"), 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0.0446429, 0.7, QString::fromUtf8("saved in:"), 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(true, true);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.446429, 0.4, "condition", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(true, true);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
			title_6 = factory.createLabel(6, 0.446429, 0.7, "variableId", false, 0);
			title_6->setBackground(Qt::transparent);
			title_6->setScaling(true, true);
			title_6->setHard(false);
			title_6->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_6);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~TakeGridOrListElementsInLogicEditor() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("gridOrListId"));
			Q_UNUSED(repo);
			Q_UNUSED(repo);
			title_5->setTextFromRepo(repo->logicalProperty("condition"));
			title_6->setTextFromRepo(repo->logicalProperty("variableId"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
		qReal::LabelInterface *title_6;
	};

	class VariableInLogicEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/VariableInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(81);
			contents.setHeight(85);
			title_1 = factory.createLabel(1, 0.271605, 0.188235, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.283951, 0.552941, "type", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~VariableInLogicEditor() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("type"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class WaitMessage : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WaitMessageClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(142);
			contents.setHeight(100);
			ports << portFactory.createPort(QLineF(0.0704225, 0.99, 0.908451, 1), false, true, false, true, 142, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0704225, -0.01, 0.908451, 0), false, true, false, true, 142, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(0.978873, 0.1, 0.985915, 0.89), false, true, false, true, 142, 100, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00704225, 0.1, 0, 0.89), false, true, false, true, 142, 100, new NonTyped());
			title_1 = factory.createLabel(1, 0.0704225, 0.1, QString::fromUtf8("WAIT MESSAGE"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0704225, 0.4, QString::fromUtf8("type:"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.0704225, 0.7, QString::fromUtf8("from:"), 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
			title_4 = factory.createLabel(4, 0.352113, 0.4, "waitMessageType", false, 0);
			title_4->setBackground(Qt::transparent);
			title_4->setScaling(true, true);
			title_4->setHard(false);
			title_4->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_4);
			title_5 = factory.createLabel(5, 0.352113, 0.7, "waitFrom", false, 0);
			title_5->setBackground(Qt::transparent);
			title_5->setScaling(true, true);
			title_5->setHard(false);
			title_5->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_5);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~WaitMessage() {}

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
			title_4->setTextFromRepo(repo->logicalProperty("waitMessageType"));
			title_5->setTextFromRepo(repo->logicalProperty("waitFrom"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
		qReal::LabelInterface *title_4;
		qReal::LabelInterface *title_5;
	};

	class WhileInLogicEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/WhileInLogicEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(104);
			contents.setHeight(74);
			ports << portFactory.createPort(QPointF(0.5, 0.027027), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.0192308, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.980769, 0.5), false, true, 104, 74, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.972973), false, true, 104, 74, new NonTyped());
			title_1 = factory.createLabel(1, 0.259615, 0.432432, "condition", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.336538, 0.0945946, QString::fromUtf8("WHILE"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~WhileInLogicEditor() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("condition"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class AbstractHandlerNode : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractHandlerNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~AbstractHandlerNode() {}

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

	class AbstractIsNullNode : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractIsNullNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~AbstractIsNullNode() {}

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

	class AbstractListsNode : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractListsNodeClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~AbstractListsNode() {}

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

	class AbstractSlideElement : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractSlideElementClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~AbstractSlideElement() {}

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

	class BeginNodeInPresentationEditor : public qReal::ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/BeginNodeInPresentationEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(38);
			contents.setHeight(38);
			ports << portFactory.createPort(QPointF(0.0526316, 0.5), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.947368, 0.5), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.947368), false, false, 38, 38, new NonTyped());
			ports << portFactory.createPort(QPointF(0.5, 0.0526316), false, false, 38, 38, new NonTyped());
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~BeginNodeInPresentationEditor() {}

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

	class Button : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ButtonClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(121);
			contents.setHeight(40);
			ports << portFactory.createPort(QLineF(0.0826446, 0.975, 0.818182, 1), false, false, false, false, 121, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(0.900826, 0.25, 0.909091, 0.725), false, false, false, false, 121, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0826446, -0.025, 0.818182, 0), false, false, false, false, 121, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00826446, 0.25, 0, 0.725), false, false, false, false, 121, 40, new NonTyped());
			title_1 = factory.createLabel(1, 0.495868, 0.25, "buttonText", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0826446, 0.25, QString::fromUtf8("BUTTON"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Button() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("buttonText"));
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
		qReal::LabelInterface *title_2;
	};

	class ExitButton : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ExitButtonClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(141);
			contents.setHeight(40);
			ports << portFactory.createPort(QLineF(0.070922, 0.975, 0.843972, 1), false, false, false, false, 141, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(0.914894, 0.25, 0.921986, 0.725), false, false, false, false, 141, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(0.070922, -0.025, 0.843972, 0), false, false, false, false, 141, 40, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.0070922, 0.25, 0, 0.725), false, false, false, false, 141, 40, new NonTyped());
			title_1 = factory.createLabel(1, 0.567376, 0.25, "buttonText", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.070922, 0.25, QString::fromUtf8("EXIT BUTTON"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~ExitButton() {}

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
			title_1->setTextFromRepo(repo->logicalProperty("buttonText"));
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
		qReal::LabelInterface *title_2;
	};

	class Grid : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/GridClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(130);
			contents.setHeight(40);
			title_1 = factory.createLabel(1, 0.384615, 0.25, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.692308, 0.25, "##w## x ##h##", true, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.0769231, 0.25, QString::fromUtf8("GRID"), 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(true, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Grid() {}

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
			title_2->setTextFromRepo(QString::fromUtf8("") + repo->logicalProperty("w") + QString::fromUtf8(" x ") + repo->logicalProperty("h") + QString::fromUtf8(""));
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
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
	};

	class Image : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ImageClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(110);
			contents.setHeight(40);
			title_1 = factory.createLabel(1, 0.5, 0.25, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0909091, 0.25, QString::fromUtf8("IMAGE"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Image() {}

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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class List : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/ListClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(110);
			contents.setHeight(40);
			title_1 = factory.createLabel(1, 0.363636, 0.25, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0909091, 0.25, QString::fromUtf8("LIST"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~List() {}

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
		qReal::LabelInterface *title_2;
	};

	class QUbiqPresentationDiagram : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/QUbiqPresentationDiagramClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(175);
			contents.setHeight(132);
			title_1 = factory.createLabel(1, 0.04, 0.0530303, QString::fromUtf8("Presentation Diagram"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0457143, 0.257576, "name", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~QUbiqPresentationDiagram() {}

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
			title_2->setTextFromRepo(repo->name());
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
		qReal::LabelInterface *title_2;
	};

	class Slide : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/SlideClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(150);
			contents.setHeight(120);
			ports << portFactory.createPort(QLineF(0.0666667, 0.991667, 0.926667, 1), false, false, false, false, 150, 120, new NonTyped());
			ports << portFactory.createPort(QLineF(0.0666667, -0.00833333, 0.926667, 0), false, false, false, false, 150, 120, new NonTyped());
			ports << portFactory.createPort(QLineF(0.993333, 0.0833333, 1, 0.908333), false, false, false, false, 150, 120, new NonTyped());
			ports << portFactory.createPort(QLineF(-0.00666667, 0.0833333, 0, 0.908333), false, false, false, false, 150, 120, new NonTyped());
			title_1 = factory.createLabel(1, 0.0666667, 0.0833333, QString::fromUtf8("SLIDE"), 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.333333, 0.0833333, "name", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
			title_3 = factory.createLabel(3, 0.666667, 0.0833333, "##width## x ##height##", true, 0);
			title_3->setBackground(Qt::transparent);
			title_3->setScaling(false, true);
			title_3->setHard(false);
			title_3->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_3);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Slide() {}

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
			title_2->setTextFromRepo(repo->name());
			title_3->setTextFromRepo(QString::fromUtf8("") + repo->logicalProperty("width") + QString::fromUtf8(" x ") + repo->logicalProperty("height") + QString::fromUtf8(""));
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
		qReal::LabelInterface *title_2;
		qReal::LabelInterface *title_3;
	};

	class Text : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/TextClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(110);
			contents.setHeight(40);
			title_1 = factory.createLabel(1, 0.454545, 0.25, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.0909091, 0.25, QString::fromUtf8("TEXT"), 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~Text() {}

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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

	class TransitionLink : public qReal::ElementImpl {
	public:
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~TransitionLink() {}

		qReal::ElementImpl *clone() { return NULL; }
		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isResizeable() const { return true; }
		bool isContainer() const { return false; }
		bool isDividable() const { return false; }
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
			return enums::linkShape::square;
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
		Qt::PenStyle getPenStyle() const { return Qt::DashLine; }
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
			Q_UNUSED(repo);
		}

	private:
	};

	class VariableInPresentationEditor : public qReal::ElementImpl
	{
	public:
		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

		void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports
							, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles
							, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)
		{
			Q_UNUSED(portFactory);
			Q_UNUSED(ports);
			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/VariableInPresentationEditorClass.sdf"));
			mRenderer->setElementRepo(elementRepo);
			contents.setWidth(81);
			contents.setHeight(85);
			title_1 = factory.createLabel(1, 0.271605, 0.188235, "name", false, 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setHard(false);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createLabel(2, 0.283951, 0.552941, "type", false, 0);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, true);
			title_2->setHard(false);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 qReal::ElementImpl *clone() { return NULL; }
		~VariableInPresentationEditor() {}

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
			title_2->setTextFromRepo(repo->logicalProperty("type"));
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
		qReal::LabelInterface *title_1;
		qReal::LabelInterface *title_2;
	};

