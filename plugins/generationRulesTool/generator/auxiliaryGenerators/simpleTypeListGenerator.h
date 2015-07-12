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

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/identifier.h"

namespace generationRules {
namespace generator {

/// Class that generates list of ids for simple type list node.
class SimpleTypeListGenerator
{
public:
	/// Returns list of ids for given list node.
	/// @param simpleIdentifierNode - simple identifier node.
	/// @param logicalModelInterface - information about model.
	static qReal::IdList generatedList(const QSharedPointer<simpleParser::ast::Identifier> &simpleIdentifierNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			);
};

}
}
