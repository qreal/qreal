#include "../common/realrepoapiclasses.h"
#include "../common/classes.h"
#include "../repo/realrepoclient.h"

#include <QTextStream>
#include <QString>

int main()
{
	QTextStream out(stdout);
	RealRepoClient realRepoClient;
	out << "Connected to repo\n";
	sleep(2);
	return 0;
}

