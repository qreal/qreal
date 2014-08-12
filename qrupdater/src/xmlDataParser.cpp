#include "xmlDataParser.h"

using namespace qrUpdater;

XmlDataParser::XmlDataParser()
	: DetailsParser()
	, mXml(nullptr)
	, mHasInvalidFile(false)
{
}

XmlDataParser::~XmlDataParser()
{
	delete mXml;
}

void XmlDataParser::parseDevice(QIODevice *device)
{
	mHasInvalidFile = false;
	mXml = new QXmlStreamReader(device);
	try {
		readXml();
	} catch(ReadError &) {
		mHasInvalidFile = true;
	}

	device->deleteLater();
}

bool XmlDataParser::hasErrors() const
{
	return mHasInvalidFile;
}

void XmlDataParser::readXml() throw(ReadError)
{
	if (!mXml->readNextStartElement() || mXml->name() != "updaterInformation") {
		throw (ReadError());
	}

	while (mXml->readNextStartElement()) {
		if (mXml->name() == "unitFile") {
			readUnitFile();
		} else if (mXml->name() != "unitFileList") {
			mXml->skipCurrentElement();
		}
	}
}

void XmlDataParser::readUnitFile()
{
	Q_ASSERT(mXml->isStartElement() && mXml->name() == "unitFile");

	QString curModule;
	QUrl fileUrl;
	QString argument;
	QString version;
	while (mXml->readNextStartElement()) {
		if (mXml->name() == "unit") {
			curModule = mXml->readElementText();
		} else if (mXml->name() == "url") {
			fileUrl = QUrl(mXml->readElementText());
		} else if (mXml->name() == "version") {
			version = mXml->readElementText();
		} else if (mXml->name() == "paramStr") {
			argument = mXml->readElementText();
		} else {
			mXml->skipCurrentElement();
		}
	}

	mFileUrls.insert(curModule, fileUrl);
	mParamStrings.insert(curModule, argument);
	mVersions.insert(curModule, version);
}
