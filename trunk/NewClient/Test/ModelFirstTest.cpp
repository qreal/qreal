#include "SubModel.h"
#include "../Service/definitions.h"

#include <QtTest>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMimeData>
#include <QString>
#include <QMetaType>

using namespace QTest;

Q_DECLARE_METATYPE(QModelIndex)

namespace qReal {

	namespace test {

		class ModelFirstTest : public QObject
		{
			Q_OBJECT

		private:
			SubModel *model;
			void addTestItems();

		private slots:
			void initTestCase();
			void data_data();
			void data();
			void mimeData_data();
			void mimeData();
			void cleanupTestCase();
		};


		void ModelFirstTest::initTestCase()
		{
			model = new SubModel();
			addTestItems();
		}

		void ModelFirstTest::addTestItems()
		{
			model->addItem("item1",ROOT_ID,"item1",QPointF(10,10));
			model->addItem("item2",ROOT_ID,"item2",QPointF(20,20));
			model->addItem("item3","item2","item3",QPointF(30,30));
			model->addItem("item4","item1","item4",QPointF(40,40));
			model->addItem("item5","item3","item5",QPointF(50,50));
			model->addItem("item6","item3","item6",QPointF(60,60));
		}


		void ModelFirstTest::data_data()
		{
			addColumn<QModelIndex>("index");
			addColumn<QString>("expectedName");
			newRow("root") << model->index(0,0) << "item1";
		}
		
		void ModelFirstTest::data()
		{	
			QFETCH(QModelIndex,index);
			QFETCH(QString,expectedName);			
			QCOMPARE(model->data(index).toString(),expectedName);
		}

		void ModelFirstTest::mimeData_data()
		{
			addColumn<QModelIndex>("index");
			addColumn<QString>("expectedId");
			newRow("root") << QModelIndex() << ROOT_ID;
		}

		void ModelFirstTest::mimeData()
		{
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
		}

		void ModelFirstTest::cleanupTestCase()
		{
			delete model;
		}
	}

}

using namespace qReal;

using namespace test;

QTEST_MAIN(ModelFirstTest)
#include "ModelFirstTest.moc"
