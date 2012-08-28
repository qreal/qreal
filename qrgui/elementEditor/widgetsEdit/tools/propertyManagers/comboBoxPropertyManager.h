#pragma once

#include "../comboBox.h"
#include "toolPropertyManager.h"

namespace Ui
{
namespace WidgetsEdit
{

class ComboBoxPropertyManager : public ToolPropertyManager
{
public:
	ComboBoxPropertyManager(ComboBox *comboBox);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QComboBox *mComboBox;
	QList<QPair<QString, int> > mProperties;
	QStringList mSizeAdjustPolicyTypes;

	void initProperties();
	bool containsProperty(QString const &name);

	void addEnum(QString const &name);

	QComboBox::SizeAdjustPolicy sizeAdjustPolicyTypeToEnum(QString const &type);
	QString sizeAdjustPolicyEnumToType(QComboBox::SizeAdjustPolicy policy);
};

}
}
