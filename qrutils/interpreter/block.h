#pragma once

#include <QtCore/QObject>
#include <QtGui/QColor>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <qrutils/interpreter/blockInterface.h>
#include <qrtext/languageToolboxInterface.h>

namespace qReal {
namespace interpretation {

/// Base class for all blocks implementations used in interpreter.
class QRUTILS_EXPORT Block : public BlockInterface
{
	Q_OBJECT

public:
	void interpret() override;

	void setFailedStatus() override;

	qReal::Id const id() const override;

	void finishedSteppingInto() override;

	/// Initializes this block with external components that may be used during the intepretetation process.
	/// @todo Not good.
	void init(qReal::Id const &graphicalId
			, qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface const &logicalModelApi
			, qReal::ErrorReporterInterface * const errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			);

protected:
	/// Enum with flags whether to report parser errors or let the language toolbox fail silently.
	enum class ReportErrors {
		/// Do report parser errors.
		report

		/// Do not report errors to user. Used when parser shall only try to parse a code (for example, in
		/// autoconfigurer).
		, doNotReport
	};

	Block();

	/// Returns a property of current block with given name as QVariant.
	QVariant property(QString const &propertyName) const;

	/// Returns a property of current block with given name as QString.
	QString stringProperty(QString const &propertyName) const;

	/// Returns a property of current block with given name as int, or 0 if it can't be converted to int.
	int intProperty(QString const &propertyName) const;

	/// Returns a property of current block with given name as bool, or "false" if it can't be converted to bool.
	bool boolProperty(QString const &propertyName) const;

	/// Returns a property with given name of block with given id as QVariant.
	QVariant property(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property with given name of block with given id as QString.
	QString stringProperty(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property with given name of block with given id as int, or 0 if it can't be converted to int.
	int intProperty(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property with given name of block with given id as bool, or "false" if it can't be converted to bool.
	bool boolProperty(qReal::Id const &id, QString const &propertyName) const;

	/// Returns a property of current block with given name as color.
	QColor propertyToColor(QString const &property) const;

	/// Reports error and emits "failure" signal.
	void error(QString const &message);

	/// Reports warning.
	void warning(QString const &message);

	/// Evaluates contents of a given property using text language interpreter and returns result.
	template<typename T>
	T eval(QString const &propertyName, ReportErrors reportErrors = ReportErrors::report)
	{
		return evalCode<T>(stringProperty(propertyName), propertyName, reportErrors);
	}

	/// Evaluates given code using text language interpreter and returns result.
	template<typename T>
	T evalCode(QString const &code, ReportErrors reportErrors = ReportErrors::report)
	{
		return evalCode<T>(code, "", reportErrors);
	}

	/// Evaluates given code using text language interpreter and returns result.
	/// @param code - code to evaluate.
	/// @param propertyName - name of corresponding property, used for connection.
	template<typename T>
	T evalCode(QString const &code, QString const &propertyName, ReportErrors reportErrors = ReportErrors::report)
	{
		T result = mParser->interpret<T>(mGraphicalId, propertyName, code);
		if (!mParser->errors().isEmpty() && reportErrors == ReportErrors::report) {
			reportParserErrors();
			emit failure();
			return result;
		}

		return result;
	}

	/// Evaluates given code using text language interpreter.
	void evalCode(QString const &code);

	/// Evaluates contents of a given property using text language interpreter.
	void eval(QString const &propertyName);

	/// Returns true, if there were parser/interpreter errors on last eval() call.
	bool errorsOccured() const;

	/// @todo: there is no such things as protected fields. State of a class shall not be directly available to
	/// descendants.
	qReal::Id mNextBlockId;
	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Doesn't have ownership.
	qReal::LogicalModelAssistInterface const *mLogicalModelApi;  // Doesn't have ownership.

	qReal::Id mGraphicalId;

private slots:
	void finishedRunning();

private:
	enum State {
		idle
		, running
		, failed
	};

	void reportParserErrors();

	/// Shall be reimplemented to set ids of next blocks. Default implementation covers usual sequential blocks, like
	/// "motors on", it is reimplemented in "if", "loop" and such kinds of blocks.
	virtual bool initNextBlocks();

	/// Shall be reimplemented to provide semantics of block execution.
	virtual void run() = 0;

	qrtext::LanguageToolboxInterface *mParser;  // Doesn't have ownership.

	State mState;
	qReal::ErrorReporterInterface *mErrorReporter;  // Doesn't have ownership.
};

}
}
