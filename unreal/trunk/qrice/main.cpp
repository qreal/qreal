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
	
			Ice::ObjectPtr servant = new ::RepoIce::RepoApiI();
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
	QString str = "Hello World!";
	out << str << endl;

	qrRepo::RepoApi repoApi;
	//Real::IdList children(qReal::Id const &id) const;
	qReal::IdList list = repoApi.children(qReal::ROOT_ID);
	out << repoApi.name(qReal::ROOT_ID) << " " << list.size() << endl;
	out << "String representation: " << qReal::ROOT_ID.toString() << endl;
	

	for (int i = 0; i < list.size(); ++i) 
		    out << list.at(i).toString() << endl;

	QVariant variant("12.66");

	out << variant.toDouble() << endl;

	RepoServer app;
	return app.main(argc, argv);
	//return 0;
}

