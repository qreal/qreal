#include "keyobjecttable.h"
#include "GeometricForms.h"
#include "adopter.h"
#include "levenshteindistance.h"

KeyObjectTable::KeyObjectTable(IKeyManager * keyManager)
{
    mKeyManager = keyManager;
}

KeyObjectTable::KeyObjectTable()
{
    mKeyManager = &mMouseMovementManager;
}

void KeyObjectTable::add(QString const & object, QList<QPoint> const & correctPath)
{
    KeyObjectItem keyObjectItem;
    keyObjectItem.object = object;
    keyObjectItem.correctPath = correctPath;
    keyObjectItem.key = mKeyManager->getKey(Adopter().getMousePath(correctPath));
    mKeyObjectTable.push_back(keyObjectItem);
}

void KeyObjectTable::setKeyManager(IKeyManager * keyManager)
{
    mKeyManager = keyManager;
    for (int i = 0; i < mKeyObjectTable.size(); i++)
    {
        mKeyObjectTable[i].key = mKeyManager->getKey(
                Adopter().getMousePath(mKeyObjectTable[i].correctPath));
    }
}

void KeyObjectTable::setPath(QString const & object, QList<QPoint> const & correctPath)
{
    if (mKeyObjectTable.isEmpty())
    {
        add(object, correctPath);
        return;
    }
    for (int i = 0; i < mKeyObjectTable.size(); i++)
    {
        if (mKeyObjectTable[i].object == object)
        {
            mKeyObjectTable[i].correctPath = correctPath;
            QString key = mKeyManager->getKey(Adopter().getMousePath(correctPath));
            mKeyObjectTable[i].key = key;
            return;
        }
    }
}

QString KeyObjectTable::getObject(QList<QPoint> const & path)
{
    int e = 100;
    int min = e;
    float distance;
    QString key = mKeyManager->getKey(path);
    QString object = "";
    foreach (KeyObjectItem item, mKeyObjectTable)
    {

        if (!item.key.isEmpty())
        {
            distance = (float)(LevenshteinDistance().getLevenshteinDistance(item.key, key) * e
                                / std::min(key.size(), item.key.size()));
            if (distance < min)
            {
                min = distance;
                object = item.object;
            }
        }
    }
    return object;
}

QString KeyObjectTable::getKey(QString const & name)
{
    return getItem(name).key;
}

QList<QPoint> KeyObjectTable::getPath(QString const & name)
{
    return getItem(name).correctPath;
}

KeyObjectItem KeyObjectTable::getItem(QString const & name)
{
    foreach (KeyObjectItem item, mKeyObjectTable)
    {
        if (item.object == name)
        {
            return item;
        }
    }
    KeyObjectItem item;
    return item;
}

