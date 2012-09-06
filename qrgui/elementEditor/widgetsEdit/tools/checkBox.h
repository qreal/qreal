#pragma once

#include <QtGui/QCheckBox>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class CheckBox : public AbstractButton
{
	Q_OBJECT

public:
	CheckBox(ToolController *controller);

private slots:
	void toggled(bool checked);

private:
	QCheckBox *mCheckBox;

};

}
}
