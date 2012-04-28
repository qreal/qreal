#pragma once

#include <QBrush>
#include <QPainter>

#include "../../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../../qrgui/editorPluginInterface/elementTitleHelpers.h"

	class Atom1Node : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/Atom1NodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/Atom1NodePorts.sdf"));
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
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Atom1Node() {}

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
			title_1->setHtml(QString("<center>%1</center>").arg(repo->logicalProperty("process")).replace("\n", "<br>"));
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

	class Atom2Node : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/Atom2NodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/Atom2NodePorts.sdf"));
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
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Atom2Node() {}

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
			title_1->setHtml(QString("<center>%1</center>").arg(repo->logicalProperty("process")).replace("\n", "<br>"));
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

	class Atom3Node : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/Atom3NodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/Atom3NodePorts.sdf"));
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
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Atom3Node() {}

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
			title_1->setHtml(QString("<center>%1</center>").arg(repo->logicalProperty("process")).replace("\n", "<br>"));
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

	class Atom4Node : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/Atom4NodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/Atom4NodePorts.sdf"));
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
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~Atom4Node() {}

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
			title_1->setHtml(QString("<center>%1</center>").arg(repo->logicalProperty("process")).replace("\n", "<br>"));
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

	class DragonAtomNode : public ElementImpl
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
			mRenderer->load(QString(":/generated/shapes/DragonAtomNodeClass.sdf"));
			portRenderer->load(QString(":/generated/shapes/DragonAtomNodePorts.sdf"));
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
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);
		}

		 ElementImpl *clone() { return NULL; }
		~DragonAtomNode() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->logicalProperty("name")).replace("\n", "<br>"));
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

