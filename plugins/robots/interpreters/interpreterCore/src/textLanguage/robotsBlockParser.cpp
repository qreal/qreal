#include "interpreterCore/textLanguage/robotsBlockParser.h"

#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>

using namespace qReal;
using namespace interpreterCore::textLanguage;
using namespace qrtext::lua;

const QString sensorVariablePerfix = QObject::tr("sensor");
const QString encoderVariablePerfix = QObject::tr("encoder");
const QString timeVariableName = QObject::tr("time");

RobotsBlockParser::RobotsBlockParser(
		const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, const utils::ComputableNumber::IntComputer &timeComputer)
	: qrtext::lua::LuaToolbox()
	, mRobotModelManager(robotModelManager)
	, mTimeComputer(timeComputer)
{
	setReservedVariables();

	addIntrinsicFuctions();

	connect(&mRobotModelManager, &kitBase::robotModel::RobotModelManagerInterface::robotModelChanged
			, this, &RobotsBlockParser::setReservedVariables);
}

void RobotsBlockParser::setReservedVariables()
{
	qrtext::lua::LuaToolbox::clear();

	setVariableValue("pi", 3.14159265);
	markAsSpecialConstant("pi");

	for (const kitBase::robotModel::PortInfo &port : mRobotModelManager.model().availablePorts()) {
		setVariableValue(port.name(), QString("'%1'").arg(port.name()));

		markAsSpecial(port.name());
		mHiddenVariables << port.name();

		for (const QString &alias : port.nameAliases()) {
			setVariableValue(alias, port.name());
			markAsSpecial(alias);
			mHiddenVariables << alias;
		}

		if (!port.reservedVariable().isEmpty()) {
			if (port.reservedVariableType() == kitBase::robotModel::PortInfo::ReservedVariableType::scalar) {
				setVariableValue(port.reservedVariable(), 0);
			} else {
				setVectorVariableValue(port.reservedVariable(), QVector<int>{0});
			}

			markAsSpecial(port.reservedVariable());
		}
	}
}

const QStringList &RobotsBlockParser::hiddenVariables() const
{
	return mHiddenVariables;
}

void RobotsBlockParser::clear()
{
	setReservedVariables();
}

void RobotsBlockParser::addIntrinsicFuctions()
{
	const auto add0aryFunction = [this] (const QString &name
			, qrtext::core::types::TypeExpression * const returnType
			, std::function<QVariant()> const &function)
	{
		addIntrinsicFunction(name, returnType
				, {}
				, [function] (const QList<QVariant> &params) {
						Q_UNUSED(params);
						return function();
				});
	};

	const auto add1aryFunction = [this] (const QString &name
			, qrtext::core::types::TypeExpression * const returnType
			, qrtext::core::types::TypeExpression * const argumentType
			, std::function<QVariant(QVariant)> const &function)
	{
		addIntrinsicFunction(name, returnType
				, {argumentType}
				, [function] (const QList<QVariant> &params) {
						Q_ASSERT(!params.isEmpty());
						return function(params.first());
				});
	};

	const auto addFloatFunction = [this, add1aryFunction] (const QString &name
			, std::function<double(double)> const &function)
	{
		add1aryFunction(name, new types::Float, new types::Float
				, [function](const QVariant &arg) { return function(arg.toDouble()); });
	};

	const auto addIntegerFunction = [this, add1aryFunction] (const QString &name
			, std::function<double(double)> const &function)
	{
		add1aryFunction(name, new types::Integer, new types::Integer
				, [function](const QVariant &arg) { return function(arg.toInt()); });
	};

	const auto addFloatToIntegerFunction = [this, add1aryFunction] (const QString &name
			, std::function<int(double)> const &function)
	{
		add1aryFunction(name, new types::Integer(), new types::Float()
				, [function](const QVariant &arg) { return function(arg.toDouble()); });
	};

	add0aryFunction("time", new types::Integer(), [this]() { return mTimeComputer(); });

	addFloatFunction("sin", [](qreal x) {return sin(x); });
	addFloatFunction("cos", [](qreal x) {return cos(x); });
	addFloatFunction("ln", [](qreal x) {return log(x); });
	addFloatFunction("exp", [](qreal x) {return exp(x); });
	addFloatFunction("asin", [](qreal x) {return asin(x); });
	addFloatFunction("acos", [](qreal x) {return acos(x); });
	addFloatFunction("atan", [](qreal x) {return atan(x); });
	addFloatToIntegerFunction("sgn", [](qreal x) {return (0 < x) - (x < 0); });
	addFloatFunction("sqrt", [](qreal x) {return sqrt(x); });
	addFloatFunction("abs", [](qreal x) {return abs(x); });
	addFloatToIntegerFunction("ceil", [](qreal x) {return static_cast<int>(ceil(x)); });
	addFloatToIntegerFunction("floor", [](qreal x) {return static_cast<int>(floor(x)); });
	addIntegerFunction("random", [](int x) {return rand() % x; });
}
