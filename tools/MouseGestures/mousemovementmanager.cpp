#include "mousemovementmanager.h"
#include "validpathcreator.h"
#include "pathcorrector.h"
#include <QPoint>
#include <QString>
#include <stdlib.h>

//using namespace std;

static const QString strBase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int inMovement = 20;
static const int iSize = 7;
static const int unidimensionalLimit = 4;
static const double adjacentWeight = 0.25;

MouseMovementManager::MouseMovementManager()
{
	createWeights();
}

double MouseMovementManager::getMaxDistance()
{
	return 50;
}

QString MouseMovementManager::getKey(const PathVector &components)
{
	QList<QPoint> gesture = ValidPathCreator::createPath(components);
	QList<QPoint> path = PathCorrector::correctPath(gesture);
	return getKey(path);
}

//todo:: возможно оставить только создание весов в классе, а нахождение расстояния Левенштейна вынести
void MouseMovementManager::createWeights()
{
	for (int i = 0; i < iSize + 1; i++)
	{
		for (int j = 0 ; j < iSize + 1; j++)
		{
			int letterIndex = strIndex(i, j);
			for (int g = 0; g < strBase64.size(); g++)
			{
				QPair<QChar, QChar> letters(strBase64[letterIndex], strBase64[g]);
				mWeights.insert(letters, 1);
			}
			QPair<QChar, QChar> equalSymbols(strBase64[letterIndex], strBase64[letterIndex]);
			mWeights.insert(equalSymbols, 0);
			QPair<QChar, QChar> adjacentSymbols(strBase64[letterIndex], strBase64[letterIndex]);
			//todo:: убить CopyPast
			if (i > 0)
			{
				int adjacentIndex = strIndex(i - 1, j);
				adjacentSymbols.second = strBase64[adjacentIndex];
				mWeights.insert(adjacentSymbols, adjacentWeight);
			}
			if (i < iSize)
			{
				int adjacentIndex = strIndex(i + 1, j);
				adjacentSymbols.second = strBase64[adjacentIndex];
				mWeights.insert(adjacentSymbols, adjacentWeight);
			}
			if (j > 0)
			{
				int adjacentIndex = strIndex(i, j - 1);
				adjacentSymbols.second = strBase64[adjacentIndex];
				mWeights.insert(adjacentSymbols, adjacentWeight);
			}
			if (j < iSize)
			{
				int adjacentIndex = strIndex(i, j + 1);
				adjacentSymbols.second = strBase64[adjacentIndex];
				mWeights.insert(adjacentSymbols, adjacentWeight);
			}
		}
	}
}

int MouseMovementManager::strIndex(int row, int column)
{
	return row * (iSize + 1) + column;
}

double MouseMovementManager::getDistance(const QString &key1, const QString &key2)
{
	int m = key1.size();
	int n = key2.size();
	if (m == 0)
		return n;
	if (n == 0)
		return m;
	double matrix[m + 1][n + 1];
	for (int i = 0; i <= m; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			if (i == 0 || j ==0)
				matrix[i][j] = 1;
		}
	}
	matrix[0][0] = 0;
	for (int i = 1; i <= m; ++i)
	{
		for(int j = 1; j <= n; ++j)
		{
			QPair<QChar, QChar> letters(key1[i-1], key2[j-1]);
			int aboveCell = matrix[i - 1][j];
			int leftCell = matrix[i][j - 1];
			int diagonalCell = matrix[i - 1][j - 1];
			matrix[i][j] = std::min((double) std::min(aboveCell + 1, leftCell + 1),
									diagonalCell + mWeights[letters]);
		}
	}
	return (float) matrix[m][n] * 100 / std::min(m, n);
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
