/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "interpreterCore/textLanguage/robotsBlockParser.h"

#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/shell.h>

#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>
#include <qrtext/lua/types/string.h>

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

	const QHash<QString, int> buttons = mRobotModelManager.model().buttonCodes();
	for (const QString &button : buttons.keys()) {
		setVariableValue(button, buttons[button]);
		markAsSpecial(button);
		mHiddenVariables << button;
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

	const auto add2aryFunction = [this] (const QString &name
			, qrtext::core::types::TypeExpression * const returnType
			, qrtext::core::types::TypeExpression * const argument1Type
			, qrtext::core::types::TypeExpression * const argument2Type
			, std::function<QVariant(QVariant, QVariant)> const &function)
	{
		addIntrinsicFunction(name, returnType
				, {argument1Type, argument2Type}
				, [function] (const QList<QVariant> &params) {
						Q_ASSERT(params.count() == 2);
						return function(params.first(), params.last());
				});
	};


	const auto addFloatFunction = [this, add1aryFunction] (const QString &name
			, std::function<qreal(qreal)> const &function)
	{
		add1aryFunction(name, new types::Float, new types::Float
				, [function](const QVariant &arg) { return function(arg.toReal()); });
	};

	const auto addIntegerFunction = [this, add1aryFunction] (const QString &name
			, std::function<qreal(qreal)> const &function)
	{
		add1aryFunction(name, new types::Integer, new types::Integer
				, [function](const QVariant &arg) { return function(arg.toInt()); });
	};

	const auto addFloatToIntegerFunction = [this, add1aryFunction] (const QString &name
			, std::function<int(qreal)> const &function)
	{
		add1aryFunction(name, new types::Integer(), new types::Float()
				, [function](const QVariant &arg) { return function(arg.toReal()); });
	};

	add0aryFunction("time", new types::Integer(), [this]() { return mTimeComputer(); });
	add1aryFunction("sensor", new types::Integer(), new types::String(), [this](const QVariant &port) {
		if (port.toString().isEmpty()) {
			/// @todo: Add error reporting
			return 0;
		}

		return interpret<int>("sensor" + port.toString());
	});

	add1aryFunction("print", new types::String(), new types::String, [this](const QVariant &text) {
		kitBase::robotModel::robotParts::Shell *shell = kitBase::robotModel::RobotModelUtils::findDevice
				<kitBase::robotModel::robotParts::Shell>(mRobotModelManager.model(), "ShellPort");
		if (shell) {
			qDebug() << "printing in interpreter" << text.toString() << shell;
			shell->print(text.toString());
		}

		return text;
	});

	addFloatFunction("sin", [](qreal x) {return sin(x); });
	addFloatFunction("cos", [](qreal x) {return cos(x); });
	addFloatFunction("ln", [](qreal x) {return log(x); });
	addFloatFunction("exp", [](qreal x) {return exp(x); });
	addFloatFunction("asin", [](qreal x) {return asin(x); });
	addFloatFunction("acos", [](qreal x) {return acos(x); });
	addFloatFunction("atan", [](qreal x) {return atan(x); });
	addFloatToIntegerFunction("sgn", [](qreal x) {return (0 < x) - (x < 0); });
	addFloatFunction("sqrt", [](qreal x) {return sqrt(x); });
	addFloatFunction("abs", [](qreal x) {return qAbs(x); });
	addFloatToIntegerFunction("ceil", [](qreal x) {return static_cast<int>(ceil(x)); });
	addFloatToIntegerFunction("floor", [](qreal x) {return static_cast<int>(floor(x)); });
	addIntegerFunction("random", [](int x) {return rand() % x; });

	add2aryFunction("min", new types::Float(), new types::Float(), new types::Float()
			, [](const QVariant &a, const QVariant &b) { return qMin(a.toReal(), b.toReal()); });
	add2aryFunction("max", new types::Float(), new types::Float(), new types::Float()
			, [](const QVariant &a, const QVariant &b) { return qMax(a.toReal(), b.toReal()); });
}
