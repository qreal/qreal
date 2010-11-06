#include "outFile.h"

using namespace utils;

OutFile::OutFile(QString const &fileName)
	: mIndent(0), mIndentSymbol('\t'), mIndentSymbolCount(1)
{
	mFile.setFileName(fileName);
	mFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if (!mFile.isOpen())
		throw "File open operation failed";
	mOut.setDevice(&mFile);
}

OutFile::~OutFile()
{
	mFile.close();
}

QTextStream& OutFile::operator()()
{
	mOut << indent();
	return mOut;
}

void OutFile::incIndent()
{
	++mIndent;
}

void OutFile::decIndent()
{
	--mIndent;
}

QString OutFile::indent() const
{
	QString result;
	for (int i = 0; i < mIndent; ++i)
		for (int j = 0; j < mIndentSymbolCount; ++j)
			result += mIndentSymbol;
	return result;
}

void OutFile::setIndentOptions(IndentSymbol symbol, int symbolsCount)
{
	mIndentSymbol = symbol == space ? ' ' : '\t';
	mIndentSymbolCount = symbolsCount;
}

