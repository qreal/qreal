/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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
#include <QtWidgets/QLabel>

namespace qReal {
namespace gui {

/// Nice looking bubbling messages. Shown upon the given widget for the half of the given time, then fades away.
/// HTML may be passed there. Clicking disables message before the lifetime goes out.
class HintReporter : public QLabel
{
	Q_OBJECT

public:
	HintReporter(QWidget *parent, const QString &message, int lifeTime);

private slots:
	void disappear();

private:
	void mouseReleaseEvent(QMouseEvent *event) override;

	int mDuration;
};

}
}
