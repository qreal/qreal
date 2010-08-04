#include "pointPort.h"
#include "../utils/outFile.h"
#include "utils/defs.h"
#include "metaCompiler.h"

using namespace utils;

bool PointPort::init(QDomElement const &element, int width, int height)
{
	mX = static_cast<qreal>(element.attribute("x").toInt()) / width;
	mY = static_cast<qreal>(element.attribute("y").toInt()) / height;
	mWidth = width;
	mHeight = height;
	return true;
}

Port* PointPort::clone() const
{
	PointPort *result = new PointPort();
	result->mX = mX;
	result->mY = mY;
	return result;
}

QString PointPort::generateSdf(MetaCompiler *compiler) const
{
	QString result = compiler->getTemplateUtils(pointPortTag);
	result.replace(pointXTag, QString::number(mX * mWidth)).replace(pointYTag, QString::number(mY * mHeight));
	return result;
}

QString PointPort::generateInit(MetaCompiler *compiler) const
{
	QString result = compiler->getTemplateUtils(nodePointPortInitTag);
	result.replace(pointXTag, QString::number(mX)).replace(pointYTag, QString::number(mY));
	return result;

}
