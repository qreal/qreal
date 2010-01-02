#pragma once

#include <QFile>
#include <QTextStream>

namespace utils {

	class OutFile
	{
	public:
		explicit OutFile(QString const &fileName);
		~OutFile();
		QTextStream& operator()();

	private:
		QTextStream mOut;
		QFile mFile;
	};

}