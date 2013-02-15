#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QLine>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QDate>
#include <QtGui/QSizePolicy>

#include "xmlUtils.h"

using namespace utils;

QDomDocument xmlUtils::loadDocument(QString const& fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "cannot open file " << fileName;
		return QDomDocument();
	}

	QDomDocument doc;
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;

	if (!doc.setContent(&file, false, &error, &errorLine, &errorCol)) {
		qDebug() << "parse error in " << fileName
				<< " at (" << errorLine << "," << errorCol
				<< "): " << error;
		file.close();
		return QDomDocument();
	}

	file.close();
	return doc;
}

/*!
* Converts a given QVariant value INTO
*  XML tag (QDomElement) (not it`s child)
*/
void xmlUtils::qVariantToXml(QDomElement &element, const QVariant &variant)
{
	switch (variant.type()) {
	case QVariant::Invalid:
	case QVariant::BitArray:
		//not supported yet
	case QVariant::Bitmap:
		//not supported yet
		break;
	case QVariant::Bool:
		element.setAttribute("type", "Bool");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::Brush:
		//not supported yet
	case QVariant::ByteArray:
		//not supported yet
		break;
	case QVariant::Char:
		element.setAttribute("type", "Char");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::Color:
		//not supported yet
	case QVariant::Cursor:
		//not supported yet
		break;
	case QVariant::Date:
		element.setAttribute("type", "Date");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::DateTime:
		element.setAttribute("type", "DateTime");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::Double:
		element.setAttribute("type", "Double");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::EasingCurve:
		//not supported yet
	case QVariant::Font:
		//not supported yet
	case QVariant::Hash:
		//not supported yet
	case QVariant::Icon:
		//not supported yet
	case QVariant::Image:
		//not supported yet
		break;
	case QVariant::Int:
		element.setAttribute("type", "Int");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::KeySequence:
		//not supported yet
		break;
	case QVariant::Line:
		element.setAttribute("type", "Line");
		element.setAttribute("x1", variant.toLine().x1());
		element.setAttribute("y1", variant.toLine().y1());
		element.setAttribute("x2", variant.toLine().x2());
		element.setAttribute("y2", variant.toLine().y2());
		break;
	case QVariant::LineF:
		element.setAttribute("type", "LineF");
		element.setAttribute("x1", variant.toLineF().x1());
		element.setAttribute("y1", variant.toLineF().y1());
		element.setAttribute("x2", variant.toLineF().x2());
		element.setAttribute("y2", variant.toLineF().y2());
		break;
	case QVariant::List:
		//not supported yet
	case QVariant::Locale:
		//not supported yet
		break;
	case QVariant::LongLong:
		element.setAttribute("type", "LongLong");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::Map:
		//not supported yet
	case QVariant::Matrix:
		//not supported yet
	case QVariant::Transform:
		//not supported yet
	case QVariant::Matrix4x4:
		//not supported yet
	case QVariant::Palette:
		//not supported yet
	case QVariant::Pen:
		//not supported yet
	case QVariant::Pixmap:
		//not supported yet
		break;
	case QVariant::Point:
		element.setAttribute("type", "Point");
		element.setAttribute("x", variant.toPoint().x());
		element.setAttribute("y", variant.toPoint().y());
		break;
	case QVariant::PointF:
		element.setAttribute("type", "PointF");
		element.setAttribute("x", variant.toPointF().x());
		element.setAttribute("y", variant.toPointF().y());
		break;
	case QVariant::Polygon:
		//not supported yet
	case QVariant::Quaternion:
		//not supported yet
		break;
	case QVariant::Rect:
		element.setAttribute("type", "Rect");
		element.setAttribute("x", variant.toRect().x());
		element.setAttribute("y", variant.toRect().y());
		element.setAttribute("width", variant.toRect().width());
		element.setAttribute("height", variant.toRect().height());
		break;
	case QVariant::RectF:
		element.setAttribute("type", "RectF");
		element.setAttribute("x", variant.toRectF().x());
		element.setAttribute("y", variant.toRectF().y());
		element.setAttribute("width", variant.toRectF().width());
		element.setAttribute("height", variant.toRectF().height());
		break;
	case QVariant::RegExp:
		//not supported yet
	case QVariant::Region:
		//not supported yet
		break;
	case QVariant::Size:
		element.setAttribute("type", "Size");
		element.setAttribute("width", variant.toSize().width());
		element.setAttribute("height", variant.toSize().height());
		break;
	case QVariant::SizeF:
		element.setAttribute("type", "SizeF");
		element.setAttribute("width", variant.toSizeF().width());
		element.setAttribute("height", variant.toSizeF().height());
		break;
	case QVariant::SizePolicy: {
		element.setAttribute("type", "SizePolicy");
		QSizePolicy const policy = variant.value<QSizePolicy>();
		element.setAttribute("verticalPolicy", policy.verticalPolicy());
		element.setAttribute("horizontalPolicy", policy.horizontalPolicy());
		element.setAttribute("verticalStretch", policy.verticalStretch());
		element.setAttribute("horizontalStretch", policy.horizontalStretch());
	}
		break;
	case QVariant::String:
		element.setAttribute("type", "String");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::StringList:
		//not supported yet
	case QVariant::TextFormat:
		//not supported yet
	case QVariant::TextLength:
		//not supported yet
		break;
	case QVariant::Time:
		element.setAttribute("type", "Time");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::UInt:
		element.setAttribute("type", "UInt");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::ULongLong:
		element.setAttribute("type", "ULongLong");
		element.setAttribute("value", variant.toString());
		break;
	case QVariant::Url:
		//not supported yet
	case QVariant::Vector2D:
		//not supported yet
	case QVariant::Vector3D:
		//not supported yet
	case QVariant::Vector4D:
		//not supported yet
	case QVariant::UserType:
		//not supported yet
		break;
	default:
		break;
	}
}

