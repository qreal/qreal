#include "../Model/Model.h"
#include "../Service/definitions.h"

#include <QtTest>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMimeData>
#include <QByteArray>
#include <QString>
#include <QPersistentModelIndex>
#include <QVariant>
#include <QMetaType>

using namespace qReal;

using namespace model;

using namespace QTest;

Q_DECLARE_METATYPE(QModelIndex)

class ModelFirstTest : public QObject
{
	Q_OBJECT

	private slots:
		void addItem1();
		void mimeData_data();
		void mimeData();
};


void ModelFirstTest::addItem1()
{	
	Model *model = new Model();
	model->test();
	QCOMPARE(model->data(model->index(0,0)).toString(),QString("anon"));
	delete model;
}

void ModelFirstTest::mimeData_data()
{
	addColumn<QModelIndex>("index");
	addColumn<QString>("expectedId");
	newRow("root") << QModelIndex() << ROOT_ID;
}

void ModelFirstTest::mimeData()
{
	Model *model = new Model();
	model->test();
	QModelIndexList list;
	QFETCH(QModelIndex,index);
	QFETCH(QString,expectedId);
	list << index;
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
	QCOMPARE(id,expectedId);
//	QCOMPARE(pathToItem,QString());
//	QCOMPARE(name,ROOT_ID);	
//	QCOMPARE(position,QPointF());
    delete model;
	
}

QTEST_MAIN(ModelFirstTest)
#include "ModelFirstTest.moc"