#pragma once

#include "../checkBox.h"
#include "buttonPropertyManagerBase.h"

namespace Ui
{
namespace WidgetsEdit
{

class CheckBoxPropertyManager : public ButtonPropertyManagerBase
{
public:
	CheckBoxPropertyManager(CheckBox *checkBox);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QCheckBox *mCheckBox;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);

};

}
}
