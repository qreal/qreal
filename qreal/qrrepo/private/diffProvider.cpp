#include "diffProvider.h"

using namespace qrRepo::details;

DiffProvider::DiffProvider(QString const &pathToClient)
	: mPathToClient(pathToClient), mDiffStream()
{

}

void DiffProvider::setPathToClient(const QString &pathToClient)
{
	mPathToClient = pathToClient;
}

QHash<qReal::Id, Difference*> *DiffProvider::getDifference(QHash<qReal::Id, Object*> const &repoObjects, QHash<qReal::Id, Object*> const &workingCopyObjects)
{
	QHash<qReal::Id, Difference*> *result = new QHash<qReal::Id, Difference*>();

	QHashIterator<qReal::Id, Object*> i(repoObjects);
	while (i.hasNext())
	{
		i.next();
		if (workingCopyObjects.contains(i.key()))
		{
			result->insert(i.key(), new Difference(i.value(), workingCopyObjects.value(i.key())));
		}
		else
		{
			result->insert(i.key(), new Difference(i.value(), Removed));
		}
	}
	QHashIterator<qReal::Id, Object*> j(workingCopyObjects);
	while (j.hasNext())
	{
		j.next();
		if (!result->contains(j.key()))
		{
			result->insert(j.key(), new Difference(j.value(), Added));
		}
	}
	return result;
}
