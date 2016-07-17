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

#include "keyManager.h"

#include <QtCore/QPointF>
#include <QtCore/QString>

static const QString strBase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

using namespace qReal::gestures;

KeyManager::KeyManager()
{
}

QString KeyManager::getKey(QList<QPoint> const &path)
{
	analysePoints(path);
	QString key = "";
	QPoint pntLast (-1, -1);
	QPoint pntPath;
	if ((mLowerBound - mUpperBound) / iSize > unidimensionalLimit
			|| (mRightBound - mLeftBound) / iSize > unidimensionalLimit
	) {
		for (const QPoint &point : path) {
			if((mLowerBound - mUpperBound) / iSize < unidimensionalLimit) {
				pntPath.setX((point.x() - mLeftBound) * iSize / (mRightBound - mLeftBound));
				pntPath.setY(0);
			}
			else if((mRightBound - mLeftBound) / iSize < unidimensionalLimit) {
				pntPath.setX(0);
				pntPath.setY((point.y() - mUpperBound) * iSize / (mLowerBound - mUpperBound));
			}
			else {
				pntPath.setX((point.x() - mLeftBound) * iSize / (mRightBound - mLeftBound));
				pntPath.setY((point.y() - mUpperBound) * iSize / (mLowerBound - mUpperBound));
			}
			if(pntPath.x() != pntLast.x() || pntPath.y() != pntLast.y()) {
				pntLast = pntPath;
				key += strBase64[pntPath.x() + pntPath.y() * iSize];
			}
		}
	}
	return key;
}

void KeyManager::analysePoints(QList<QPoint> const &path)
{
	if (path.size() > 0) {
		mLeftBound = path[0].x();
		mRightBound = path[0].x();
		mUpperBound = path[0].y();
		mLowerBound = path[0].y();
	}
	for (const QPoint &pnt : path) {
		if (pnt.x() < mLeftBound)
			mLeftBound = pnt.x();
		if (pnt.x() > mRightBound)
			mRightBound = pnt.x();
		if (pnt.y() > mLowerBound)
			mLowerBound = pnt.y();
		if (pnt.y() < mUpperBound)
			mUpperBound = pnt.y();
	}
}

void KeyManager::clear()
{
	mLeftBound = 0;
	mRightBound = 0;
	mUpperBound = 0;
	mLowerBound = 0;
}
