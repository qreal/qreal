#include "label.h"
#include "../metaCompiler.h"

#include <QDebug>

using namespace qrmc;

bool Label::init(QDomElement const &element, int index, bool nodeLabel, int width, int height)
{
	initCoordinate(mX, element.attribute("x", "0"), width);
	initCoordinate(mY, element.attribute("y", "0"), height);

	scalingX = mX.isScalable() ? "true" : "false";
	scalingY = mY.isScalable() ? "true" : "false";

	mCenter = element.attribute("center", "false");
	mText = element.attribute("text");
	mTextBinded = element.attribute("textBinded");
	mReadOnly = element.attribute("readOnly", "false");
	mIndex = index;
	mBackground = element.attribute("background", nodeLabel ? "transparent" : "white");
	if ((mText.isEmpty() && mTextBinded.isEmpty()) || (mReadOnly != "true" && mReadOnly != "false")) {
		qDebug() << "ERROR: can't parse label";
		return false;
	}
	return true;
}

void Label::initCoordinate(ScalableCoordinate &field, QString coordinate, int maxValue)
{
	if (coordinate.endsWith("a"))
	{
		coordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / maxValue, maxValue, true);
	}
	else if (coordinate.endsWith("%"))
	{
		coordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / 100, 100, false);
	}
	else
	{
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / maxValue, maxValue, false);
	}
}

QString Label::generateInit(MetaCompiler *compiler, bool isNode) const
{
	QString result = isNode ? compiler->getTemplateUtils(nodeInitTag) : compiler->getTemplateUtils(edgeInitTag);
	QString name = mText.isEmpty() ? mTextBinded : mText;

	result.replace(labelXTag, mX.toString(false))
			.replace(labelYTag, mY.toString(false))
			.replace(xCoordIsScalable, scalingX)
			.replace(yCoordIsScalable, scalingY)
			.replace(labelReadonlyTag, mReadOnly)
			.replace(labelIndexTag, QString::number(mIndex))
			.replace(labelNameTag, "\"" + name + "\"");

	return result;
}

QString Label::generateUpdate(MetaCompiler *compiler) const
{
	if (mTextBinded.isEmpty()) // static label
		return (nodeIndent + "Q_UNUSED(repo)" + endline);

	QString result = compiler->getTemplateUtils(updateDataTag);
	QString name = (mTextBinded == "name") ? compiler->getTemplateUtils(nameRoleTag)
						: compiler->getTemplateUtils(customRoleTag).replace(labelNameTag, mTextBinded);

	return result.replace(updateRoleTag, name)
			.replace(labelIndexTag, QString::number(mIndex));
}

QString Label::generateDefinition(MetaCompiler *compiler) const
{
	QString result = compiler->getTemplateUtils(labelDefinitionTag);
	return result.replace(labelIndexTag, QString::number(mIndex));
}
