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
#include <QtCore/QList>
#include "ids.h"

namespace qReal {

namespace roles {

/// Roles that are heavily used by front-end. Should be present in all entities.
enum {
	idRole = Qt::UserRole + 1 // element's ID. e.g. qReal::Id
	, logicalIdRole // For elements of graphical model it is an Id of corresponding logical element,
					// if any. Otherwise it's Id().
	, positionRole // element's position within current context (model knows which one it is)
	, configurationRole // element's configuration (e.g. shape, size) within current context
	, fromRole  // for edges it's an ID of a node, which this edge starts from. is ignored for nodes
	, toRole // for edges it's an ID of a node, which this edge ends on. is ignored for nodes
	, fromPortRole // for edges it's a port ID, which this edge starts from. is ignored for nodes
	, toPortRole // for edges it's a port ID, which this edge ends on. is ignored for nodes
	, customPropertiesBeginRole // generated roles start from this one
	// also, `name' is also among the mandatory attributes. it's handled by Qt::EditRole and Qt::DisplayRole
};
}
}
