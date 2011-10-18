#pragma once
#include "GeometricForms.h"

class Sorting
{
public:
    static Key sortPicture(Key const & unsortedKey)
    {
        Key key = unsortedKey;
        int size = key.size();
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = 0; j < size - i - 1; j++)
            {
                if (key.at(j).first > key.at(j + 1).first
                    || (key.at(j).second > key.at(j + 1).second
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
    static Key sortCurve(const Key & key)
    {
        Key sortedKey;
        Key unsortedKey = key;
        SquarePos pos(-1, -1);
        while (!unsortedKey.isEmpty())
        {
            pos = getNextPos(pos, unsortedKey);
            if (pos.first < 0 || pos.second < 0)
                pos = getMinPos(unsortedKey);
            sortedKey.push_back(pos);
            unsortedKey.removeOne(pos);
        }
        return sortedKey;
    }
private:
    static SquarePos getNextPos(const SquarePos &pos, const Key &key)
    {
        if (pos.first < 0 || pos.second < 0)
            return pos;
        SquarePos next(pos.first + 1, pos.second);
        if (key.contains(next))
            return next;
        next.second += 1;
        if (key.contains(next))
            return next;
        next.first -= 1;
        if (key.contains(next))
            return next;
        next.first -= 1;
        if (key.contains(next))
            return next;
        next.second -= 1;
        if (key.contains(next))
            return next;
        next.second -= 1;
        if (key.contains(next))
            return next;
        next.first += 1;
        if (key.contains(next))
            return next;
        next.first += 1;
        if (key.contains(next))
            return next;
        return SquarePos(-1, -1);
    }
    static SquarePos getMinPos(const Key &key)
    {
        SquarePos minPos;
        if (!key.isEmpty())
            minPos = key.at(0);
        foreach (SquarePos pos, key)
        {
            if (pos.first < minPos.first || (pos.first == minPos.first
                                             && pos.second < minPos.second))
                minPos = pos;
        }
        return minPos;
    }
};
