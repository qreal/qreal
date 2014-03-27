#pragma once

#include <QtCore/QList>
#include <QtXml/QDomDocument>

namespace qReal
{
namespace elementEdit
{

/// A builder for element shape or widget template.
/// A state for element editor navigation framework.
class TemplateDocumentBuilder
{
public:
	TemplateDocumentBuilder();
	/// Collects all provided info into result document

	QDomDocument buildTemplate() const;
	/// Creates builder instance from specified document
	static TemplateDocumentBuilder *fromDocument(QDomDocument const &document);
	/// Returns widget template part from provided info
	QDomDocument wtf() const;
	/// Returns shape graphics (picture + other graphics)
	QDomDocument shape();
	/// Returns picture graphics
	QDomDocument picture() const;

	/// Returns icon`s widget template part from provided info
	QDomDocument iconWtf() const;
	/// Returns icon`s shape graphics (picture + other graphics)
	QDomDocument iconShape();
	/// Returns icon`s picture graphics
	QDomDocument iconPicture() const;

	bool isWidgetBased() const;
	void setWidgetBased(bool isWidgetBased);
	bool isIconWidgetBased() const;
	void setIconWidgetBased(bool isWidgetBased);

	/// Merges current builder info with info in specified document
	void mergeDocument(QDomDocument const &document);
	/// Merges current builder info with info in specified document (icon`s part)
	void mergeIconDocument(QDomDocument const &document);

	/// Merges current builder wtf info with wtf info in specified document
	void mergeShapeGraphics(QDomDocument const &shape);
	/// Merges current builder wtf info with wtf info in specified document
	void mergeWidgetGraphics(QDomDocument const &wtf);
	/// Merges current builder wtf info with wtf info in specified document (icon`s part)
	void mergeIconShapeGraphics(QDomDocument const &shape);
	/// Merges current builder wtf info with wtf info in specified document (icon`s part)
	void mergeIconWidgetGraphics(QDomDocument const &wtf);

	/// Surrounds widget-template tag with graphics one
	void toGraphics(QDomDocument &widgetTemplate);

private:
	static TemplateDocumentBuilder *fromGraphics(QDomElement const &graphics);
	void mergeGraphics(QDomElement const &graphics);

	void processWtf(QDomElement const &wtf);
	void processPicture(QDomElement const &picture);
	void processOtherGraphics(QDomElement const &someGraphics);
	void processIcon(QDomElement const &wtf);

	QDomElement wtfShape(QDomElement const &wtf);
	void setWtfShape(QDomElement const &wtf, QDomElement const &picture);

	inline QDomElement templateElement() const;
	inline QDomDocument createDocument(QDomElement const &documentElement) const;

	// A factory for our nodes
	QDomDocument mDocument;
	QDomElement mWidgetTemplate;
	QDomElement mShapeTemplate;
	TemplateDocumentBuilder *mIconBuilder;
	QList<QDomElement> mOtherGraphics;
	bool mIsWidgetBased;
};

}
}
