#pragma once

#include <QtGui/QRadioButton>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class RadioButton : public AbstractButton
{
	Q_OBJECT

public:
	RadioButton(ToolController *controller);

private slots:
	void toggled(bool checked);

private:
	QRadioButton *mRadioButton;

};

}
}
