#include "portType.h"

#include <qrutils/outFile.h>

bool PortType::init(const QDomElement &element, const QString &context)
{
	return NonGraphicType::init(element, context);
}

Type * PortType::clone() const
{
	PortType *result = new PortType();
	Type::copyFields(result);
	return result;
}

void PortType::generateCode(utils::OutFile &out)
{
	const QString name = Type::name();

	out() << "\tclass " << name << " : public PortImpl\n"
	<< "\t{\n"
	<< "\tpublic:\n"
	<< "\t\t~" << name << "() {}\n"
	<< "\t\tvirtual QString type() const { return \"" << name << "\"; }\n"
	<< "\t};\n\n";
}

bool PortType::generateEnumValues(utils::OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return true;
}

void PortType::generatePropertyTypes(utils::OutFile &out)
{
	Q_UNUSED(out)
}

void PortType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out)
}
