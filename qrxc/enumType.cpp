#include "enumType.h"

#include <QtCore/QStringList>

#include <qrutils/outFile.h>

#include "nameNormalizer.h"

bool EnumType::init(QDomElement const &element, QString const &context)
{
	if (!NonGraphicType::init(element, context)) {
		return false;
	}

	for (QDomElement valueElement = element.firstChildElement("value")
		; !valueElement.isNull()
		; valueElement = valueElement.nextSiblingElement("value"))
	{
		QString const name = valueElement.attribute("name");
		QString displayedName = valueElement.attribute("displayedName");
		if (displayedName.isEmpty()) {
			displayedName = name;
		}

		mValues[name] = displayedName;
	}

	return true;
}

Type* EnumType::clone() const
{
	EnumType *result = new EnumType();
	Type::copyFields(result);
	result->mValues = mValues;
	return result;
}

bool EnumType::generateEnumValues(utils::OutFile &out, bool isNotFirst)
{
	if (mValues.isEmpty()) {
		return false;
	}

	generateOneCase(out, isNotFirst);

	out() << "\t\treturn { ";
	QStringList pairs;
	for (QString const &name : mValues.keys()) {
		pairs << QString("qMakePair(QString(\"%1\"), tr(\"%2\"))").arg(name, mValues[name]);
	}

	out() << pairs.join(", ");
	out() << " };\n";
	return true;
}

void EnumType::generateOneCase(utils::OutFile &out, bool isNotFirst) const
{
	//QString name = NameNormalizer::normalize(qualifiedName());

	if (!isNotFirst) {
		out() << "\tif (name == \"" << NameNormalizer::normalize(name()) << "\")\n";
	} else {
		out() << "\telse if (name == \"" << NameNormalizer::normalize(name()) << "\")\n";
	}
}

void EnumType::generatePropertyTypes(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void EnumType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void EnumType::generateMouseGesturesMap(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void EnumType::generateExplosionsMap(utils::OutFile &out)
{
	Q_UNUSED(out)
}
