#include "newclient.h"
#include "Model/Model.h"
#include <QTreeView>

using namespace qReal;

using namespace model;

NewClient::NewClient(QWidget *parent)
	: QWidget(parent)
{
	Model *model =  new Model();
	QTreeView *view = new QTreeView(this);
	resize(500,400);
	view->setModel(model);
}

NewClient::~NewClient()
{

}
