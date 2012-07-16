#pragma once

#include <QFile>
#include <QDir>
#include <QTextStream>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT InFile
{
public:
	explicit InFile(QString const &fileName);
	QTextStream& operator()();
	~InFile();

private:
	QFile mFile;
	QTextStream mInput;
};
}
