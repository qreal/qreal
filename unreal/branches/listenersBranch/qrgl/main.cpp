#include "../qrrepo/repoApi.h"

#include <QDebug>

int main()
{
	qrRepo::RepoApi rApi("..");
	
	qReal::IdList classElementsList = rApi.elementsByType("Class");

	foreach (qReal::Id id, classElementsList) {
		qDebug() << rApi.name(id);
		foreach (qReal::Id containersId, rApi.children(id)) {
			qDebug() << "\t" << containersId.element();
			foreach (qReal::Id methodsAndFieldsId, rApi.children(containersId)) {
				qDebug() << "\t\t" << rApi.name(methodsAndFieldsId);
			}
		}
	}

	return 0;
}
