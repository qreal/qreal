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
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QString>

#include <qrkernel/ids.h>

namespace Ui {
	class GesturesWidget;
}

namespace qReal {
namespace gestures {

class GesturesWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GesturesWidget(QWidget *parent = 0);
	~GesturesWidget();

	void draw(const QString &paths);
	Id currentElement() const;
	void setElements(QList<QPair<QString, Id> > const &elements);

signals:
	void currentElementChanged();

private:
	Ui::GesturesWidget *mUi;
	int mCurrentPointNumber;
	static int coord(int previous, int next, int part);
};

}
}
