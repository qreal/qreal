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

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>

namespace qReal {

class HintInterface
{
public:
	///Send message as HintReporter.
	Q_INVOKABLE void addHint(QWidget *parent, QString const &message, int const duration);

	///Draw arrow near target.
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle);
};

}
