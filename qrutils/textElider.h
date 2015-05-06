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

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Provides text auto-trimming on a given widget.
/// The given widget must have setText(QString) slot, i.e. QLabel is ok
class QRUTILS_EXPORT TextElider: public QObject
{
	Q_OBJECT

public:
	explicit TextElider(QWidget *widget, Qt::TextElideMode mode = Qt::ElideRight);

signals:
	/// Emitted after each time after text recalculation
	void textChanged(const QString &text);

public slots:
	void setText(const QString &text);

private:
	QWidget *mWidget;  // Doesn`t take ownership
	Qt::TextElideMode mMode;
};

}
