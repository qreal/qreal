#pragma once

#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "object.h"
#include "graphicalPart.h"

namespace qrRepo {
namespace details {

class GraphicalObject : public Object
{
public:
	GraphicalObject(qReal::Id const &id, qReal::Id const &parent, qReal::Id const &logicalId);
	virtual ~GraphicalObject();

	qReal::Id logicalId() const;
	virtual bool isLogicalObject() const;

	virtual QDomElement serialize(QDomDocument &document) const;

	void createGraphicalPart(int index);
	QVariant graphicalPartProperty(int index, QString const &name) const;
	void setGraphicalPartProperty(int index, QString const &name, QVariant const &value);

protected:
	virtual Object *createClone() const;

private:
	qReal::Id mLogicalId;
	QHash<int, GraphicalPart *> mGraphicalParts;  // Has ownership.
};

}

}
