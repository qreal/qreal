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

#pragma once

#include <QtCore/QStringList>

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

/// Reresents function type.
class Function : public core::types::TypeExpression
{
public:
	/// Constructor.
	/// @param returnType - type expression denoting return type of a function.
	/// @param formalParameters - a list of formal parameter types.
	Function(QSharedPointer<core::types::TypeExpression> const &returnType
			, QList<QSharedPointer<core::types::TypeExpression>> const &formalParameters)
		: mReturnType(returnType), mFormalParameters(formalParameters)
	{
	}

	/// Returns return type of a function.
	QSharedPointer<core::types::TypeExpression> const &returnType() const
	{
		return mReturnType;
	}

	/// Returns a list of types of formal parameters.
	QList<QSharedPointer<core::types::TypeExpression>> const &formalParameters() const
	{
		return mFormalParameters;
	}

	QString toString() const override
	{
		QStringList parameterTypes;
		for (QSharedPointer<core::types::TypeExpression> const &type : mFormalParameters) {
			parameterTypes << type->toString();
		}

		return QString("%1(%2)").arg(mReturnType->toString()).arg(parameterTypes.join(", ")) +  QObject::tr("");
	}


private:
	QSharedPointer<core::types::TypeExpression> mReturnType;
	QList<QSharedPointer<core::types::TypeExpression>> mFormalParameters;
};

}
}
}
