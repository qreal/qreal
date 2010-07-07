#include "shapeEdit.h"

#include "ui_shapeEdit.h"
#include "../../../utils/outFile.h"

#include <QtGui/QFileDialog>
#include <QtGui/QGraphicsItem>

using namespace utils;

ShapeEdit::ShapeEdit(QWidget *parent) :
		QWidget(parent),
		mUi(new Ui::ShapeEdit)
{
	mUi->setupUi(this);

	mScene = new Scene(this);
	mUi->graphicsView->setScene(mScene);

	connect(mUi->drawLineButton, SIGNAL(pressed()), mScene, SLOT(drawLine()));
	connect(mUi->drawEllipseButton, SIGNAL(pressed()), mScene, SLOT(drawEllipse()));
	connect(mUi->drawArcButton, SIGNAL(pressed()), mScene, SLOT(drawArc()));
	connect(mUi->drawRectButton, SIGNAL(pressed()), mScene, SLOT(drawRectangle()));
	connect(mUi->clearButton, SIGNAL(pressed()), mScene, SLOT(clearScene()));
	connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(saveToXml()));
}

ShapeEdit::~ShapeEdit()
{
	delete mUi;
}

void ShapeEdit::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void ShapeEdit::setXandY(QDomElement& dom, QRectF const &rect)
{
	dom.setAttribute("y1", rect.top());
	dom.setAttribute("x1", rect.left());
	dom.setAttribute("y2", rect.bottom());
	dom.setAttribute("x2", rect.right());
}

QDomElement ShapeEdit::setPenBrush(QString const &domName, QPen const &pen, QBrush const &brush)
{
	QDomElement dom = mDocument.createElement(domName);
	dom.setAttribute("fill", brush.color().name());

	if (brush.style() == Qt::NoBrush)
		dom.setAttribute("fill-style", "none");
	if (brush.style() == Qt::SolidPattern)
		dom.setAttribute("fill-style", "solid");

	dom.setAttribute("stroke", pen.color().name());

	dom.setAttribute("stroke-width", pen.width());

	QString penStyle;
	switch (pen.style()) {
	case Qt::SolidLine:
		penStyle = "solid";
		break;
	case Qt::DotLine:
		penStyle = "dot";
		break;
	case Qt::DashLine:
		penStyle = "dash";
		break;
	case Qt::DashDotLine:
		penStyle =  "dashdot";
		break;
	case Qt::DashDotDotLine:
		penStyle = "dashdotdot";
		break;
	case Qt::NoPen:
		penStyle = "none";
		break;
	default:
		break;
	}
	dom.setAttribute("stroke-style", penStyle);

	return dom;
}

QRectF ShapeEdit::sceneBoundingRectCoord(Item *item)
{
	qreal const x1 = item->scenePos().x() + item->boundingRect().x() - mTopLeftPicture.x();
	qreal const y1 = item->scenePos().y() + item->boundingRect().y() - mTopLeftPicture.y();
	return QRectF(x1, y1, item->boundingRect().width(), item->boundingRect().height());
}

QDomElement ShapeEdit::generateLine(Line* item)
{
	qreal const x1 = item->scenePos().x() + item->line().x1() - mTopLeftPicture.x();
	qreal const y1 = item->scenePos().y() + item->line().y1() - mTopLeftPicture.y();
	qreal const x2 = item->scenePos().x() + item->line().x2() - mTopLeftPicture.x();
	qreal const y2 = item->scenePos().y() + item->line().y2() - mTopLeftPicture.y();

	QDomElement line = setPenBrush("line", item->pen(), item->pen().brush());
	setXandY(line, QRectF(x1, y1, x2 - x1, y2 - y1));

	return line;
}

QDomElement ShapeEdit::generateEllipse(Ellipse* item)
{
	QDomElement ellipse = setPenBrush("ellipse", item->pen(), item->brush());
	setXandY(ellipse, sceneBoundingRectCoord(item));

	return ellipse;
}

QDomElement ShapeEdit::generateArch(Arch* item)
{
	QDomElement arch = mDocument.createElement("arch");
	arch.setAttribute("startAngle", item->startAngle());
	arch.setAttribute("spanAngle", item->spanAngle());
	setXandY(arch, sceneBoundingRectCoord(item));

	return arch;
}

QDomElement ShapeEdit::generateRectangle(Rectangle* item)
{
	QDomElement rectangle = setPenBrush("rectangle", item->pen(), item->brush());
	setXandY(rectangle, sceneBoundingRectCoord(item));

	return rectangle;
}

QDomElement ShapeEdit::generatePicture()
{
	QDomElement picture = mDocument.createElement("picture");

	mScene->removeItem(mScene->mEmptyRect);
	mTopLeftPicture = mScene->itemsBoundingRect().topLeft();

	QList<QGraphicsItem *> list = mScene->items();
	foreach (QGraphicsItem *graphicsItem, list) {

		Line* lineItem = dynamic_cast<Line*>(graphicsItem);
		Ellipse* ellipseItem = dynamic_cast<Ellipse*>(graphicsItem);
		Arch* archItem = dynamic_cast<Arch*>(graphicsItem);
		Rectangle* rectangleItem = dynamic_cast<Rectangle*>(graphicsItem);

		if (lineItem != NULL) {
			QDomElement line = generateLine(lineItem);
			picture.appendChild(line);
		}  else if (archItem != NULL) {
			QDomElement arch = generateArch(archItem);
			picture.appendChild(arch);
		} else if (ellipseItem != NULL) {
			QDomElement ellipse = generateEllipse(ellipseItem);
			picture.appendChild(ellipse);
		} else if (rectangleItem != NULL) {
			QDomElement rectangle = generateRectangle(rectangleItem);
			picture.appendChild(rectangle);
		} else
			continue;
	}

	picture.setAttribute("sizex", static_cast<int>(mScene->itemsBoundingRect().width()));
	picture.setAttribute("sizey", static_cast<int>(mScene->itemsBoundingRect().height()));

	return picture;
}

void ShapeEdit::exportToXml(QString const &fileName)
{
	OutFile file(fileName);
	QDomElement graphics = mDocument.createElement("graphics");
	mDocument.appendChild(graphics);

	QDomElement picture = generatePicture();
	graphics.appendChild(picture);

	file() << "<?xml version='1.0' encoding='utf-8'?>\n";
	file() << mDocument.toString(4);
}

void ShapeEdit::saveToXml()
{
	mDocument.clear();
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return;
	exportToXml(fileName);
}
