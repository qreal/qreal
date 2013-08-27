#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QMap>

#include "mainwindow/shapeEdit/item.h"

namespace Ui {
class VisibilityConditionsDialog;
}

class VisibilityConditionsDialog : public QDialog
{
	Q_OBJECT

public:
	enum Type { Enum, Int, Bool, String };
	struct PropertyInfo
	{
		Type type;
		QStringList values;

		PropertyInfo() {}
		PropertyInfo(Type t, QStringList const &v);
	};

	explicit VisibilityConditionsDialog(QMap<QString, PropertyInfo> const &enumValues
			, QList<Item *> const &items, QWidget *parent = 0);
	~VisibilityConditionsDialog();

private slots:
	void changeProperty(QString const &propertyName);
	void okClicked();

private:
	void setWidgetValues();
	bool areValuesEqual() const;
	void changeOperators(Type type);

	Ui::VisibilityConditionsDialog *ui;
	QMap<QString, PropertyInfo> mProperties;
	QList<Item *> mItems;
};
