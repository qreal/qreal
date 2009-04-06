#include "newclient.h"
#include "Model/Model.h"
#include <QTableView>

using namespace qReal;

using namespace model;

NewClient::NewClient(QWidget *parent)
	: QWidget(parent)
{
	Model *model =  new Model();
	QTableView *view = new QTableView(this);
	resize(500,400);
	view->setModel(model);
	model->test();
}

NewClient::~NewClient()
{

}