/* Copyright 2016 Dmitry Mordvinov
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

#include <qrutils/graphicsUtils/itemPopup.h>

class QCheckBox;
class QPushButton;

namespace twoDModel {
namespace view {

/// A popup for configuring ImageFieldItem on 2D model scene.
/// Contains item`s path picker and check box for setting image`s memorizing.
class ImageItemPopup : public graphicsUtils::ItemPopup
{
	Q_OBJECT

public:
	ImageItemPopup(graphicsUtils::AbstractScene &scene, QWidget *parent = nullptr);
	~ImageItemPopup();

	/// Returns true if user selected to embed image into save last time.
	bool isMemorized() const;

	/// Returns last path to image entered by user.
	QString lastPath() const;

signals:
	/// Emitted when keeping image externally has changed.
	void memorizingChanged(bool isMemorized);

	/// Emitted when path to image picked by user has changed.
	void pathChanged(const QString &path);

private slots:
	void updateMemorizationToolTip();

private:
	bool suits(QGraphicsItem *item) override;
	bool attachTo(const QList<QGraphicsItem *> &items) override;

	void initWidget();
	QWidget *initMemorizationPicker();
	QWidget *initPathPicker();

	QPushButton *mPathPicker;  // Takes ownership
	QCheckBox *mMemorizationPicker;  // Takes ownership
	bool mLastIsMemorized;
	QString mLastPath;
};

}
}
