#pragma once

#include "../radioButton.h"
#include "buttonPropertyManagerBase.h"

namespace qReal
{
namespace widgetsEdit
{

class RadioButtonPropertyManager : public ButtonPropertyManagerBase
{
public:
	RadioButtonPropertyManager(RadioButton *radioButton);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QRadioButton *mRadioButton;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);
};

}
}
