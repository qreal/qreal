/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QMap>

#include "mainWindow/shapeEdit/item.h"

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
		PropertyInfo(Type t, const QStringList &v);
	};

	explicit VisibilityConditionsDialog(QMap<QString, PropertyInfo> const &enumValues
			, QList<Item *> const &items, QWidget *parent = 0);
	~VisibilityConditionsDialog();

private slots:
	void changeProperty(const QString &propertyName);
	void okClicked();

private:
	void setWidgetValues();
	bool areValuesEqual() const;
	void changeOperators(Type type);

	Ui::VisibilityConditionsDialog *ui;
	QMap<QString, PropertyInfo> mProperties;
	QList<Item *> mItems;
};
