#include "listsmartline.h"

ListSmartLine::ListSmartLine()
{
}

bool ListSmartLine::isContains(QList<SmartLine_old> const &storage, QString const &templateCode)
{
	foreach (SmartLine_old const &code, storage) {
		if (code.text() == templateCode) {
			return true;
		}
	}
	return false;
}

bool ListSmartLine::isContainsPart(QList<SmartLine_old> const &storage, QString const &templateCode)
{
	foreach (SmartLine_old const &code, storage) {
		if (templateCode.compare(code.text()) <= 0) {
			return true;
		}
	}
	return false;
}
