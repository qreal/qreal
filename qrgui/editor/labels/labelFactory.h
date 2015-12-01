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

#include "plugins/editorPluginInterface/labelInterface.h"
#include "plugins/editorPluginInterface/labelFactoryInterface.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {
namespace gui {
namespace editor {

class LabelFactory : public LabelFactoryInterface
{
public:
	LabelFactory(models::GraphicalModelAssistApi &graphicalModelAssistApi, const Id &elementId);
	LabelInterface *createLabel(int index, qreal x, qreal y, const QString &text, qreal rotation);
	LabelInterface *createLabel(int index, qreal x, qreal y, const QString &binding, bool readOnly, qreal rotation);

private:
	models::GraphicalModelAssistApi &mGraphicalModelAssistApi;
	const Id mId;
};

}
}
}
