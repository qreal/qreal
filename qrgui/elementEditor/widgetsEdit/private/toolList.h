#pragma once

#include <QtGui/QListWidget>

namespace Ui
{
namespace WidgetsEdit
{

class ToolList : public QListWidget
{
	Q_OBJECT

public:
	ToolList(QWidget *parent = 0);

protected:
	virtual void mousePressEvent(QMouseEvent *event);

private:
	void loadItems();

};

}
}
