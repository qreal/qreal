#pragma once

#include <QWidget>
#include <QComboBox>

#include "visibilityConditionsDialog.h"

class VisibilityValueWidget : public QWidget
{
	Q_OBJECT
public:
	explicit VisibilityValueWidget(QWidget *parent = 0);
	~VisibilityValueWidget();

	void setPropertyInfo(VisibilityConditionsDialog::PropertyInfo const &info);
	void setValue(QString const &value);
	QString value() const;
signals:
	
public slots:
	
private:
	void makeWidget(VisibilityConditionsDialog::Type type);
	void addValues(QStringList const &values);

	QWidget *mWidget;
};
