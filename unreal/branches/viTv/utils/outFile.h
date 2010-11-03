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

		enum IndentSymbol {
			space
			, tab
		};

		void setIndentOptions(IndentSymbol symbol, int symbolsCount);
	private:
		QString indent() const;

		QTextStream mOut;
		QFile mFile;
		int mIndent;
		char mIndentSymbol;
		int mIndentSymbolCount;
	};

}
