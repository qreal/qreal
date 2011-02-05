#include "../qrrepo/repoApi.h"
#include "../qrgui/kernel/definitions.h"
#include <QString>
#include <QTextStream>
#include <QVariant>
#include <QtDebug>

#include <Ice/Ice.h>
#include <repoclienticeI.h>

//using namespace ;
using namespace RepoIce;


class RepoServer : virtual public Ice::Application {
	public:
		virtual int run(int, char * []) {
			// Terminate cleanly on receipt of a signal
			shutdownOnInterrupt();
			
			RepoApiI::_adapter = communicator()->createObjectAdapterWithEndpoints("RepoApiAdapter", "default -p 6667");
	
			qrRepo::RepoApi repoApi("../qrgui/save");
			Ice::ObjectPtr servant = new ::RepoIce::RepoApiI(repoApi);
			RepoApiI::_adapter->add(servant, communicator()->stringToIdentity("RepoApi") );
			RepoApiI::_adapter->activate();
	
			communicator()->waitForShutdown();
			if (interrupted()) {
				qDebug() << appName() << ": received signal, shutting down" << endl;
			}

			RepoApiI::_adapter = 0;

			return 0;
		}
};

int main(int argc, char **argv)
{
	QTextStream out(stdout);

	qrRepo::RepoApi repoApi("../qrgui/save");

	qReal::Id diagramId = repoApi.children(qReal::ROOT_ID).at(0);

	qReal::IdList list = repoApi.children(diagramId);
	out << repoApi.name(diagramId) << " " << list.size() << endl;
	out << "String representation: " << diagramId.toString() << endl;
	

	for (int i = 0; i < list.size(); ++i) 
		    out << list.at(i).toString() << endl;

	QVariant variant("12.66");

	out << variant.toDouble() << endl;

	RepoServer app;
	return app.main(argc, argv);
	return 0;
}

