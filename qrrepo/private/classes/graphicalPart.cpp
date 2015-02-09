#include "graphicalPart.h"

#include <qrkernel/exception/exception.h>

#include "qrrepo/private/valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

GraphicalPart::GraphicalPart()
{
}

GraphicalPart::GraphicalPart(const QDomElement &element)
{
	ValuesSerializer::deserializeNamedVariantsMap(mProperties, element);
}

QVariant GraphicalPart::property(const QString &name) const
{
	if (!mProperties.contains(name)) {
		throw Exception("Trying to get non-existing graphical part property " + name);
	}

	return mProperties[name];
}

void GraphicalPart::setProperty(const QString &name, const QVariant &value)
{
	mProperties.insert(name, value);
}

GraphicalPart *GraphicalPart::clone() const
{
	GraphicalPart * const result = new GraphicalPart();
	result->mProperties = this->mProperties;
	return result;
}

QDomElement GraphicalPart::serialize(int index, QDomDocument &document) const
{
	QDomElement result = ValuesSerializer::serializeNamedVariantsMap("graphicalPart", mProperties, document);
	result.setAttribute("index", index);
	return result;
}
