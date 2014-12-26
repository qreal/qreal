#include "robotsBlockParser.h"

#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>

using namespace qReal;
using namespace interpreterCore::textLanguage;
using namespace qrtext::lua;

QString const sensorVariablePerfix = QObject::tr("sensor");
QString const encoderVariablePerfix = QObject::tr("encoder");
QString const timeVariableName = QObject::tr("time");

RobotsBlockParser::RobotsBlockParser(
		interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, utils::ComputableNumber::IntComputer const &timeComputer)
	: qrtext::lua::LuaToolbox()
	, mRobotModelManager(robotModelManager)
	, mTimeComputer(timeComputer)
{
	setReservedVariables();

	addIntrinsicFuctions();

	connect(&mRobotModelManager, &interpreterBase::robotModel::RobotModelManagerInterface::robotModelChanged
			, this, &RobotsBlockParser::setReservedVariables);
}

void RobotsBlockParser::setReservedVariables()
{
	/// @todo Remove old reserved variables for old model.
	qrtext::lua::LuaToolbox::clear();

	setVariableValue("pi", 3.14159265);
	markAsSpecialConstant("pi");

	for (interpreterBase::robotModel::PortInfo const &port : mRobotModelManager.model().availablePorts()) {
		setVariableValue(port.name(), QString("'%1'").arg(port.name()));

		markAsSpecial(port.name());
		mHiddenVariables << port.name();

		for (QString const &alias : port.nameAliases()) {
			setVariableValue(alias, port.name());
			markAsSpecial(alias);
			mHiddenVariables << alias;
		}

		if (!port.reservedVariable().isEmpty()) {
			setVariableValue(port.reservedVariable(), 0);
			markAsSpecial(port.reservedVariable());
		}
	}
}

QStringList const &RobotsBlockParser::hiddenVariables() const
{
	return mHiddenVariables;
}

void RobotsBlockParser::clear()
{
	setReservedVariables();
}

void RobotsBlockParser::addIntrinsicFuctions()
{
	auto const add0aryFunction = [this] (QString const &name
			, qrtext::core::types::TypeExpression * const returnType
			, std::function<QVariant()> const &function)
	{
		addIntrinsicFunction(name, returnType
				, {}
				, [function] (QList<QVariant> const &params) {
						Q_UNUSED(params);
						return function();
				});
	};

	auto const add1aryFunction = [this] (QString const &name
			, qrtext::core::types::TypeExpression * const returnType
			, qrtext::core::types::TypeExpression * const argumentType
			, std::function<QVariant(QVariant)> const &function)
	{
		addIntrinsicFunction(name, returnType
				, {argumentType}
				, [function] (QList<QVariant> const &params) {
						Q_ASSERT(!params.isEmpty());
						return function(params.first());
				});
	};

	auto const addFloatFunction = [this, add1aryFunction] (QString const &name
			, std::function<double(double)> const &function)
	{
		add1aryFunction(name, new types::Float, new types::Float
				, [function](QVariant const &arg) { return function(arg.toDouble()); });
	};

	auto const addIntegerFunction = [this, add1aryFunction] (QString const &name
			, std::function<double(double)> const &function)
	{
		add1aryFunction(name, new types::Integer, new types::Integer
				, [function](QVariant const &arg) { return function(arg.toInt()); });
	};

	auto const addFloatToIntegerFunction = [this, add1aryFunction] (QString const &name
			, std::function<int(double)> const &function)
	{
		add1aryFunction(name, new types::Integer(), new types::Float()
				, [function](QVariant const &arg) { return function(arg.toDouble()); });
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
