#pragma once

#include "object.h"

namespace qrRepo {
namespace details {

/// Represents logical object in repository.
class LogicalObject : public Object
{
public:
	/// Constructor.
	/// @param id - id of a new object.
	explicit LogicalObject(qReal::Id const &id);

	/// Deserializing constructor.
	/// @param element - root of XML DOM subtree with serialized object.
	explicit LogicalObject(QDomElement const &element);

	// Override.
	virtual bool isLogicalObject() const;

protected:
	// Override.
	virtual Object *createClone() const;

};

}
}
