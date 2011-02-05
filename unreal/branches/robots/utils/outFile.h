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

		void incIndent();
		void decIndent();
	private:
		QString indent() const;

		QTextStream mOut;
		QFile mFile;
		int mIndent;
	};

}
