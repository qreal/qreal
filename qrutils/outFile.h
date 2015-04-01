#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT OutFile
{
public:
	explicit OutFile(const QString &fileName);
	~OutFile();
	QTextStream &operator()();

	/// Flushes all new modifications in text stream into the file system.
	void flush();

	/// Creates in heap new OutFile instance if it can be done
	/// or returns nullptr and writes into log error message otherwise.
	static OutFile *openOrLogError(const QString &fileName);

private:
	QTextStream mOut;
	QFile mFile;
};

}
