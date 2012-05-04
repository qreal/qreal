#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../../qrgui/editorPluginInterface/elementTitleHelpers.h"

	class AbstractErrorTypeNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractErrorTypeNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/AbstractErrorTypeNodePorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~AbstractErrorTypeNode() {}

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

	class AbstractExistsNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractExistsNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/AbstractExistsNodePorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~AbstractExistsNode() {}

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

	class AbstractListOfElementsNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractListOfElementsNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/AbstractListOfElementsNodePorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~AbstractListOfElementsNode() {}

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

	class AbstractNodeForNodeConstraint : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractNodeForNodeConstraintClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/AbstractNodeForNodeConstraintPorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~AbstractNodeForNodeConstraint() {}

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

	class AbstractSelectionNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/AbstractSelectionNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/AbstractSelectionNodePorts.sdf"));
			contents.setWidth(-1);
			contents.setHeight(-1);
		}

		 ElementImpl *clone() { return NULL; }
		~AbstractSelectionNode() {}

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

	class BeginNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/BeginNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/BeginNodePorts.sdf"));
			contents.setWidth(149);
			contents.setHeight(149);
			{
				StatLine ln;
				ln.line = QLineF(-0.00671141, 0.0872483, 0, 0.899329);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 149;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.986577, 0.0939597, 0.993289, 0.90604);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 149;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.90604, -0.00671141, 0.0939597, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 149;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.899329, 0.993289, 0.0872483, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 149;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.120805, 0.134228, "exists", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~BeginNode() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("exists")).replace("\n", "<br>"));
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

	class Childrens : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/ChildrensClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ChildrensPorts.sdf"));
			contents.setWidth(149);
			contents.setHeight(152);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.0986842, 0, 0.901316);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 152;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.90604, 0, 0.0872483, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 152;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.993289, 0.111842, 0.993289, 0.914474);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 152;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.912752, 0.993421, 0.0939597, 0.993421);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 149;
				ln.initHeight = 152;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.134228, 0.125, "selection", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.644295, 0.125, "count", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~Childrens() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("selection")).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("count")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class ConstraintsDiagram : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/ConstraintsDiagramClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ConstraintsDiagramPorts.sdf"));
			contents.setWidth(139);
			contents.setHeight(139);
		}

		 ElementImpl *clone() { return NULL; }
		~ConstraintsDiagram() {}

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

	class EdgeConstraint : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EdgeConstraintClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EdgeConstraintPorts.sdf"));
			contents.setWidth(248);
			contents.setHeight(197);
			title_1 = factory.createTitle(0.0443548, 0.0507614, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.0403226, 0.862944, "errorType", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~EdgeConstraint() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->name()).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("errorType")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class EdgesConstraint : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EdgesConstraintClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EdgesConstraintPorts.sdf"));
			contents.setWidth(249);
			contents.setHeight(198);
			title_1 = factory.createTitle(0.0441767, 0.0505051, "selection", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.0401606, 0.858586, "errorType", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~EdgesConstraint() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("selection")).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("errorType")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class EndNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/EndNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/EndNodePorts.sdf"));
			contents.setWidth(148);
			contents.setHeight(149);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.0939597, 0.00675676, 0.90604);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 148;
				ln.initHeight = 149;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.905405, -0.00671141, 0.0945946, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 148;
				ln.initHeight = 149;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.993243, 0.100671, 1, 0.912752);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 148;
				ln.initHeight = 149;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.905405, 0.993289, 0.0945946, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 148;
				ln.initHeight = 149;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.121622, 0.14094, "exists", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~EndNode() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("exists")).replace("\n", "<br>"));
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

	class IncomingLinks : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/IncomingLinksClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/IncomingLinksPorts.sdf"));
			contents.setWidth(199);
			contents.setHeight(150);
			{
				StatLine ln;
				ln.line = QLineF(-0.00502513, 0.293333, 0, 0.733333);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 199;
				ln.initHeight = 150;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.994975, 0.273333, 1, 0.72);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 199;
				ln.initHeight = 150;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.859296, 0.993333, 0.432161, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 199;
				ln.initHeight = 150;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.839196, -0.00666667, 0.442211, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 199;
				ln.initHeight = 150;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.396985, 0.126667, "selection", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.703518, 0.12, "count", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~IncomingLinks() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("selection")).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("count")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class IncomingNodes : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/IncomingNodesClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/IncomingNodesPorts.sdf"));
			contents.setWidth(201);
			contents.setHeight(150);
			{
				StatLine ln;
				ln.line = QLineF(-0.00497512, 0.306667, 0, 0.72);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 201;
				ln.initHeight = 150;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.995025, 0.106667, 1, 0.886667);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 201;
				ln.initHeight = 150;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.910448, 0.993333, 0.328358, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 201;
				ln.initHeight = 150;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.915423, -0.00666667, 0.333333, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 201;
				ln.initHeight = 150;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.328358, 0.0733333, "selection", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.771144, 0.0733333, "count", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~IncomingNodes() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("selection")).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("count")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class MetamodelConstraints : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/MetamodelConstraintsClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/MetamodelConstraintsPorts.sdf"));
			contents.setWidth(88);
			contents.setHeight(79);
		}

		 ElementImpl *clone() { return NULL; }
		~MetamodelConstraints() {}

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

	class NodeConstraint : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/NodeConstraintClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/NodeConstraintPorts.sdf"));
			contents.setWidth(247);
			contents.setHeight(196);
			title_1 = factory.createTitle(0.0445344, 0.0510204, "name", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.0404858, 0.867347, "errorType", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~NodeConstraint() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->name()).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("errorType")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class NodesConstraint : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/NodesConstraintClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/NodesConstraintPorts.sdf"));
			contents.setWidth(248);
			contents.setHeight(197);
			title_1 = factory.createTitle(0.0443548, 0.0507614, "selection", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(true, true);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.0403226, 0.862944, "errorType", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(true, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~NodesConstraint() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("selection")).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("errorType")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class Or : public ElementImpl {
	public:
		void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
											ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &,
											SdfRendererInterface *, SdfRendererInterface *) {}

		void init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
		{
			Q_UNUSED(titles);
			Q_UNUSED(factory);
		}

		virtual ~Or() {}

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

		void updateData(ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo);
		}

	private:
	};

	class OutgoingLinks : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/OutgoingLinksClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/OutgoingLinksPorts.sdf"));
			contents.setWidth(200);
			contents.setHeight(151);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.298013, 0, 0.728477);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 151;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.995, 0.278146, 0.995, 0.715232);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 151;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.855, 0.993377, 0.435, 0.993377);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 151;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.835, 0, 0.445, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 200;
				ln.initHeight = 151;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.405, 0.125828, "selection", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.72, 0.125828, "count", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~OutgoingLinks() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("selection")).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("count")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class OutgoingNodes : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/OutgoingNodesClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/OutgoingNodesPorts.sdf"));
			contents.setWidth(203);
			contents.setHeight(151);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.304636, 0, 0.721854);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 203;
				ln.initHeight = 151;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.995074, 0.10596, 0.995074, 0.887417);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 203;
				ln.initHeight = 151;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.926108, 0.993377, 0.330049, 0.993377);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 203;
				ln.initHeight = 151;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.931034, 0, 0.334975, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 203;
				ln.initHeight = 151;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.325123, 0.0860927, "selection", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
			title_2 = factory.createTitle(0.773399, 0.0860927, "count", false);
			title_2->setBackground(Qt::transparent);
			title_2->setScaling(false, false);
			title_2->setFlags(0);
			title_2->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_2);
		}

		 ElementImpl *clone() { return NULL; }
		~OutgoingNodes() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("selection")).replace("\n", "<br>"));
			title_2->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("count")).replace("\n", "<br>"));
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
		ElementTitleInterface *title_2;
	};

	class Parent : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/ParentClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/ParentPorts.sdf"));
			contents.setWidth(150);
			contents.setHeight(153);
			{
				StatLine ln;
				ln.line = QLineF(0, 0.0718954, 0, 0.882353);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 150;
				ln.initHeight = 153;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.993333, 0.0980392, 0.993333, 0.908497);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 150;
				ln.initHeight = 153;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.926667, 0, 0.0933333, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 150;
				ln.initHeight = 153;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.926667, 0.993464, 0.0933333, 0.993464);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 150;
				ln.initHeight = 153;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.12, 0.117647, "exists", false);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Parent() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("exists")).replace("\n", "<br>"));
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

	class PropertyNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/PropertyNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/PropertyNodePorts.sdf"));
			contents.setWidth(207);
			contents.setHeight(37);
			{
				StatLine ln;
				ln.line = QLineF(0.995169, 0.189189, 1, 0.756757);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 207;
				ln.initHeight = 37;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(-0.00483092, 0.189189, 0, 0.756757);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 207;
				ln.initHeight = 37;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.154589, -0.027027, 0.801932, 0);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 207;
				ln.initHeight = 37;
				linePorts << ln;
			};
			{
				StatLine ln;
				ln.line = QLineF(0.154589, 0.972973, 0.801932, 1);
				ln.prop_x1 = false;
				ln.prop_y1 = false; 
				ln.prop_x2 = false; 
				ln.prop_y2 = false; 
				ln.initWidth = 207;
				ln.initHeight = 37;
				linePorts << ln;
			};
			title_1 = factory.createTitle(0.0193237, 0.189189, "##property## ##sign## ##value##", true);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~PropertyNode() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(QString::fromUtf8("") + repo->logicalProperty("property") + QString::fromUtf8(" ") + repo->logicalProperty("sign") + QString::fromUtf8(" ") + repo->logicalProperty("value") + QString::fromUtf8("")).replace("\n", "<br>"));
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

