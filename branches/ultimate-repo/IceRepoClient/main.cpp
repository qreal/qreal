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

	RealType realType;

	for (QIntList::const_iterator i = typesList.begin(); i != typesList.end(); i++)
	{
		realType = repoClient.getTypeById(*i);
		out << "Type N" << *i << " name: " << realType.getName() 
		<< " desc: " << realType.getDescription() 
		<< " metatype: " << realType.getMetaType() << endl;
	}
	
	return 0;
}

