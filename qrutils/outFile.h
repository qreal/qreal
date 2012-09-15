#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT OutFile
{
public:
	explicit OutFile(QString const &fileName);
	~OutFile();
	QTextStream &operator()();

private:
	QTextStream mOut;
	QFile mFile;
};

}
