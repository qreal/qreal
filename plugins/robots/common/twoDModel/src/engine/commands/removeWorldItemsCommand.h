/* Copyright 2015-2016 CyberTech Labs Ltd.
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

#include <qrgui/controller/commands/doNothingCommand.h>

namespace twoDModel {

namespace model {
class Model;
}

namespace commands {

/// Accepts a list of world items ids and removes them from model.
class RemoveWorldItemsCommand : public qReal::commands::DoNothingCommand
{
public:
	RemoveWorldItemsCommand(model::Model &model, const QStringList &itemsToDelete);
};

}
}
