#include "xmlSerializer.h"
#include "../../qrutils/outFile.h"

using namespace qrRepo::details;
using namespace utils;

XmlSerializer::XmlSerializer()
{
}


void XmlSerializer::exportTo(const QString &targetFile)
{
	Q_ASSERT_X(!targetFile.isEmpty(), "XmlSerializer::exportTo(...)", "target filename is empty");

	QDomDocument doc;
	QDomElement root = doc.createElement("project");
	doc.appendChild(root);

	OutFile out(targetFile);
	doc.save(out(), 2);
}
