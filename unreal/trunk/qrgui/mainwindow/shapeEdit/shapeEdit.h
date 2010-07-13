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
signals:
	void shapeSaved(QString const &shape);
protected:
	void changeEvent(QEvent *e);

private slots:
	void saveToXml();
	void save();

private:
	Scene *mScene;
	QGraphicsItemGroup mItemGroup;
	QDomDocument mDocument;
	QPointF mTopLeftPicture;
	Ui::ShapeEdit *mUi;

	void generateDom();
	void exportToXml(QString const &fileName);
	QList<QDomElement> generateGraphics();
};
