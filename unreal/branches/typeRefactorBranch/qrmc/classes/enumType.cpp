#include "enumType.h"
#include "utils/nameNormalizer.h"

using namespace qReal;
using namespace qrmc;

EnumType::EnumType(Diagram *diagram, qrRepo::RepoApi *api, const qReal::NewType &type) : NonGraphicType(diagram, api, type)
{
}

bool EnumType::init(QString const &context)
{
	Type::init(context);
        TypeList children = mApi->children(mType);
        foreach(NewType child, children) {
		if (child.element() == metaEntityValue) {
			mValues << mApi->stringProperty(child, "valueName");
		}
	}

	return true;
}

Type* EnumType::clone() const
{
        EnumType *result = new EnumType(NULL, mApi, mType);
	Type::copyFields(result);
	result->mValues = mValues;
	return result;
}

void EnumType::print()
{
	qDebug() << "enum type" << mName;
}

QString EnumType::generateEnums(const QString &lineTemplate) const
{
	QString enums;
	QString line = lineTemplate;
	foreach(QString value, mValues) {
		enums += "<< \"" + value + "\" ";
	}
	line.replace(enumsListTag, enums).replace(elementNameTag, name());
	return line;
}
