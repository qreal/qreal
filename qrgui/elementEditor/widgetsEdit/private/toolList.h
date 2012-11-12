#pragma once

#include <QtGui/QListWidget>

namespace qReal
{
namespace widgetsEdit
{

class ToolList : public QListWidget
{
	Q_OBJECT

public:
	ToolList(QWidget *parent = 0);

signals:
	void keyPressed(QKeyEvent *event);

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

private:
	void loadItems();

};

}
}
