#include "graphicalPart.h"

#include "../../../qrkernel/exception/exception.h"
#include "../valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

GraphicalPart::GraphicalPart()
{
}

GraphicalPart::GraphicalPart(QDomElement const &element)
{
	ValuesSerializer::deserializeNamedVariantsMap(mProperties, element);
}

QVariant GraphicalPart::property(QString const &name) const
{
	if (!mProperties.contains(name)) {
		throw Exception("Trying to get non-existing graphical part property " + name);
	}

	return mProperties[name];
}

void GraphicalPart::setProperty(QString const &name, const QVariant &value)
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
