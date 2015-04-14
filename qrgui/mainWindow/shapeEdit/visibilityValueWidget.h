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
