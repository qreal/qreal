#pragma once

#include <QtGui/QWidget>

#include "../../../qrutils/navigationUtils/navigationPageInterface.h"

namespace qReal
{
namespace elementEdit
{

class StartPage : public QWidget, public navigation::NavigationPageInterface
{
	Q_OBJECT

public:
	StartPage();

signals:
	void shapeSelected();
	void widgetSelected();

private slots:
	void onShapeSelected();
	void onWidgetSelected();
};

}
}
