/* Copyright 2017 CyberTech Labs Ltd.
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

#include <qrutils/utilsDeclSpec.h>

class QLabel;
class QLineEdit;

namespace qReal {
namespace ui {

/// Picks some image from disk, saves into settings.
class QRUTILS_EXPORT ImagePicker : public QWidget
{
	Q_OBJECT

public:
	explicit ImagePicker(QWidget *parent = nullptr);

	/// Sets parameters of the image picker.
	void configure(const QString &settingsKey, const QString &title);

	/// Saves picked location into settings.
	void save() const;

	/// Restores last picked value.
	void restore();

private slots:
	void pick();

private:
	QString mSettingsKey;
	QLabel *mLabel;
	QLineEdit *mPathEditor;
};

}
}
