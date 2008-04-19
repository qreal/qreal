#include "../common/realrepoapiclasses.h"
#include "../common/classes.h"
#include "../repo/realrepoclient.h"

#include <QTextStream>
#include <QString>

int main()
{
	QTextStream out(stdout);
	RealRepoClient repoClient;

	out << "getTypesCount: " << repoClient.getTypesCount() << endl;
	QIntList typesList = repoClient.getAllTypes();
	out << "count: " << typesList.count() << endl;
	for (QIntList::const_iterator i = typesList.begin(); i != typesList.end(); i++)
	{
		RealType* realType = repoClient.getTypeById(*i);
		//out << "Type N" << *i << " has name: " << realType->getName() << endl;
		delete realType;
	}
	
	return 0;
}

