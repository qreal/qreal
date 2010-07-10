#pragma once

#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include "scene.h"
#include "item.h"

namespace Ui {
	class ShapeEdit;
}

class ShapeEdit : public QWidget {
	Q_OBJECT
public:
	explicit ShapeEdit(QWidget *parent = NULL);
	~ShapeEdit();

protected:
	void changeEvent(QEvent *e);

private slots:
	void saveToXml();

private:
	Scene *mScene;
	QGraphicsItemGroup mItemGroup;
	QDomDocument mDocument;
	QPointF mTopLeftPicture;
	Ui::ShapeEdit *mUi;

	void exportToXml(QString const &fileName);
	QList<QDomElement> generateGraphics();
	QDomElement generateLine(Line *item);
	QDomElement generateEllipse(Ellipse *item);
	QDomElement generateArch(Arch *item);
	QDomElement generateRectangle(Rectangle *item);
	QDomElement generateText(Text *item);
	QDomElement generatePointPort(PointPort* item);
	QDomElement generateLinePort(LinePort* item);
	QRectF sceneBoundingRectCoord(Item *item);

	QDomElement setPenBrush(QString const &domName, QPen const &pen, QBrush const &brush);
	static void setXandY(QDomElement& dom, QRectF const &rect);
};
