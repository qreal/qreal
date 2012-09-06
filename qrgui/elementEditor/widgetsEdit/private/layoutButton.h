#pragma once

#include <QtGui/QPushButton>
#include <QtGui/QIcon>

#include "../tools/layoutHelpers/layoutTypes.h"

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
