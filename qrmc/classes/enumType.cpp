#include "enumType.h"
#include "../utils/nameNormalizer.h"

using namespace qReal;
using namespace qrmc;

EnumType::EnumType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id) : NonGraphicType(diagram, api, id)
{
}

bool EnumType::init(const QString &context)
{
	Type::init(context);
	IdList children = mApi->children(mId);
	foreach(Id child, children) {
		if (!mApi->isLogicalElement(child))
			continue;
		if (child.element() == metaEntityValue) {
			//mValues << mApi->stringProperty(child, "valueName");
			QString name = mApi->stringProperty(child, "valueName");
			QString displayedName = mApi->stringProperty(child,"displayedName");
			mValues[name] = displayedName;
		}
	}

	return true;
}

Type* EnumType::clone() const
{
	EnumType *result = new EnumType(nullptr, mApi, mId);
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
	//QString enums;
	QString line = lineTemplate;
	QString lineForWright = "";
//	foreach(QString value, mValues) {
//		enums += "<< \"" + value + "\" ";
//	}
//	line.replace(enumsListTag, enums).replace(elementNameTag, name());
	for (const QString &value : mValues.keys()) {
		//line.replace(enumsListTag, mValues[value]).replace(dispNameTag, value).replace(elementNameTag, name());
//{ qMakePair(QString("@@DispName@@"), tr("@@EnumValuesList@@")) };


		if (!lineForWright.isEmpty()) {
			lineForWright += ", qMakePair(QString(\"" + value + "\"), tr(\"" + mValues[value] + "\"))";
		}
		else {
			lineForWright = "qMakePair(QString(\"" + value + "\"), tr(\"" + mValues[value] + "\"))";
		}
		//line.replace(qMakeLineTag, lineForWright).replace(elementNameTag, name());
	}
	line.replace(qMakeLineTag, lineForWright).replace(elementNameTag, name());

	return line;
}
