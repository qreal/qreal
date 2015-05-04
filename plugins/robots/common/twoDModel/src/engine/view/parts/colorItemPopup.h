/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

class QSpinBox;

namespace graphicsUtils {
class ColorListEditor;
}

namespace twoDModel {

namespace items {
class ColorFieldItem;
}

namespace view {

/// A popup for configuring ColorFieldItem on 2D model scene.
/// Contains item`s color picker and spin box for setting item`s thickness.
class ColorItemPopup : public graphicsUtils::ItemPopup
{
public:
	explicit ColorItemPopup(graphicsUtils::AbstractScene &scene, QWidget *parent = 0);
	~ColorItemPopup();

private:
	bool suits(QGraphicsItem *item) override;
	void attachTo(const QList<QGraphicsItem *> &items);

	void initWidget();
	QWidget *initColorPicker();
	QWidget *initSpinBox();

	graphicsUtils::ColorListEditor *mColorPicker;  // Takes ownership
	QSpinBox *mSpinBox;  // Takes ownership
};

}
}
