#include "keyBuilder.h"
#include <QPoint>
#include <QString>

static const QString strBase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

MouseMovementManager::MouseMovementManager()
{
}

QString MouseMovementManager::getKey(QList<QPoint> const & path)
{
	analysePoints(path);
	QString key = "";
	QPoint pntLast (-1, -1);
	QPoint pntPath;
	if ((mLowerBound - mUpperBound) / iSize > unidimensionalLimit
		|| (mRightBound - mLeftBound) / iSize > unidimensionalLimit)
	{
		foreach (QPoint point, path)
		{
			if((mLowerBound - mUpperBound) / iSize < unidimensionalLimit)
			{
				pntPath.setX((point.x() - mLeftBound) * iSize / (mRightBound - mLeftBound));
				pntPath.setY(0);
			}
			else if((mRightBound - mLeftBound) / iSize < unidimensionalLimit)
			{
				pntPath.setX(0);
				pntPath.setY((point.y() - mUpperBound) * iSize / (mLowerBound - mUpperBound));
			}
			else
			{
				pntPath.setX((point.x() - mLeftBound) * iSize / (mRightBound - mLeftBound));
				pntPath.setY((point.y() - mUpperBound) * iSize / (mLowerBound - mUpperBound));
			}
			if(pntPath.x() != pntLast.x() || pntPath.y() != pntLast.y())
			{
				pntLast = pntPath;
				key += strBase64[pntPath.x() + pntPath.y() * iSize];
			}
		}
	}
	return key;
}

void MouseMovementManager::analysePoints(QList<QPoint> const & path)
{
	if (path.size() > 0)
	{
		mLeftBound = path[0].x();
		mRightBound = path[0].x();
		mUpperBound = path[0].y();
		mLowerBound = path[0].y();
	}
	foreach (QPoint pnt, path)
	{
		if (pnt.x() < mLeftBound) mLeftBound = pnt.x();
		if (pnt.x() > mRightBound) mRightBound = pnt.x();
		if (pnt.y() > mLowerBound) mLowerBound = pnt.y();
		if (pnt.y() < mUpperBound) mUpperBound = pnt.y();
	}
}

void MouseMovementManager::clear()
{
	mLeftBound = 0;
	mRightBound = 0;
	mUpperBound = 0;
	mLowerBound = 0;
}
