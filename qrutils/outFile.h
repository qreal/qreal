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
	QTextStream& operator()();

	void incIndent();
	void decIndent();
private:
	QString indent() const;

	QTextStream mOut;
	QFile mFile;
	int mIndent;
};

}
