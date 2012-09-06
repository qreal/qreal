#pragma once

#include "../groupBox.h"
#include "layoutToolPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class GroupBoxPropertyManager : public LayoutToolPropertyManager
{
public:
	GroupBoxPropertyManager(GroupBox *groupBox);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QGroupBox *mGroupBox;
	QList<QPair<QString, int> > mProperties;
	QStringList mAlignmentTypes;

	void initProperties();
	bool containsProperty(QString const &name);

	void addEnum(QString const &name);

	Qt::Alignment alignmentTypeToEnum(QString const &type);
	QString alignmentEnumToType(Qt::Alignment alignment);
};

}
}
