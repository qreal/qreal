#pragma once

#include <QtCore/QString>

#include <qrrepo/repoApi.h>
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace qReal {
namespace robots {
namespace generators {
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

		virtual QString convert(QString const &data) const = 0;
	};

	/// Can be overloaded for creating the converter that splits given string
	/// into multiple tokens and converts each of them
	class ROBOTS_GENERATOR_EXPORT MultiConverterInterface
	{
	public:
		virtual ~MultiConverterInterface();

		virtual QStringList convert(QString const &data) const = 0;
	};

	/// Doesn`t process input string returning itself
	class ROBOTS_GENERATOR_EXPORT EmptyConverter : public ConverterInterface
	{
	public:
		virtual QString convert(QString const &data) const;
	};

	/// Creates new instance of binding that simply replaces given label with a given value
	static Binding *createStatic(QString const &label, QString const &value);

	/// Creates new instance of binding to static string with a given converter
	static Binding *createStaticConverting(QString const &label, QString const &value
			, ConverterInterface const *converter);

	/// Creates new instance of binding without converting data form repo before applying
	static Binding *createDirect(QString const &label, QString const &property);

	/// Creates new instance of binding to a repo property with a given converter
	static Binding *createConverting(QString const &label, QString const &property
			, ConverterInterface const *converter);

	/// Creates new instance of binding that multiplies given data and substitutes
	/// into each converted by multiconverter property part
	static Binding *createMultiTarget(QString const &label, QString const &property
			, MultiConverterInterface const *converter);

	~Binding();

	/// Replaces all occurences of specified in constructor label with
	/// specified property value from repo with pre-converting it using
	/// specified converter.
	void apply(qrRepo::RepoApi const &repoApi
			, Id const &id, QString &data);

private:
	Binding(QString const &label, QString const &propertyOrValue, bool takeFromRepo);

	Binding(QString const &label, QString const &propertyOrValue, bool takeFromRepo
			, ConverterInterface const *converter);

	Binding(QString const &label, QString const &property
			, MultiConverterInterface const *converter);

	void applyMulti(QString const &property, QString &data);

	QString const mLabel;
	QString const mProperty;
	QString const mValue;
	ConverterInterface const *mConverter; // Takes ownership
	MultiConverterInterface const *mMultiConverter; // Takes ownership
};

}
}
}
}
