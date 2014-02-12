#include <QtCore/QDebug>

#include "templateDocumentBuilder.h"

using namespace qReal::elementEdit;

TemplateDocumentBuilder::TemplateDocumentBuilder()
	: mIconBuilder(NULL), mIsWidgetBased(false)
{
}

QDomDocument TemplateDocumentBuilder::wtf() const
{
	return createDocument(mWidgetTemplate);
}

QDomDocument TemplateDocumentBuilder::shape()
{
	// Small cheat not to write build logic twice
	bool oldWidgetBased = mIsWidgetBased;
	setWidgetBased(false);

	QDomDocument const result = buildTemplate();

	setWidgetBased(oldWidgetBased);

	return result;
}

QDomDocument TemplateDocumentBuilder::picture() const
{
	return createDocument(mShapeTemplate);
}

QDomDocument TemplateDocumentBuilder::iconWtf() const
{
	if (!mIconBuilder) {
		return wtf();
	}
	return mIconBuilder->wtf();
}

QDomDocument TemplateDocumentBuilder::iconShape()
{
	if (!mIconBuilder) {
		return shape();
	}
	return mIconBuilder->shape();
}

QDomDocument TemplateDocumentBuilder::iconPicture() const
{
	if (!mIconBuilder) {
		return picture();
	}
	return mIconBuilder->picture();
}

bool TemplateDocumentBuilder::isWidgetBased() const
{
	return mIsWidgetBased;
}

void TemplateDocumentBuilder::setWidgetBased(bool isWidgetBased)
{
	mIsWidgetBased = isWidgetBased;
}

bool TemplateDocumentBuilder::isIconWidgetBased() const
{
	return mIconBuilder ? mIconBuilder->isWidgetBased() : mIsWidgetBased;
}

void TemplateDocumentBuilder::setIconWidgetBased(bool isWidgetBased)
{
	if (mIconBuilder) {
		mIconBuilder->setWidgetBased(isWidgetBased);
	}
}

QDomDocument TemplateDocumentBuilder::buildTemplate() const
{
	QDomDocument result;
	QDomElement graphics = result.createElement("graphics");
	graphics.appendChild(templateElement());
	if (mIconBuilder) {
		QDomElement icon = result.createElement("icon");
		icon.appendChild(mIconBuilder->buildTemplate().cloneNode());
		graphics.appendChild(icon);
	}
	foreach (QDomElement const &element, mOtherGraphics) {
		graphics.appendChild(element.cloneNode());
	}
	result.appendChild(graphics);
	return result;
}

TemplateDocumentBuilder *TemplateDocumentBuilder::fromDocument(QDomDocument const &document)
{
	return fromGraphics(document.documentElement());
}

TemplateDocumentBuilder *TemplateDocumentBuilder::fromGraphics(QDomElement const &graphics)
{
	TemplateDocumentBuilder *result = new TemplateDocumentBuilder;
	result->mergeGraphics(graphics);
	return result;
}

void TemplateDocumentBuilder::mergeDocument(QDomDocument const &document)
{
	mergeGraphics(document.documentElement());
}

void TemplateDocumentBuilder::mergeIconDocument(QDomDocument const &document)
{
	if (mIconBuilder) {
		mIconBuilder->mergeDocument(document);
	} else {
		mIconBuilder = TemplateDocumentBuilder::fromDocument(document);
	}
}

void TemplateDocumentBuilder::mergeGraphics(QDomElement const &graphics)
{
	QDomElement element = graphics.firstChildElement();
	while (!element.isNull()) {
		QString const tagName = element.tagName();
		if (tagName == "widget-template") {
			processWtf(element);
			mIsWidgetBased = true;
		} else if (tagName == "picture") {
			processPicture(element);
		} else if (tagName == "icon") {
			QDomElement const iconGraphics =
					element.firstChildElement("graphics").cloneNode().toElement();
			mIconBuilder = fromGraphics(iconGraphics);
		} else {
			processOtherGraphics(element);
		}
		element = element.nextSiblingElement();
	}
}

void TemplateDocumentBuilder::mergeShapeGraphics(QDomDocument const &shape)
{
	QDomElement const graphics = shape.documentElement();
	QDomElement element = graphics.firstChildElement();
	while (!element.isNull()) {
		QString const tagName = element.tagName();
		if (tagName == "picture") {
			processPicture(element);
		} else {
			processOtherGraphics(element);
		}
		element = element.nextSiblingElement();
	}
}

