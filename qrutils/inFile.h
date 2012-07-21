#pragma once

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT InFile
{
public:
	explicit InFile();
	static QString readAll(QString const &nameFile);
	~InFile();
};
}
