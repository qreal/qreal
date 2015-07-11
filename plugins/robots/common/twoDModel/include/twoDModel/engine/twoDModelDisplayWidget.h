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

#include "twoDModel/engine/twoDModelDisplayInterface.h"
#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {
namespace engine {

/// Base class for all widgets that emulate display`s behaviour. Implements 'clear' function.
class TWO_D_MODEL_EXPORT TwoDModelDisplayWidget : public QWidget, public TwoDModelDisplayInterface
{
	Q_OBJECT

public:
	explicit TwoDModelDisplayWidget(QWidget *parent = nullptr);

	void setPainter(qReal::ui::PainterInterface *painter) override;
	void reset() override;

private:
	qReal::ui::PainterInterface *mPainter;
};

}
}