void TemplateDocumentBuilder::mergeWidgetGraphics(QDomDocument const &wtf)
{
	processWtf(wtf.documentElement().firstChildElement("widget-template"));
}

void TemplateDocumentBuilder::mergeIconShapeGraphics(QDomDocument const &shape)
{
	if (!mIconBuilder) {
		mIconBuilder = TemplateDocumentBuilder::fromDocument(shape);
	} else {
		mIconBuilder->mergeShapeGraphics(shape);
	}
}

void TemplateDocumentBuilder::mergeIconWidgetGraphics(QDomDocument const &wtf)
{
	if (!mIconBuilder) {
		mIconBuilder = TemplateDocumentBuilder::fromDocument(wtf);
	} else {
		mIconBuilder->mergeWidgetGraphics(wtf);
	}
}

void TemplateDocumentBuilder::processWtf(const QDomElement &wtf)
{
	mWidgetTemplate = wtf;
	QDomElement const currentWtfShape = wtfShape(wtf);
	mShapeTemplate = currentWtfShape;
	if (!mShapeTemplate.isNull() && currentWtfShape.isNull()) {
		setWtfShape(mWidgetTemplate, mShapeTemplate);
	}
}

void TemplateDocumentBuilder::processPicture(QDomElement const &picture)
{
	mShapeTemplate = picture.cloneNode().toElement();
	if (!mWidgetTemplate.isNull()) {
		setWtfShape(mWidgetTemplate, picture);
	}
}

void TemplateDocumentBuilder::processOtherGraphics(QDomElement const &someGraphics)
{
	QString const tagName = someGraphics.tagName();
	foreach(QDomElement const &someOldGraphics, mOtherGraphics) {
		if (someOldGraphics.tagName() == tagName) {
			mOtherGraphics.removeOne(someOldGraphics);
			mOtherGraphics.append(someGraphics);
			return;
		}
	}
	mOtherGraphics.append(someGraphics);
}

QDomElement TemplateDocumentBuilder::wtfShape(QDomElement const &wtf)
{
	QDomElement const root = wtf.firstChildElement("Root");
	if (root.isNull()) {
		return QDomElement();
	}
	QDomElement rootChild = root.firstChildElement();
	while (!rootChild.isNull()) {
		if (rootChild.tagName() == "property" && rootChild.hasAttribute("propertyName")
				&& rootChild.attribute("propertyName") == "shapeXml") {
			QString const shapeString = rootChild.attribute("value");
			QDomDocument shapeDocument;
			shapeDocument.setContent(shapeString);
			return shapeDocument.documentElement();
		}
		rootChild = rootChild.nextSiblingElement();
	}
	return QDomElement();
}

void TemplateDocumentBuilder::setWtfShape(QDomElement const &wtf
		, QDomElement const &picture)
{
	QDomElement const root = wtf.firstChildElement("Root");
	if (root.isNull()) {
		return;
	}
	QDomElement rootChild = root.firstChildElement();
	while (!rootChild.isNull()) {
		if (rootChild.tagName() == "property" && rootChild.hasAttribute("propertyName")
				&& rootChild.attribute("propertyName") == "shapeXml") {
			QDomDocument pictureDocument;
			// To make it non-null
			pictureDocument.createElement("");
			pictureDocument.appendChild(picture.cloneNode());
			rootChild.setAttribute("value", pictureDocument.toString(4));
			break;
		}
		rootChild = rootChild.nextSiblingElement();
	}
}

QDomElement TemplateDocumentBuilder::templateElement() const
{
	QDomElement result = mIsWidgetBased ? mWidgetTemplate : mShapeTemplate;
	return result.cloneNode().toElement();
}

QDomDocument TemplateDocumentBuilder::createDocument(QDomElement const &documentElement) const
{
	QDomDocument result;
	// To make it non-null
	result.createElement("");
	result.appendChild(documentElement.cloneNode());
	return result;
}

void TemplateDocumentBuilder::toGraphics(QDomDocument &widgetTemplate)
{
	QDomElement const wtf = widgetTemplate.firstChildElement("widget-template");
	if (wtf.isNull()) {
		return;
	}
	QDomNode const newWtf = widgetTemplate.removeChild(wtf);
	QDomElement graphics = widgetTemplate.createElement("graphics");
	graphics.appendChild(newWtf);
	widgetTemplate.appendChild(graphics);
}
