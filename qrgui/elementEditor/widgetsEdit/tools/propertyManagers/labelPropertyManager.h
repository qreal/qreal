#pragma once

#include "../label.h"
#include "framePropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class LabelPropertyManager : public FramePropertyManager
{
public:
	LabelPropertyManager(Label *label);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QLabel *mLabel;
	QList<QPair<QString, int> > mProperties;
	QStringList mAlignmentTypes;
	QStringList mTextFormatTypes;

	void initProperties();
	bool containsProperty(QString const &name);

	void addEnum(QString const &name);

	Qt::Alignment alignmentTypeToEnum(QString const &type);
	QString alignmentEnumToType(Qt::Alignment alignment);
	Qt::TextFormat textFormatTypeToEnum(QString const &type);
	QString textFormatEnumToType(Qt::TextFormat format);
};

}
}
