#pragma once

#include <QtCore/QString>

#include <logicalModelAssistInterface.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Binds property from repo to substring in the given string data
class Binding
{
public:
	/// Can be overriden for creating WPF-like bindings with converters
	class ConverterInterface
	{
	public:
		virtual ~ConverterInterface();

		virtual QString convert(QString const &data) const = 0;
	};

	// TODO: make it private
	class EmptyConverter : public ConverterInterface
	{
	public:
		virtual QString convert(QString const &data) const;
	};

	/// Creates new instance of binding without converting data in repo before applying
	Binding(QString const &label, QString const &property);

	/// Creates new instance of binding with given converter
	Binding(QString const &label, QString const &property
			, ConverterInterface const *converter);

	~Binding();

	/// Replaces all occurences of specified in constructor label with
	/// specified property value from repo with pre-converting it using
	/// specified converter.
	void apply(LogicalModelAssistInterface const &model
			, Id const &id, QString &data);

private:
	QString const mLabel;
	QString const mProperty;
	ConverterInterface const *mConverter; // Takes ownership
};

}
}
}
}
