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

#include <QtCore/QString>

#include <qrrepo/repoApi.h>
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace simple {

/// Binds property from repo or a static string to a given substring of a given data
class ROBOTS_GENERATOR_EXPORT Binding
{
public:
	/// Can be overriden for creating WPF-like bindings with converters
	class ROBOTS_GENERATOR_EXPORT ConverterInterface
	{
	public:
		virtual ~ConverterInterface();

		virtual QString convert(const QString &data) const = 0;
	};

	/// Can be overloaded for creating the converter that splits given string
	/// into multiple tokens and converts each of them
	class ROBOTS_GENERATOR_EXPORT MultiConverterInterface
	{
	public:
		virtual ~MultiConverterInterface();

		virtual QStringList convert(const QString &data) const = 0;
	};

	/// Doesn`t process input string returning itself
	class ROBOTS_GENERATOR_EXPORT EmptyConverter : public ConverterInterface
	{
	public:
		virtual QString convert(const QString &data) const;
	};

	/// Creates new instance of binding that simply replaces given label with a given value
	static Binding *createStatic(const QString &label, const QString &value);

	/// Creates new instance of binding to static string with a given converter
	static Binding *createStaticConverting(const QString &label, const QString &value
			, const ConverterInterface *converter);

	/// Creates new instance of binding without converting data form repo before applying
	static Binding *createDirect(const QString &label, const QString &property);

	/// Creates new instance of binding to a repo property with a given converter
	static Binding *createConverting(const QString &label, const QString &property
			, const ConverterInterface *converter);

	/// Creates new instance of binding that multiplies given data and substitutes
	/// into each converted by multiconverter property part
	static Binding *createMultiTarget(const QString &label, const QString &property
			, const MultiConverterInterface *converter);

	~Binding();

	/// Replaces all occurences of specified in constructor label with
	/// specified property value from repo with pre-converting it using
	/// specified converter.
	void apply(const qrRepo::RepoApi &repoApi
			, const qReal::Id &id, QString &data);

private:
	Binding(const QString &label, const QString &propertyOrValue, bool takeFromRepo);

	Binding(const QString &label, const QString &propertyOrValue, bool takeFromRepo
			, const ConverterInterface *converter);

	Binding(const QString &label, const QString &property
			, const MultiConverterInterface *converter);

	void applyMulti(const QString &property, QString &data);

	const QString mLabel;
	const QString mProperty;
	const QString mValue;
	const ConverterInterface *mConverter; // Takes ownership
	const MultiConverterInterface *mMultiConverter; // Takes ownership
};

}
}
