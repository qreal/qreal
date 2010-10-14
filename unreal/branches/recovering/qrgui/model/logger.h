#pragma once

#include "model.h"

namespace qReal {
	namespace model {
		class Model;

		class Logger : QObject
		{
			Q_OBJECT

			public:
				Logger(Model *model);
				~Logger();
				void enable();
				void disable();

				enum action
				{
					setData,
					addElement,
					removeElement,
					createDiagram,
					destroyDiagram
				};

				enum flag
				{
					editors,
					diagrams,
					uselessMessages,
					invalidMessages,
				};

				void setFlag(flag name, bool arg);

				void log(action performed,
						Id const scene);
				void log(action performed,
						Id const scene, Id const target);
				void log(action performed,
						Id const scene, Id const target,
							QVariant const prevData, QVariant const newData,
								QString const additional);

				void output();

			private:
				bool pass(Id const scene);
				void remove(Id const scene);
				void write(QString const message, Id const scene);

				QString getDataString(QVariant const data) const;

				Model *mModel;
				QString mWorkingDir;

				bool enabled;
				bool flagsEnabled[4];
				QSet<Id> cleanDiagrams;
				QMultiHash<Id, QString*> buffer;

				const QString msgInvalid;
				const QString msgSetData;
				const QString msgAddElement;
				const QString msgRemoveElement;
				const QString msgCreateDiagram;
				const QString msgDestroyDiagram;

			private slots:
				void setWorkingDir(QString workingDir);
		};
	}
}
