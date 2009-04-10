#include "../Model/Model.h"
#include "../Service/definitions.h"

#include <QtTest>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMimeData>
#include <QByteArray>

using namespace qReal;

using namespace model;

class ModelFirstTest : public QObject
{
	Q_OBJECT

	private slots:
		void addItem1();
		void mimeDataRoot();
};


void ModelFirstTest::addItem1()
{	
	Model *model = new Model();
	model->test();
	QCOMPARE(model->data(model->index(0,0)).toString(),QString("anon"));
	delete model;
}

void ModelFirstTest::mimeDataRoot()
{
	Model *model = new Model();
	model->test();
	QModelIndexList list;
	list << QModelIndex();
	QMimeData *mimeData = model->mimeData(list);
	QByteArray data = mimeData->data(DEFAULT_MIME_TYPE);
	QDataStream stream(&data, QIODevice::ReadOnly);
	IdType id;
	PropertyName pathToItem;
	QString name;
	QPointF position;
	stream >> id;
	stream >> pathToItem;
	stream >> name;
	stream >> position;
	QCOMPARE(id,ROOT_ID);
	QCOMPARE(pathToItem,QString());
	QCOMPARE(name,ROOT_ID);	
	QCOMPARE(position,QPointF());
	delete model;
}

QTEST_MAIN(ModelFirstTest)
#include "ModelFirstTest.moc"