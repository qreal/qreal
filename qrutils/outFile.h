#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT OutFile
{
public:
	explicit OutFile(const QString &fileName);
	~OutFile();
	QTextStream &operator()();

private:
	QTextStream mOut;
	QFile mFile;
};

}
