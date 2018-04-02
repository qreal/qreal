/* Copyright 2018 QReal Research Group
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
#pragma once

#include <qrtext/include/qrtext/languageToolboxInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class LanguageToolboxInterfaceMock : public qrtext::LanguageToolboxInterface
{
	Q_OBJECT

public:
	MOCK_METHOD3(interpret, void(const qReal::Id &id, const QString &propertyName, const QString &code));
	MOCK_METHOD1(interpret, void(const QString &code));
	MOCK_METHOD3(parse, QSharedPointer<qrtext::core::ast::Node> &(const qReal::Id &id
			, const QString &propertyName
			, const QString &code));
	MOCK_CONST_METHOD2(ast, QSharedPointer<qrtext::core::ast::Node>(const qReal::Id &id, const QString &propertyName));
	MOCK_CONST_METHOD1(type, QSharedPointer<qrtext::core::types::TypeExpression>(
			const QSharedPointer<qrtext::core::ast::Node> &expression));
	MOCK_CONST_METHOD0(errors, QList<qrtext::core::Error> &());
	MOCK_METHOD4(addIntrinsicFunction, void(const QString &name
			, qrtext::core::types::TypeExpression * const returnType
			, const QList<qrtext::core::types::TypeExpression *> &parameterTypes
			, std::function<QVariant(const QList<QVariant> &)> const &semantic));

	MOCK_CONST_METHOD0(variableTypes, QMap<QString, QSharedPointer<qrtext::core::types::TypeExpression>>());
	MOCK_CONST_METHOD0(specialIdentifiers, QStringList&());
	MOCK_CONST_METHOD0(specialConstants, QStringList&());
	MOCK_METHOD0(clear, void());
	MOCK_CONST_METHOD2(isGeneralization, bool(const QSharedPointer<qrtext::core::types::TypeExpression> &specific
			, const QSharedPointer<qrtext::core::types::TypeExpression> &general));

	// from DebuggerInterface
	MOCK_CONST_METHOD0(identifiers, QStringList());
	MOCK_METHOD1(interpret, QVariant(const QSharedPointer<qrtext::core::ast::Node> &root));
	//virtual QVariant value(const QString &identifier) const = 0;
	MOCK_CONST_METHOD1(value, QVariant(const QString &identifier));
	//virtual void setVariableValue(const QString &name, const QString &initCode, const QVariant &value) = 0;
	MOCK_METHOD3(setVariableValue, void(const QString &name, const QString &initCode, const QVariant &value));
};

}


