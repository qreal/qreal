#pragma once

#include <QString>
#include <QSet>

#include "model.h"

namespace qReal {
	namespace model {
		class Model;

		class Logger
		{
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
							QVariant const data, QString const additional);

			private:
				Model *mModel;

				bool enabled;
				bool flagsEnabled[4];
				QSet<Id> cleanDiagrams;


				bool pass(Id const scene);
				void remove(Id const scene);
				void write(QString const message, Id const scene);

				QString getDataString(QVariant const data) const;
		};
	}
}