QVariant xmlUtils::xmlToVariant(const QDomElement &element)
{
	QString const type = element.attribute("type", "invalid");
	if (type == "invalid") {
		return QVariant();
	}
	if (type == "Bool") {
		return boolXmlToVariant(element);
	}
	if (type == "Char") {
		return charXmlToVariant(element);
	}
	if (type == "Date") {
		return dateXmlToVariant(element);
	}
	if (type == "DateTime") {
		return dateTimeXmlToVariant(element);
	}
	if (type == "Double") {
		return doubleXmlToVariant(element);
	}
	if (type == "Int") {
		return intXmlToVariant(element);
	}
	if (type == "Line") {
		return lineXmlToVariant(element);
	}
	if (type == "LineF") {
		return lineFXmlToVariant(element);
	}
	if (type == "LongLong") {
		return longLongXmlToVariant(element);
	}
	if (type == "Point") {
		return pointXmlToVariant(element);
	}
	if (type == "PointF") {
		return pointFXmlToVariant(element);
	}
	if (type == "Rect") {
		return rectXmlToVariant(element);
	}
	if (type == "RectF") {
		return rectFXmlToVariant(element);
	}
	if (type == "Size") {
		return sizeXmlToVariant(element);
	}
	if (type == "SizeF") {
		return sizeFXmlToVariant(element);
	}
	if (type == "SizePolicy") {
		return sizePolicyToVariant(element);
	}
	if (type == "String") {
		return stringXmlToVariant(element);
	}
	if (type == "Time") {
		return timeXmlToVariant(element);
	}
	if (type == "UInt") {
		return uIntXmlToVariant(element);
	}
	if (type == "ULongLong") {
		return uLongLongXmlToVariant(element);
	}
	return QVariant();
}

void xmlUtils::enumToXml(QDomElement &element, const QString &value)
{
	element.setAttribute("type", "enum");
	element.setAttribute("value", value);
}

void xmlUtils::enumToXml(QDomElement &element, int value)
{
	element.setAttribute("type", "enum");
	element.setAttribute("value", value);
}

int xmlUtils::xmlToEnumInt(QDomElement const &element)
{
	QString const type = element.attribute("type");
	if (type != "enum") {
		return -1;
	}
	QString const value = element.attribute("value");
	bool ok;
	int const e = value.toInt(&ok);
	if (!ok) {
		return -1;
	}
	return e;
}

void xmlUtils::flagsToXml(QDomElement &element, const QString &values)
{
	element.setAttribute("type", "flags");
	element.setAttribute("value", values);
}

QVariant xmlUtils::boolXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	if (value != "true" && value != "false") {
		return QVariant();
	}
	return QVariant(value == "true");
}

QVariant xmlUtils::charXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	if (value.length() != 1) {
		return QVariant();
	}
	return QVariant(value[0]);
}

QVariant xmlUtils::dateXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	QDate const date = QDate::fromString(value);
	if (!date.isValid()) {
		return QVariant();
	}
	return QVariant(date);
}

QVariant xmlUtils::dateTimeXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	QDateTime const dateTime = QDateTime::fromString(value);
	if (!dateTime.isValid()) {
		return QVariant();
	}
	return QVariant(dateTime);
}

QVariant xmlUtils::doubleXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	bool ok;
	double const d = value.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(d);
}

QVariant xmlUtils::intXmlToVariant(const QDomElement &element)
{
	QString value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	bool ok;
	int const i = value.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(i);
}

QVariant xmlUtils::lineXmlToVariant(const QDomElement &element)
{
	QString const x1S = element.attribute("x1", "invalid");
	if (x1S == "invalid") {
		return QVariant();
	}
	QString const y1S = element.attribute("y1", "invalid");
	if (y1S == "invalid") {
		return QVariant();
	}
	QString const x2S = element.attribute("x2", "invalid");
	if (x2S == "invalid") {
		return QVariant();
	}
	QString const y2S = element.attribute("y2", "invalid");
	if (y2S == "invalid") {
		return QVariant();
	}
	bool ok;
	int const x1 = x1S.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const y1 = y1S.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const x2 = x2S.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const y2 = y2S.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QLine(x1, y1, x2, y2));
}

