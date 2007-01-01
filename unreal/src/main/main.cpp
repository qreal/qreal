//=====================================================================
// Project:      altREAL 
// File Name:    main.cpp
// Description:  main() function file  
//
// Date:         21-November-06
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 


#include <QtGui>
#include <QString>
#include <QHBoxLayout>
#include <QTableView>
#include "treemodel.h"
#include "pieview.h"

//class 

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TreeModel *model = new TreeModel();
    
    QSplitter *splitter = new QSplitter;
    QTreeView *table = new QTreeView;
    PieView *pieChart = new PieView;
    splitter->addWidget(table);
    splitter->addWidget(pieChart);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    table->setModel(model);
    pieChart->setModel(model);
    
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    pieChart->setSelectionModel(selectionModel);
						    
    splitter->setWindowTitle(QObject::tr("0bj3ct xpl0r3r (tr33) && pr0p3rty 3d1t0r (tr33)"));

//  splitter->resize(400,200);
    splitter->show();

    return app.exec();
}
