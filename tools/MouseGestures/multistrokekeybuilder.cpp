#include "multistrokekeybuilder.h"
#include "pathcorrector.h"
#include <stdlib.h>
#include "cmath"
#include <QDebug>

static const int inMovement = 20;
static const int heightSize = 6;
static const int widthSize = 8;
static const int unidimensionalLimit = 3;
static const double adjacentWeight = 0.25;
static const double keyDistance = 5;

double MultistrokeGesturesManager::getMaxDistance()
{
	return keyDistance;
}

Key MultistrokeGesturesManager::sort(Key const & unsortedKey)
{
	Key key = unsortedKey;
	int size = key.size();
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j ++)
		{
			if (key.at(j).first < key.at(j + 1).first
				|| (key.at(j).second < key.at(j + 1).second
					&& key.at(j).first == key.at(j + 1).first))
			{
				key.insert(j, key.at(j+1));
				key.removeAt(j+2);
			}
		}
	}
	int k = 0;
	while (k < key.size() - 1)
	{
		if (key.at(k).first == key.at(k + 1).first
			&& key.at(k).second == key.at(k + 1).second)
			key.removeAt(k);
		else
			k++;
	}
	return key;
}

//double MultistrokeGesturesManager::getDistance(const Key &currentKey1, const Key &currentKey2)
//{
//	int m = currentKey1.size();
//	int n = currentKey2.size();
//	if (m == 0)
//		return m;
//	if (n == 0)
//		return n;
//	Key key1 = transferKey(currentKey1, n);
//	Key key2 = transferKey(currentKey2, m);
//	double dist = 0;
//	for (int i = 0; i < n * m; i++)
//	{
//		dist += norm(key1.at(i), key2.at(i));
//	}
//	return dist / (m * n);
//}

double MultistrokeGesturesManager::getDistance(const Key &key1, const Key &key2)
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
			if (i == 0 || j ==0 )
				matrix[i][j] = std::max(i, j);
		}
	}
	for (int i = 1; i <= m; ++i)
	{
		for(int j = 1; j <= n; ++j)
		{
			double norm = abs(key1[i - 1].first - key2[j - 1].first) +
						  abs(key1[i - 1].second - key2[j - 1].second);
			int aboveCell = matrix[i - 1][j];
			int leftCell = matrix[i][j - 1];
			int diagonalCell = matrix[i - 1][j - 1];
			matrix[i][j] = std::min((double) std::min(aboveCell + 3, leftCell + 3),
									diagonalCell + norm);
		}
	}
	return (double) (matrix[m][n] * (abs(m - n) + 1)) / std::min(n, m);
}

Key MultistrokeGesturesManager::transferKey(const Key &key, int size)
{
	Key newKey;
	foreach (SquarePos square, key)
	{
		for (int i = 0; i < size; i++)
			newKey.append(square);
	}
	return newKey;
}

double MultistrokeGesturesManager::norm(const SquarePos &pos1, const SquarePos &pos2)
{
	return std::max(abs(pos1.first - pos2.first), abs(pos1.second - pos2.second));
}

Key MultistrokeGesturesManager::getKey(const PointVector &path)
{
	analysePoints(path);
	Key key;
	SquarePos last;
	SquarePos previous(-1, -1);
	if ((mLowerBound - mUpperBound) / heightSize < unidimensionalLimit
		&& (mRightBound - mLeftBound) / widthSize < unidimensionalLimit)
		return key;
	foreach (QPoint point, path)
	{
		if((mLowerBound - mUpperBound) / heightSize < unidimensionalLimit)
		{
			last.first = (point.x() - mLeftBound) * widthSize / (mRightBound - mLeftBound);
			last.second = 0;
		}
		else if((mRightBound - mLeftBound) / widthSize < unidimensionalLimit)
		{
			last.first = 0;
			last.second = (point.y() - mUpperBound) * heightSize / (mLowerBound - mUpperBound);
		}
		else
		{
			last.first = (int)((point.x() - mLeftBound) * widthSize / (double)(mRightBound - mLeftBound));
			last.second = (int)((point.y() - mUpperBound) * heightSize / (double)(mLowerBound - mUpperBound));
		}
		if (previous != last)
		{
			previous = last;
			key.push_back(last);
		}
	}
	return sort(key);
}

void MultistrokeGesturesManager::analysePoints(QList<QPoint> const & path)
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
