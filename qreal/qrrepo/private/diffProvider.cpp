#include "diffProvider.h"

#include <QFile>
#include <QDir>

using namespace qrRepo::details;

DiffProvider::DiffProvider(QString const &pathToClient)
	: mPathToClient(pathToClient), mDiffStream()
{

}

void DiffProvider::setPathToClient(const QString &pathToClient)
{
	mPathToClient = pathToClient;
}

QHash<Object*, Difference*> *DiffProvider::getDifference(QString const &workingCopy)
{
	QHash<Object*, Difference*> *result = new QHash<Object*, Difference*>();
	QString diffFileName("textDiff");

	QProcess *client = new QProcess(NULL);
	client->setStandardOutputFile(diffFileName);
	QStringList arguments;
	arguments << "diff" << workingCopy << "-r" << "HEAD";
	client->start(mPathToClient, arguments);
	client->waitForFinished();


	QFile textDiffFile(diffFileName);
	if (!textDiffFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return result;
	}
	mDiffStream.setDevice(&textDiffFile);

	mDiffStream.flush();
//	QDir dir;
//	dir.remove(diffFileName);
	return result;
}
