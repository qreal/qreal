#pragma once

#include "model.h"

namespace qReal {
	namespace model {
		class Model;

		class Logger
		{
			public:
				Logger(Model *model);
				~Logger();

				enum action
				{
					actSetData,
					actAddElement,
					actRemoveElement,
					actCreateDiagram,
					actDestroyDiagram
				};

				enum flag
				{
					flgEditors,
					flgDiagrams,
					flgUselessMessages,
					flgInvalidMessages,
				};

				void enable();
				void disable();

				void setFlag(flag name, bool arg);
				void setWorkingDir(QString workingDir);

				void log(action performed,
						Id const scene);
				void log(action performed,
						Id const scene, Id const target);
				void log(action performed,
						Id const scene, Id const target,
							QVariant const prevData, QVariant const newData,
								QString const additional);
				void output();

				const QString msgInvalid;
				const QString msgSetData;
				const QString msgAddElement;
				const QString msgRemoveElement;
				const QString msgCreateDiagram;
				const QString msgDestroyDiagram;

				const QString msgOperation;
				const QString msgTarget;
				const QString msgDetails;
				const QString msgPrevValue;
				const QString msgNewValue;

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
				QMap<QString, QFile*> files;
				QMultiHash<Id, QString*> buffer;
		};
	}
}
