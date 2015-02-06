#include "version.h"

#include <QtCore/QStringList>

using namespace qReal;

Version Version::fromString(const QString &version)
{
	const QStringList dashParts = version.split("-", QString::SkipEmptyParts);
	const QStringList spaceParts = version.split(" ", QString::SkipEmptyParts);
	if (dashParts.isEmpty() || spaceParts.isEmpty() || dashParts.count() > 2 || spaceParts.count() > 2) {
		return Version();
	}

	const QStringList parts = dashParts.count() == 2 ? dashParts : spaceParts;
	const QString prefix = parts[0];
	const QString suffix = parts.count() == 1 ? QString() : parts[1];

	bool success = true;
	bool currentIsOk;

	const QStringList numbers = prefix.split(".", QString::SkipEmptyParts);
	if (numbers.isEmpty() || numbers.count() > 3) {
		return Version();
	}

	const QString majorString = numbers[0];
	const QString minorString = numbers.count() > 1 ? numbers[1] : "0";
	const QString buildString = numbers.count() > 2 ? numbers[2] : "0";

	const int major = majorString.toInt(&currentIsOk);
	success &= currentIsOk;
	const int minor = minorString.toInt(&currentIsOk);
	success &= currentIsOk;
	const int build = buildString.toInt(&currentIsOk);
	success &= currentIsOk;

	if (suffix.isEmpty() && success) {
		return Version(major, minor, build);
	}

	// Splitting string into two parts: first is maximum letter prefix, second is remaining part.
	QString letterSuffix;
	QString numberSuffix;
	bool iteratingFirstPart = true;
	for (QChar const ch : suffix) {
		if (!ch.isLetter() && iteratingFirstPart) {
			iteratingFirstPart = false;
		}

		if (iteratingFirstPart) {
			letterSuffix.append(ch);
		} else {
			numberSuffix.append(ch);
		}
	}

	if (numberSuffix.isEmpty()) {
		numberSuffix = "0";
	}

	Stage const stage = parseStage(letterSuffix, currentIsOk);
	success &= currentIsOk;
	const int stageNumber = numberSuffix.toInt(&currentIsOk);
	success &= currentIsOk;

	if (!success) {
		return Version();
	}

	return Version(major, minor, build, stage, stageNumber);
}

Version::Stage Version::parseStage(const QString &stage, bool &ok)
{
	Stage result = stable;
	ok = false;
	if (stage.toLower() == "a" || stage.toLower() == "alpha") {
		ok = true;
		result = alpha;
	} else if (stage.toLower() == "b" || stage.toLower() == "beta") {
		ok = true;
		result = beta;
	} else if (stage.toLower() == "rc" || stage.toLower() == "releaseCandidate") {
		ok = true;
		result = releaseCandidate;
	}

	return result;
}

Version::Version()
	: mMajor(-1)
	, mMinor(-1)
	, mBuild(-1)
	, mStage(stable)
	, mStageNumber(-1)
{
}

Version::Version(int major, int minor, int build, Version::Stage stage, int stageNumber)
	: mMajor(major)
	, mMinor(minor)
	, mBuild(build)
	, mStage(stage)
	, mStageNumber(stageNumber)
{
}

bool Version::isValid() const
{
	return mMajor >= 0;
}

int qReal::Version::major() const
{
	return mMajor;
}

int qReal::Version::minor() const
{
	return mMinor;
}

int Version::build() const
{
	return mBuild;
}

Version::Stage Version::stage() const
{
	return mStage;
}

int Version::stageNumber() const
{
	return mStageNumber;
}

QString Version::toString() const
{
	return QString("%1.%2.%3").arg(QString::number(mMajor)
			, QString::number(mMinor), QString::number(mBuild)) + suffixString();
}

QString Version::suffixString() const
{
	switch (mStage) {
	case alpha:
		return QString("-alpha%1").arg(QString::number(mStageNumber));
	case beta:
		return QString("-beta%1").arg(QString::number(mStageNumber));
	case releaseCandidate:
		return QString("-rc%1").arg(QString::number(mStageNumber));
	default:
		return QString();
	}
}
