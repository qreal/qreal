#pragma once

#include <functional>

#include <qrkernel/version.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>

namespace qReal {

/// A converter that transforms models from some version to later one.
class ProjectConverter
{
public:
	/// Represent a result of project convertion process.
	enum ConvertionResult
	{
		/// Convertion finished successfully, the save file is now has valid contents.
		Success = 0
		/// Convertion can`t be performed because of too old save version, convertion rules are unknown.
		, VersionTooOld
		/// Convertion can`t be completed because save file has unexpected contents.
		, SaveInvalid
	};

	typedef std::function<ConvertionResult(GraphicalModelAssistInterface const &
			, LogicalModelAssistInterface &)> Converter;

	ProjectConverter(QString const &editor
			, Version const &fromVersion
			, Version const &toVersion
			, Converter const &converter)
		: mEditor(editor)
		, mFromVersion(fromVersion)
		, mToVersion(toVersion)
		, mConverter(converter)
	{
	}

	/// Returns an editor whoose diagrams will be converted by this converter instance.
	QString editor() const
	{
		return mEditor;
	}

	/// Returns a version of saves that would be transformed.
	Version fromVersion() const
	{
		return mFromVersion;
	}

	/// Returns a version that saves would have after transformation.
	Version toVersion() const
	{
		return mToVersion;
	}

	/// Performs conversion process and returns the success or the fail reason of this operation.
	/// If operation was unsuccessful then the whole save is not accepted by the system
	/// and corresponding error message will be shown.
	ConvertionResult convert(GraphicalModelAssistInterface const &graphicalApi
			, LogicalModelAssistInterface &logicalApi)
	{
		return mConverter(graphicalApi, logicalApi);
	}

private:
	QString mEditor;
	Version mFromVersion;
	Version mToVersion;
	Converter mConverter;
};

}
