#include "levenshteindistance.h"

int LevenshteinDistance::getLevenshteinDistance(QString const & key1, QString const & key2)
{
    int m = key1.size();
    int n = key2.size();
    if (m == 0)
        return n;
    if (n == 0)
        return m;
    int matrix[m + 1][n + 1];
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0 || j ==0)
                matrix[i][j] = 1;
        }
    }
    matrix[0][0] = 0;
    int cost = 0;
    for (int i = 1; i <= m; ++i)
    {
        for(int j = 1; j <= n; ++j)
        {
            if (key1[i - 1] == key2[j - 1])
            {
                cost = 0;
            }
            else
            {
                cost = 1;
            }
            int aboveCell = matrix[i - 1][j];
            int leftCell = matrix[i][j - 1];
            int diagonalCell = matrix[i - 1][j - 1];
            matrix[i][j] = std::min(std::min(aboveCell + 1, leftCell + 1), diagonalCell + cost);
        }
    }
    return matrix[m][n];
}
