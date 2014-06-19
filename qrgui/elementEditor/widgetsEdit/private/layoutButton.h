#pragma once

#include <QtWidgets/QPushButton>
#include <QtGui/QIcon>

#include <qrgui/elementEditor/widgetsEdit/tools/layoutHelpers/layoutTypes.h>

namespace qReal
{
namespace widgetsEdit
{

class LayoutButton : public QPushButton
{
	Q_OBJECT

public:
	LayoutButton(QString const &text, QIcon const &icon
		, LayoutType const type, QWidget *parent = 0);

signals:
	void clicked(LayoutType type);

private slots:
	void onClicked();

private:
	LayoutType mType;

};

}
}
