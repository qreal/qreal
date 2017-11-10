/* Copyright 2017 CyberTech Labs Ltd
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

#include <QtWidgets/QMessageBox>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Provides a dialog(QMessageBox) where buttons are presented with translations
class QRUTILS_EXPORT QRealMessageBox : public QObject
{
	Q_OBJECT

public:
	static QMessageBox::StandardButton question(
		QWidget *parent
		, const QString &title
		, const QString &text
		, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No));
};

}
