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

	Q_PROPERTY(bool checked READ isChecked WRITE setChecked USER true DESIGNABLE true)
	Q_PROPERTY(QString bindedPropertyName READ bindedPropertyName WRITE setBindedPropertyName USER true DESIGNABLE true)

public:
	CheckBox(ToolController *controller);

private slots:
	void toggled(bool isChecked);

private:
	bool isChecked() const;
	QString bindedPropertyName() const;

	void setChecked(bool checked);
	void setBindedPropertyName(QString const &name);

	QCheckBox *mCheckBox;
	QString mBindedPropertyName;
};

}
}
