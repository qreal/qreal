#pragma once

#include <QtCore/QXmlStreamReader>
#include <QtCore/QFileInfo>

#include "detailsParser.h"

namespace qrUpdater {

/// Collects info from NetworkResponse
class XmlDataParser : public DetailsParser
{
	Q_OBJECT

public:
	class ReadError{};

	XmlDataParser();
	~XmlDataParser() override;

	void parseDevice(QIODevice *device) override;
	bool hasErrors() const override;

private:
	void readXml() throw(ReadError);
	void readUnitFile();

	QXmlStreamReader *mXml;
	bool mHasInvalidFile;
};

}
