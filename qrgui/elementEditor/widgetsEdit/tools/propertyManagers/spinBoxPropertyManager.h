#pragma once

#include "../spinBox.h"
#include "abstractSpinBoxPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class SpinBoxPropertyManager : public AbstractSpinBoxPropertyManager
{
public:
	SpinBoxPropertyManager(SpinBox *spinBox);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QSpinBox *mSpinBox;
	QList<QPair<QString, int> > mProperties;

	void initProperties();
	bool containsProperty(QString const &name);

};

}
}
