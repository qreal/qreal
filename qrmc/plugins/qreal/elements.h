#pragma once

#include <QBrush>
#include <QPainter>
                                                                     
#include "../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../qrgui/editorPluginInterface/elementRepoInterface.h"
#include "../../../qrgui/umllib/element.h"

#include "../../../qrgui/editorPluginInterface/labelFactoryInterface.h"
#include "../../../qrgui/editorPluginInterface/labelInterface.h"


	class Node : public qReal::ElementImpl
	{
	public:

		void init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}

                void init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports,
                                 qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles,
	                         qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)


		/*void init(QRectF &contents, QList<StatPoint> &pointPorts,
				QList<StatLine> &linePorts, qReal::LabelFactoryInterface &factory,
				QList<qReal::LabelFactoryInterface*> &titles, qReal::SdfRendererInterface *renderer,
				SdfRendererInterface *portRenderer)    */
		{

			mRenderer = renderer;
			mRenderer->load(QString(":/generated/shapes/NodeClass.sdf"));

			contents.setWidth(538);
			contents.setHeight(473);
		/*	StatPoint pt;   */


			title_1 = factory.createLabel(1, 0.284387, 0.255814, "name", 0);
			title_1->setBackground(Qt::transparent);
			title_1->setScaling(false, false);
			title_1->setFlags(0);
			title_1->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title_1);


//extremely temporary fixing
		ports << portFactory.createPort(QPointF(0.994424, 0.54334), false, false, 538, 473, new NonTyped());
                mRenderer->setElementRepo(elementRepo);

		}

		~Node() {}

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
			title_1->setHtml(QString("<b>%1</b>").arg(repo->name()));

		}

		bool isNode() const
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
			return QVector<int>();
		}       

		int sizeOfChildrenForestalling() const
		{
			return 0;
		}

		bool minimizesToChildren() const
		{
			return false;
		}

		bool hasMovableChildren() const
		{
			return true;
		}

		bool maximizesChildren() const
		{
			return false;
		}

//temporary fixing
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

		bool isResizeable() const
		{
			return true;
		}

	/*	int getPenWidth() const
		{ 
			return 0; 
		}

		QColor getPenColor() const 
		{ 
			return QColor(); 
		}      */

//temporary fixing		
		QStringList fromPortTypes() const
		{
			return QStringList("NonTyped");
		}

//temporary fixing
		QStringList toPortTypes() const
		{
			return QStringList("NonTyped");
		}

//temporary fixing
		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}

		bool isDividable() const
		{
			return false;
		}

	private:
		qReal::SdfRendererInterface *mRenderer;
		//QStringList mBonusContextMenuFields;
		qReal::LabelInterface *title_1;

	};





	class Edge : public qReal::ElementImpl {
	public:
	/*	void init(QRectF &, QList<StatPoint> &, QList<StatLine> &,
				qReal::LabelFactoryInterface &, QList<qReal::LabelFactoryInterface*> &,
				SdfRendererInterface *, SdfRendererInterface *) {}
	*/
		void init(QRectF &, PortFactoryInterface const &, QList<PortInterface *> &,
											qReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,
											qReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}		

		void init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)
		{
			Q_UNUSED(factory)
			Q_UNUSED(titles)

		}

		virtual ~Edge() {}

		void paint(QPainter *, QRectF &){}
		bool isNode() const { return false; }
		bool isContainer() const { return false; }
		bool isSortingContainer() const { return false; }
                QVector<int> sizeOfForestalling() const { return QVector<int>(4, 0); }
		int sizeOfChildrenForestalling() const { return 0; }
		bool hasMovableChildren() const { return false; }
		bool minimizesToChildren() const { return false; }
	//	bool isClass() { return false; }
		bool maximizesChildren() const { return false; }
		bool isResizeable() const { return true; }
	//	int getPenWidth() const { return 1; }
	//	QColor getPenColor() const { return QColor(); }
		bool isDividable() const { return false; }


		qReal::ElementImpl *clone() { return NULL; }

		bool createChildrenFromMenu() const { return false; }

		QList<double> border () const
		{
			QList<double> list;
			list << 0 << 0 << 0 << 0;
			return list;
		}
		
		
		int getPenWidth() const { return 1; }

//temporary fixing		
		QColor getPenColor() const { return QColor(0,0,0); }

		
//temporary fixing
		QStringList fromPortTypes() const
		{
			QStringList result;
			result << "NonTyped";
			return result;
		}

//temporary fixing 
                QStringList toPortTypes() const
		{
			QStringList result;
			result << "NonTyped";
			return result;
		}

//temporary fixing		
		enums::linkShape::LinkShape shapeType() const
		{
			return enums::linkShape::square;
		}
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

			painter->setBrush(old);
		}

		void updateData(qReal::ElementRepoInterface *repo) const
		{
			Q_UNUSED(repo)

		}

	private:
//
	};





