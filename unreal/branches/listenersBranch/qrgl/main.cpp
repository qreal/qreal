#include "../qrrepo/repoApi.h"

#include <QDebug>

int main()
{
	qrRepo::RepoApi rApi("../save");
	/*
	qReal::IdList classElementsList = rApi.elementsByType("Class");
	foreach (qReal::Id id, classElementsList) {
		qDebug() << rApi.name(id);
	}
	*/

	return 0;
}
