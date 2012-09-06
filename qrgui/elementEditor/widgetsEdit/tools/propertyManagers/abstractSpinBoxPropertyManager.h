#pragma once

#include "../abstractSpinBox.h"
#include "toolPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractSpinBoxPropertyManager : public ToolPropertyManager
{
public:
	AbstractSpinBoxPropertyManager(AbstractSpinBox *abstractSpinBox);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QAbstractSpinBox *mAbstractSpinBox;
	QList<QPair<QString, int> > mProperties;
	QStringList mCorrectionModeTypes;

	void initProperties();
	bool containsProperty(QString const &name);

	void addEnum(QString const &name);

	QAbstractSpinBox::CorrectionMode correctionModeTypeToEnum(QString const &type);
	QString correctionModeEnumToType(QAbstractSpinBox::CorrectionMode mode);
};

}
}
