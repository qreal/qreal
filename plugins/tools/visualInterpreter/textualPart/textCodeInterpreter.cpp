#include "textCodeInterpreter.h"
#include "textCodeGenerator.h"

using namespace qReal;

TextCodeInterpreter::TextCodeInterpreter(QObject *parent)
		: QObject(parent)
		, mApplicationConditionResult(false)
		, mErrorOccured(false)
{
}

QHash<QPair<QString, QString>, QString> &TextCodeInterpreter::parseOutput(QString const &output) const
{
	int pos = 0;
	QHash<QPair<QString, QString>, QString> *res = new QHash<QPair<QString, QString>, QString>();
	parseOutput(*res, output, pos);
	return *res;
}

void TextCodeInterpreter::parseOutput(QHash<QPair<QString, QString>, QString> &res, QString const &output, int &pos) const
{
	int const delimeterIndex = output.indexOf(TextCodeGenerator::delimeter, pos);
	if (delimeterIndex == -1 || output.indexOf("';") == -1) {
		return;
	}

	int const semicolumnIndex = output.indexOf("';", pos);
	int const equalsIndex = output.indexOf("='", pos);
	QString const elemName = output.mid(pos, delimeterIndex - pos);
	QString const attrName = output.mid(delimeterIndex + TextCodeGenerator::delimeter.length()
			, equalsIndex -delimeterIndex - TextCodeGenerator::delimeter.length());
	QString const value = output.mid(equalsIndex + 2, semicolumnIndex - equalsIndex - 2);

	res.insert(QPair<QString, QString>(elemName, attrName), value);
	pos = semicolumnIndex + 2;
	parseOutput(res, output, pos);
}
