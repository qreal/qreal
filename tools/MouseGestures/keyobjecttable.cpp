#include "keyobjecttable.h"
#include "GeometricForms.h"
#include "pathcorrector.h"

KeyObjectTable::KeyObjectTable()
{
	mKeyBuilder = new MouseMovementManager();
}

KeyObjectItem KeyObjectTable::at(int pos)
{
	return mKeyObjectTable.at(pos);
}

int KeyObjectTable::size()
{
	return mKeyObjectTable.size();
}

Objects KeyObjectTable::getObjects()
{
	Objects objects;
	foreach (KeyObjectItem item, mKeyObjectTable)
	{
		objects.push_back(Object(item.object, item.correctPath));
	}
	return objects;
}

void KeyObjectTable::add(QString const & object, QList<QPoint> const & correctPath)
{
	QString key = mKeyBuilder.getKey(PathCorrector::getMousePath(correctPath));
	KeyObjectItem keyObjectItem(object, correctPath, key);
	mKeyObjectTable.push_back(keyObjectItem);
}

void KeyObjectTable::add(const Objects &objects)
{
	foreach (Object object, objects)
	{
		add(object.name, object.path);
	}
}

void KeyObjectTable::clear()
{
	mKeyObjectTable.clear();
}

void KeyObjectTable::setPath(QString const & object, QList<QPoint> const & correctPath)
{
	for (int i = 0; i < mKeyObjectTable.size(); i++)
	{
		if (mKeyObjectTable[i].object == object)
		{
			mKeyObjectTable[i].correctPath = correctPath;
			QString key = mKeyBuilder.getKey(PathCorrector::getMousePath(correctPath));
			mKeyObjectTable[i].key = key;
			return;
		}
	}
	add(object, correctPath);
}

QString KeyObjectTable::getObject(QList<QPoint> const & path)
{
	const float e = 100;
	const float maxKeyDistance = 50;
	float min = e;
	float distance;
	QString key = mKeyBuilder.getKey(path);
	QString object = "";
	if (key.isEmpty())
		return object;
	foreach (KeyObjectItem item, mKeyObjectTable)
	{
		if (!item.key.isEmpty())
		{
			distance = (float)(mKeyBuilder.getDistance(item.object) * e
								/ std::min(key.size(), item.key.size()));
			if (distance < min && distance < maxKeyDistance)
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
			return item;
	}
	return KeyObjectItem();
}
