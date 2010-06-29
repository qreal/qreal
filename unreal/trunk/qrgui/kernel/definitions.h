#pragma once

#include <QString>
#include <QList>
#include "ids.h"

namespace qReal {

	const Id ROOT_ID = Id("ROOT_ID","ROOT_ID","ROOT_ID","ROOT_ID");

	const char ID_PATH_DIVIDER = '#';

	const QString DEFAULT_MIME_TYPE = "application/x-real-uml-data";

	namespace roles {
		// roles that are heavily used by front-end. should be present in all entities
		enum {
			idRole = Qt::UserRole + 1, // element's ID. e.g. qReal::Id
			positionRole, // element's position within current context (model knows which one it is)
			configurationRole, // element's configuration (e.g. shape, size) within current context
			fromRole,  // for edges it's an ID of a node, which this edge starts from. is ignored for nodes
			toRole, // for edges it's an ID of a node, which this edge ends on. is ignored for nodes
			fromPortRole, // for edges it's a port ID, which this edge starts from. is ignored for nodes  
			toPortRole, // for edges it's a port ID, which this edge ends on. is ignored for nodes 
			customPropertiesBeginRole, // generated roles start from this one
			// also, `name' is also among the mandatory attributes. it's handled by Qt::EditRole and Qt::DisplayRole
		};
	}
}
