#include "beep.h"

Beep::~Beep()
{}

void Beep::init(ElementTitleFactoryInterface &factory, QList<ElementTitleInterface*> &titles)
{
}

void Beep::init(QRectF &contents, QList<StatPoint> &pointPorts,
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
	title_1 = factory.createTitle(-1.8, 1.2, QString::fromUtf8("Р–РґР°С‚СЊ Р·Р°РІРµСЂС€РµРЅРёСЏ:"));
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

ElementImpl * Beep::clone()
{
	return NULL;
}

void Beep::paint(QPainter *painter, QRectF &contents)
{
	mRenderer->render(painter, contents);
}

Qt::PenStyle Beep::getPenStyle()
{
	return Qt::SolidLine;
}

int Beep::getPenWidth()
{
	return 1;
}

QColor Beep::getPenColor()
{
	return Qt::green;
}

void Beep::drawStartArrow(QPainter *) const
{}

void Beep::drawEndArrow(QPainter *) const
{}

bool Beep::hasPorts()
{
	return true;
}

void Beep::updateData(ElementRepoInterface *repo) const
{
	Q_UNUSED(repo);
	title_2->setHtml(QString("%1").arg(repo->logicalProperty("WaitForCompletion")).replace("\n", "<br>"));
}

bool Beep::isNode()
{
	return true;
}

bool Beep::isResizeable()
{
	return false;
}

bool Beep::isContainer()
{
	return false;
}

bool Beep::isSortingContainer()
{
	return false;
}

int Beep::sizeOfForestalling()
{
	return 0;
}

int Beep::sizeOfChildrenForestalling()
{
	return 0;
}

bool Beep::hasMovableChildren()
{
	return true;
}

bool Beep::minimizesToChildren()
{
	return false;
}

bool Beep::maximizesChildren()
{
	return false;
}

bool Beep::isPort()
{
	return false;
}

bool Beep::hasPin()
{
	return false;
}

QList<double> Beep::border()
{
	QList<double> list;
	list << 0 << 0 << 0 << 0;
	return list;
}

QStringList Beep::bonusContextMenuFields()
{
	return QStringList();
}
