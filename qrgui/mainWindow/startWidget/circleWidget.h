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

namespace qReal {

/// Represents a circle with the border and some icon inside. Border color can be specified
/// with palette foreground color (manually or through brand manager stylesheet)
class CircleWidget : public QWidget
{
public:
	CircleWidget(const QSize &size, const QString &icon, QWidget *parent = 0);

protected:
	virtual void paintEvent(QPaintEvent *) override;
};

}
