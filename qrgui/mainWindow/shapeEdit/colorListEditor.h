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

#include <QtWidgets/QComboBox>

class QColor;
class QWidget;

class ColorListEditor : public QComboBox
{
	Q_OBJECT
public:
	ColorListEditor(QWidget *widget = 0);

public:
	QColor color() const;
	void setColor(QColor c);

private:
	QColor mColor;
	void populateList();
};
