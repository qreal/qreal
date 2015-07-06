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

#include <functional>

#include <qrkernel/version.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>

namespace qReal {

/// A converter that transforms models from some version to later one.
class ProjectConverter
{
public:
	/// Represent a result of project convertion process.
	enum ConvertionResult
	{
		/// Convertion process made some modifications, finished successfully, the save file is now has valid contents.
		Success = 0
		/// Convertion process finished successfully, no modifications were made.
		, NoModificationsMade
		/// Convertion can`t be performed because of too old save version, convertion rules are unknown.
		, VersionTooOld
		/// Convertion can`t be completed because save file has unexpected contents.
		, SaveInvalid
	};

	typedef std::function<ConvertionResult(GraphicalModelAssistInterface &
			, LogicalModelAssistInterface &)> Converter;

	ProjectConverter(const QString &editor
			, const Version &fromVersion
			, const Version &toVersion
			, const Converter &converter)
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
	ConvertionResult convert(GraphicalModelAssistInterface &graphicalApi
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
