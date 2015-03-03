#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>

#include "mainWindow/shapeEdit/visibilityConditionsDialog.h"

class VisibilityValueWidget : public QWidget
{
	Q_OBJECT
public:
	explicit VisibilityValueWidget(QWidget *parent = 0);
	~VisibilityValueWidget();

	void setPropertyInfo(const VisibilityConditionsDialog::PropertyInfo &info);
	void setValue(const QString &value);
	QString value() const;
signals:

public slots:

private:
	void makeWidget(VisibilityConditionsDialog::Type type);
	void addValues(const QStringList &values);

	QWidget *mWidget;
};
