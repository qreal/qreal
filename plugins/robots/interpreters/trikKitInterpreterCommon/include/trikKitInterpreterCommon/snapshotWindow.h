/* Copyright 2016 Evgeny Sergeev
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

#include <QDockWidget>
#include <QtWidgets/QLabel>
#include <QtCore/QScopedPointer>

namespace trik {

/// Window that displays PNG snapshot received from real robot.
class SnapshotWindow : public QDockWidget
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param title - title of top-level widget.
	/// @param parent - parent of a window as in Qt widget parent/child relations.
	SnapshotWindow(const QString &title, QWidget *parent = 0);

	/// @param snapshot - PNG image to be displayed.
	/// @param deleteImage - determine whether snapshot shall be deleted after usage.
	void setImage(QByteArray *snapshot, bool deleteImage);

private:
	QScopedPointer<QLabel> mLabel;
};

}
