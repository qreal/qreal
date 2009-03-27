#include "Model.h"

using namespace qReal;

using namespace model;

Model::Model()
{
	mClient = new client::Client();
}

Model::~Model()
{
	delete mClient;
}

Qt::ItemFlags Model::flags( const QModelIndex &index ) const
{
	if (index.isValid()) {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled
			| Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	} else {
		return Qt::NoItemFlags;
	}
}

QVariant Model::data( const QModelIndex &index, int role) const
{

	switch (role) {
	//	case Qt::DisplayRole:
	//		return mClient->property()
	}
return QVariant();
}