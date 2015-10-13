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
#include <QtCore/QHash>
#include <QtCore/QDebug>
#include <QtCore/QMetaType>

#include "kernelDeclSpec.h"

namespace qReal {

/// Represents a version of an editor. String representation is expected to consist
/// of four parts: <Major[.Minor[.Build[-Stage[Number]]]]>.
/// Major, Minor, Build and Number parts are simply integer numbers.
/// Stage is one of "alpha" (or "a"), "beta" (or "b") or "rc" markers.
/// Stage part is also can be separated by spaces from the number part.
/// Minor, Build, Stage and Number parts are optional.
///
/// Versions can be compared each to other. Version1 is less than Version2
/// if Version1 was released earlier than Version2. Examples:
///
/// 2.5.3 < 3.0.0
/// 3.0.0-a < 3.0.0-rc1
/// 3.0.0-rc1 < 3.0.0
/// 3.0.0 == 3
class QRKERNEL_EXPORT Version
{
public:
	/// Represents release stage.
	enum Stage
	{
		/// Testing inside team.
		alpha = 0
		/// Testing inside community.
		, beta
		/// Version that seems to be stable.
		, releaseCandidate
		/// Stable version, used when no suffix specified.
		, stable
	};

	/// Constructs new Version instance from a given string.
	/// Version() is returned when input string could not be parsed.
	static Version fromString(const QString &version);

	/// Constructs invalid version instance.
	Version();

	/// Constructs a version object from a number parts. For constructing a version object from string
	/// use fromString() method.
	/// @see fromString()
	Version(int major, int minor = 0, int build = 0, Stage stage = stable, int stageNumber = 0);

	/// Returns true if version string was accepted by versions parser.
	bool isValid() const;

	/// Returns a major part of the version (first part)
	int major() const;

	/// Returns a major part of the version (second part). 0 is returned when no minor version was specified.
	int minor() const;

	/// Returns a build part of the version (third part). 0 is returned when no build version was specified.
	int build() const;

	/// Returns a stage part of the version (suffix part). 'stable' is returned when no stage was specified.
	Stage stage() const;

	/// Returns a stage number part of the version (optional number in stage suffix).
	/// 0 is returned when no stage version was specified.
	int stageNumber() const;

	/// Returns version in <Major.Minor.Build-[StageNumber]> form.
	/// @warning: returned by this method version string may differ form input one.
	QString toString() const;

private:
	static Stage parseStage(const QString &stage, bool &ok);
	QString suffixString() const;

	int mMajor;
	int mMinor;
	int mBuild;
	Stage mStage;
	int mStageNumber;
};

/// Versions equality operator. Versions are equal when all their parts are equal.
inline bool operator==(const Version &v1, const Version &v2)
{
	return v1.major() == v2.major()
			&& v1.minor() == v2.minor()
			&& v1.build() == v2.build()
			&& v1.stage() == v2.stage()
			&& v1.stageNumber() == v2.stageNumber();
}

/// Version inequality operator.
inline bool operator!=(const Version &v1, const Version &v2)
{
	return !(v1 == v2);
}

/// Less comparison operator for Version class
inline bool operator<(const Version &v1, const Version &v2)
{
	return v1.major() != v2.major() ? v1.major() < v2.major()
			: v1.minor() != v2.minor() ? v1.minor() < v2.minor()
			: v1.build() != v2.build() ? v1.build() < v2.build()
			: v1.stage() != v2.stage() ? v1.stage() < v2.stage()
			: v1.stageNumber() < v2.stageNumber();
}

/// Greater comparison operator for Version class
inline bool operator>(const Version &v1, const Version &v2)
{
	return !(v1 == v2) && !(v1 < v2);
}

/// Less or equal comparison operator for Version class
inline bool operator<=(const Version &v1, const Version &v2)
{
	return v1 == v2 || v1 < v2;
}

/// Greater or equal comparison operator for Version class
inline bool operator>=(const Version &v1, const Version &v2)
{
	return !(v1 < v2);
}

/// Version hash function for using it in QHash.
inline uint qHash(const Version &key)
{
	return qHash(key.toString());
}

/// Operator for printing Version into QDebug.
inline QDebug operator<<(QDebug &dbg, const Version &version)
{
	dbg << version.toString();
	return dbg.space();
}

}

Q_DECLARE_METATYPE(qReal::Version)

QRKERNEL_EXPORT QDataStream& operator<< (QDataStream &out, const qReal::Version &version);
QRKERNEL_EXPORT QDataStream& operator>> (QDataStream &in, qReal::Version &version);
