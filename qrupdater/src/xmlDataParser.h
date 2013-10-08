#pragma once

#include <QtCore/QXmlStreamReader>
#include <QtCore/QFileInfo>

#include "detailsParser.h"

class XmlDataParser : public DetailsParser
{
	Q_OBJECT
public:
	class ReadError{};

	XmlDataParser();
	virtual ~XmlDataParser();
	virtual void parseDevice(QIODevice *device);
	virtual bool hasErrors() const;

protected:
	void readXml() throw(ReadError);
	void readUnitFile();

	QXmlStreamReader *mXml;
	bool mHasInvalidFile;
};

