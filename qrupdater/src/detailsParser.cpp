#include "detailsParser.h"

DetailsParser::DetailsParser()
{
	mCurrentUpdate = new Update(this);
}

void DetailsParser::changeUnit(QString const unit)
{
	if (mCurrentUpdate->unit() == unit) {
		return;
	}
	mCurrentUpdate->setUnitName(unit);
	mCurrentUpdate->setData(
			QFileInfo(mFileUrls.value(unit).toString()).fileName()
			, mParamStrings.value(unit).split(" ")
			, mVersions.value(unit)
			, mFileUrls.value(unit)
	);
}

Update *DetailsParser::currentUpdate() const
{
	return mCurrentUpdate;
}

QStringList DetailsParser::units() const
{
	return mFileUrls.keys();
}

QString DetailsParser::currentUnit() const
{
	return mCurrentUpdate->unit();
}

