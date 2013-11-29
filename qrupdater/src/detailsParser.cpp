#include "detailsParser.h"

DetailsParser::DetailsParser()
{
}

Update* DetailsParser::udpate(QString const unit)
{
	for (int i = 0; i < mUpdates.size(); i++) {
		if (mUpdates.at(i)->unit() == unit) {
			return mUpdates.at(i);
		}
	}
	return NULL;
}

QStringList DetailsParser::units() const
{
	return mFileUrls.keys();
}

QList<Update *> DetailsParser::updatesParsed() const
{
	return mUpdates;
}

void DetailsParser::processDevice(QIODevice *device)
{
	parseDevice(device);

	foreach (QString const unit, units()) {
		Update *newUpdate = new Update(this);
		newUpdate->setUnitName(unit);
		newUpdate->setData(
				QFileInfo(mFileUrls.value(unit).toString()).fileName()
				, mParamStrings.value(unit).split(" ")
				, mVersions.value(unit)
				, mFileUrls.value(unit)
		);

		mUpdates << newUpdate;
	}
}

