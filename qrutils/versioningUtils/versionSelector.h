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

#include "qrutils/utilsDeclSpec.h"

namespace Ui {
class VersionSelector;
}

namespace qReal {
namespace versioning {
namespace ui {

/// @brief Dialog providing posibility to select revision.
class QRUTILS_EXPORT VersionSelector : public QDialog
{
	Q_OBJECT

public:
	explicit VersionSelector(QWidget *parent = 0);
	~VersionSelector();

	/// Returns version number of -1 if HEAD revision specified
	int revision() const;

private slots:
	void onSelectionChanged();

private:
	Ui::VersionSelector *mUi;
};

}
}
}