QVariant xmlUtils::lineFXmlToVariant(const QDomElement &element)
{
	QString const x1S = element.attribute("x1", "invalid");
	if (x1S == "invalid") {
		return QVariant();
	}
	QString const y1S = element.attribute("y1", "invalid");
	if (y1S == "invalid") {
		return QVariant();
	}
	QString const x2S = element.attribute("x2", "invalid");
	if (x2S == "invalid") {
		return QVariant();
	}
	QString const y2S = element.attribute("y2", "invalid");
	if (y2S == "invalid") {
		return QVariant();
	}
	bool ok;
	double const x1 = x1S.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const y1 = y1S.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const x2 = x2S.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const y2 = y2S.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QLineF(x1, y1, x2, y2));
}

QVariant xmlUtils::longLongXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	bool ok;
	qlonglong const l = value.toLongLong(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(l);
}

QVariant xmlUtils::pointXmlToVariant(const QDomElement &element)
{
	QString const xS = element.attribute("x", "invalid");
	if (xS == "invalid") {
		return QVariant();
	}
	QString const yS = element.attribute("y", "invalid");
	if (yS == "invalid") {
		return QVariant();
	}
	bool ok;
	int const x = xS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const y = yS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QPoint(x, y));
}

QVariant xmlUtils::pointFXmlToVariant(const QDomElement &element)
{
	QString const xS = element.attribute("x", "invalid");
	if (xS == "invalid") {
		return QVariant();
	}
	QString const yS = element.attribute("y", "invalid");
	if (yS == "invalid") {
		return QVariant();
	}
	bool ok;
	double const x = xS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const y = yS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QPointF(x, y));
}

QVariant xmlUtils::rectXmlToVariant(const QDomElement &element)
{
	QString const xS = element.attribute("x", "invalid");
	if (xS == "invalid") {
		return QVariant();
	}
	QString const yS = element.attribute("y", "invalid");
	if (yS == "invalid") {
		return QVariant();
	}
	QString const widthS = element.attribute("width", "invalid");
	if (widthS == "invalid") {
		return QVariant();
	}
	QString const heightS = element.attribute("height", "invalid");
	if (heightS == "invalid") {
		return QVariant();
	}
	bool ok;
	int const x = xS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const y = yS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const width = widthS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const height = heightS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QRect(x, y, width, height));
}

QVariant xmlUtils::rectFXmlToVariant(const QDomElement &element)
{
	QString const xS = element.attribute("x", "invalid");
	if (xS == "invalid") {
		return QVariant();
	}
	QString const yS = element.attribute("y", "invalid");
	if (yS == "invalid") {
		return QVariant();
	}
	QString const widthS = element.attribute("width", "invalid");
	if (widthS == "invalid") {
		return QVariant();
	}
	QString const heightS = element.attribute("height", "invalid");
	if (heightS == "invalid") {
		return QVariant();
	}
	bool ok;
	double const x = xS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const y = yS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const width = widthS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const height = heightS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QRectF(x, y, width, height));
}

QVariant xmlUtils::sizeXmlToVariant(const QDomElement &element)
{
	QString const widthS = element.attribute("width", "invalid");
	if (widthS == "invalid") {
		return QVariant();
	}
	QString const heightS = element.attribute("height", "invalid");
	if (heightS == "invalid") {
		return QVariant();
	}
	bool ok;
	int const width = widthS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	int const height = heightS.toInt(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QSize(width, height));
}

QVariant xmlUtils::sizeFXmlToVariant(const QDomElement &element)
{
	QString const widthS = element.attribute("width", "invalid");
	if (widthS == "invalid") {
		return QVariant();
	}
	QString const heightS = element.attribute("height", "invalid");
	if (heightS == "invalid") {
		return QVariant();
	}
	bool ok;
	double const width = widthS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	double const height = heightS.toDouble(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(QSizeF(width, height));
}

QVariant xmlUtils::sizePolicyToVariant(QDomElement const &element)
{
	QString const verticalPolicyS = element.attribute("verticalPolicy");
	QString const horizontalPolicyS = element.attribute("horizontalPolicy");
	QString const verticalStretchS = element.attribute("verticalStretch");
	QString const horizontalStretchS = element.attribute("horizontalStretch");
	QSizePolicy result;
	result.setVerticalPolicy(static_cast<QSizePolicy::Policy>(verticalPolicyS.toInt()));
	result.setHorizontalPolicy(static_cast<QSizePolicy::Policy>(horizontalPolicyS.toInt()));
	result.setVerticalStretch(verticalStretchS.toInt());
	result.setHorizontalStretch(horizontalStretchS.toInt());
	return QVariant(result);
}

QVariant xmlUtils::stringXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value");
	return QVariant(value);
}

QVariant xmlUtils::timeXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	QTime const time = QTime::fromString(value);
	if (!time.isValid()) {
		return QVariant();
	}
	return QVariant(time);
}

QVariant xmlUtils::uIntXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	bool ok;
	uint const i = value.toUInt(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(i);
}

QVariant xmlUtils::uLongLongXmlToVariant(const QDomElement &element)
{
	QString const value = element.attribute("value", "invalid");
	if (value == "invalid") {
		return QVariant();
	}
	bool ok;
	qulonglong const l = value.toULongLong(&ok);
	if (!ok) {
		return QVariant();
	}
	return QVariant(l);
}
