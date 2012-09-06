#pragma once

#include "../lineEdit.h"
#include "toolPropertyManager.h"

namespace qReal
{
namespace widgetsEdit
{

class LineEditPropertyManager : public ToolPropertyManager
{
public:
	LineEditPropertyManager(LineEdit *lineEdit);

protected:
	virtual void onPropertyChanged(const QString &name, const QVariant &value);
	virtual QVariant propertyValue(const QString &name);

private:
	QLineEdit *mLineEdit;
	QList<QPair<QString, int> > mProperties;
	QStringList mAlignmentTypes;
	QStringList mEchoModeTypes;

	void initProperties();
	bool containsProperty(QString const &name);

	void addEnum(QString const &name);

	Qt::Alignment alignmentTypeToEnum(QString const &type);
	QString alignmentEnumToType(Qt::Alignment alignment);
	QLineEdit::EchoMode echoModeTypeToEnum(QString const &type);
	QString echoModeEnumToType(QLineEdit::EchoMode echoMode);
};

}
}
