#pragma once

#include <QWidget>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include "scene.h"

namespace Ui {
	class ShapeEdit;
}

class ShapeEdit : public QWidget {
	Q_OBJECT
public:
	ShapeEdit(QWidget *parent = 0);
	~ShapeEdit();

private slots:
	void saveToXml();
protected:
	void changeEvent(QEvent *e);

private:
	Scene *mScene;
	qreal mMinPictureX;
	qreal mMaxPictureX;
	qreal mMinPictureY;
	qreal mMaxPictureY;
	QGraphicsItemGroup mItemGroup;
	QDomDocument mDocument;
	Ui::ShapeEdit *ui;
	void exportToXml(QString const &fileName);
	QDomElement generatePicture();
	QDomElement generateLine(QGraphicsLineItem* item);
	QDomElement generateEllipse(QGraphicsEllipseItem* item);
	QDomElement generateArch(Arch* item);
	QDomElement setPenBrush(QString const &domName, QPen pen);
	void setXandY(QDomElement& dom, qreal x1, qreal y1, qreal x2, qreal y2);
};
