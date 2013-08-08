#include "graphicalPart.h"

#include "../../../qrkernel/exception/exception.h"

using namespace qrRepo::details;
using namespace qReal;

GraphicalPart::GraphicalPart()
{
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
