#include <Ice/Ice.h>
#include <repoclienticeI.h>

#include <QTextStream>
#include <QString>

#include "../../common/realrepoapiclasses.h"
#include "../../common/classes.h"
#include "../../repo/realrepoclient.h"

using namespace std;

namespace RepoServer
{
	using namespace RepoIce;
	typedef QList<int> QIntList;

	class RepoServer : virtual public Ice::Application {
	public:
		virtual int run(int, char * []) {
			// Terminate cleanly on receipt of a signal
			shutdownOnInterrupt();
			
			RepoClientIceI::_adapter = communicator()->createObjectAdapterWithEndpoints("RepoServerAdapter", "default -p 6667");
	
			Ice::ObjectPtr servant = new ::RepoIce::RepoClientIceI();
			RepoClientIceI::_adapter->add(servant, communicator()->stringToIdentity("RepoClientIce") );
			RepoClientIceI::_adapter->activate();
	
			communicator()->waitForShutdown();
			if (interrupted()) {
				cerr << appName() << ": received signal, shutting down" << endl;
			}

		RepoClientIceI::_adapter = 0;

		return 0;
		}
	};

	void doWork()
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
	}
}

int main(int argc, char* argv[])
{
	RepoServer::RepoServer app;
  return app.main(argc, argv);
}

