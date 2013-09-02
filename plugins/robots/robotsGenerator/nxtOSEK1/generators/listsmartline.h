#pragma once

#include <QtCore/QList>
#include "../smartLine.h"

using namespace robots::generator;

class ListSmartLine
{
public:
	ListSmartLine();
	static bool isContains(QList<SmartLine_old> const &storage, QString const &templateCode);
	static bool isContainsPart(QList<SmartLine_old> const &storage, QString const &templateCode); // rename
};
