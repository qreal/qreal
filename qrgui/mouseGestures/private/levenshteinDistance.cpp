#include "levenshteinDistance.h"

#include <QtCore/QList>

using namespace qReal::gestures;

int LevenshteinDistance::getLevenshteinDistance(const QString &key1, const QString &key2)
{
	const int m = key1.size();
	const int n = key2.size();

	if (m == 0)
		return n;

	if (n == 0)
		return m;

	QList<QList<int> > matrix;
	for (int i = 0; i <= m; i++) {
		QList<int> list;
		for (int j = 0; j <= n; j++)
			list << 0;
		matrix << list;
	}

	for (int i = 0; i <= m; i++)
		for (int j = 0; j <= n; j++)
			if (i == 0 || j == 0)
				matrix[i][j] = std::max(i, j);

	matrix[0][0] = 0;
	int cost = 0;
	for (int i = 1; i <= m; ++i) {
		for(int j = 1; j <= n; ++j) {
			cost = (key1[i - 1] == key2[j - 1]) ? 0 : 1;
			const int aboveCell = matrix[i - 1][j];
			const int leftCell = matrix[i][j - 1];
			const int diagonalCell = matrix[i - 1][j - 1];
			matrix[i][j] = std::min(std::min(aboveCell + 1, leftCell + 1), diagonalCell + cost);
		}
	}
	return matrix[m][n];
}
