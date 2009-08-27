#pragma once 

#include <QtGui/QDialog>
#include <QDomDocument>
#include "realrepoclient.h"

namespace Ui
{
    class meta_generatorClass;
}

namespace qReal {

	class meta_generator : public QDialog
	{
		Q_OBJECT
	
	public:
		void createMetaEditor(QString EditorIdType);
		meta_generator();
		~meta_generator();
	
	private:
		QDomElement createNode(QDomDocument doc, QString nodeId, QString prefix);
		QString getName(QString IdType);
		QString getType(QString IdType);
		QString getOtherEntityFromLink(QString LinkId, QString firstNode);
		RealRepoClient *client;
		Ui::meta_generatorClass *ui;
	private slots:
		void createEditor();
	};
}

