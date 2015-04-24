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

#include "rotater.h"

namespace graphicsUtils {

class Rotater;

class QRUTILS_EXPORT RotateItem : public AbstractItem
{
public:
	explicit RotateItem(QGraphicsItem *parent = 0);

	/// Can be overrided to determine rotater`s position: it will be placed to the center of rect`s right side.
	virtual QRectF rect() const;

	/// Returns a reference to a rotater graphics item.
	Rotater &rotater();

protected:
	/// Must be called in subclass to initialize rotater position and so on.
	/// Can`t be called here in constructor because it uses virtual methods.
	void init();

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	bool theOnlySelectedRotateItem(bool thisSelected) const;


private:
	Rotater mRotater;
};

}
