#pragma once
#include "GeometricForms.h"
#include "cmath"

class Distance
{
public:
    static double getLevenshteinDistance(Key const & key1, Key const & key2)
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
                double dist = norm(key1[i - 1], key2[j - 1]);
                int aboveCell = matrix[i - 1][j];
                int leftCell = matrix[i][j - 1];
                int diagonalCell = matrix[i - 1][j - 1];
                matrix[i][j] = std::min((double) std::min(aboveCell + 3, leftCell + 3),
                                        diagonalCell + dist);
            }
        }
        return (double) (matrix[m][n] * (abs(m - n) + 1)) / std::min(n, m);
    }
    static double getCurveDistance(const Key & currentKey1, const Key & currentKey2)
    {
        int m = currentKey1.size();
        int n = currentKey2.size();
        if (m == 0)
            return m;
        if (n == 0)
            return n;
        Key key1 = transferKey(currentKey1, n);
        Key key2 = transferKey(currentKey2, m);
        double dist = 0;
        for (int i = 0; i < n * m; i++)
        {
            dist += norm(key1.at(i), key2.at(i));
        }
        return dist / (m * n);
    }
    static double getL1Distance(const Key & currentKey1, const Key & currentKey2)
    {
        return std::max(getDist(currentKey1, currentKey2), getDist(currentKey2, currentKey1));
                /// (currentKey1.size() + currentKey2.size());
    }
    static double norm(const SquarePos &pos1, const SquarePos &pos2)
    {
        return abs(pos1.first - pos2.first) + abs(pos1.second - pos2.second);
    }
private:
    static Key transferKey(const Key &key, int size)
    {
        Key newKey;
        foreach (SquarePos square, key)
        {
            for (int i = 0; i < size; i++)
                newKey.append(square);
        }
        return newKey;
    }

    //todo:: rename
    static double getDist(const Key & key, const Key & figure)
    {
        double figureDist = 0;
        if (key.isEmpty())
            return 0;
        foreach (SquarePos position, figure)
        {
            double dist = norm(position, key.at(0));
            foreach (SquarePos pos, key)
            {
                if (norm(pos, position) < dist)
                    dist = norm(pos, position);
            }
            figureDist = std::max(dist, figureDist);
        }
        return figureDist;
    }
};
