#include "function_gen.h"

using namespace qReal;
using namespace robots::generator;

void Function_gen::generateBodyWithoutNextElementCall()
{
	QByteArray byteFuncCode = mNxtGen->mApi->stringProperty(mElementId, "Body").toUtf8();

	byteFuncCode.replace("Сенсор1", "ecrobot_get_sonar_sensor(NXT_PORT_S1)");
	byteFuncCode.replace("Сенсор2", "ecrobot_get_sonar_sensor(NXT_PORT_S2)");
	byteFuncCode.replace("Сенсор3", "ecrobot_get_sonar_sensor(NXT_PORT_S3)");
	byteFuncCode.replace("Сенсор4", "ecrobot_get_sonar_sensor(NXT_PORT_S4)");

	variableAnalysis(byteFuncCode);

	QString funcCode = QString::fromUtf8(byteFuncCode);
	foreach (QString str, funcCode.split(';')) {
		mNxtGen->mGeneratedStrings.append(SmartLine(str.trimmed() + ";", mElementId));
	}
}

void Function_gen::variableAnalysis(QByteArray const &code)
{
	QList<QByteArray> funcBlocks = code.split(';');

	foreach (QByteArray block, funcBlocks) {
			//Only one possible place for first variable appear
		int firstEqualSignPos = block.indexOf('=');
		if (firstEqualSignPos == -1)
			continue;

		//must be a normal variable name
		QByteArray leftPart = block.left(firstEqualSignPos);

		leftPart = leftPart.trimmed();
		QString forbiddenLastSimbols = "+-=*/><";
		if (forbiddenLastSimbols.contains((leftPart.at(leftPart.length() - 1))))
			continue;

		bool isVariableExisted = false;
		foreach (SmartLine curVariable, mNxtGen->mVariables) {
			if (curVariable.text() == QString::fromUtf8(leftPart)) {
				isVariableExisted = true;
				break;
			}
		}
		if (!isVariableExisted)
			mNxtGen->mVariables.append(SmartLine(QString::fromUtf8(leftPart), mElementId));
	}
}
