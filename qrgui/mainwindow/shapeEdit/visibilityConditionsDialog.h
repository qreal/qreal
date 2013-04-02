#pragma once

#include <QDialog>
#include <QMap>
#include "item.h"

namespace Ui {
class VisibilityConditionsDialog;
}

class VisibilityConditionsDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit VisibilityConditionsDialog(QMap<QString, QStringList> const &enumValues
			, QList<Item *> const &items, QWidget *parent = 0);
	~VisibilityConditionsDialog();

private slots:
	void changeValues(QString const &enumName);
	void okClicked();

private:
	void setIndices();
	bool areValuesEqual();

	Ui::VisibilityConditionsDialog *ui;
	QMap<QString, QStringList> mEnumValues;
	QList<Item *> mItems;
};
