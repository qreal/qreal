#pragma once

#include <QString>
#include <QList>
#include "ids.h"

namespace qReal {

	const Id ROOT_ID = Id("ROOT_ID","ROOT_ID","ROOT_ID","ROOT_ID");

	const char ID_PATH_DIVIDER = '#';

	const QString DEFAULT_MIME_TYPE = "application/x-real-uml-data";

	namespace roles {
		// Перечисление с ролями, которые активно используются фронтендом,
		// а посему должны присутствовать у всех сущностей.
		enum {
			idRole = Qt::UserRole + 1,  // Идентификатор элемента, типа qReal::Id
			positionRole,  // Позиция элемента в конкретном контексте (модель сама знает, в каком)
			configurationRole,  // Конфигурация (форма, размер) элемента в конкретном контексте
			fromRole,  // Для связей - Id объекта, из которого связь исходит, для объектов - игнорируется
			toRole,  // Для связей - Id объекта, в который связь входит, для объектов - игнорируется
			fromPortRole,  // Для связей - идентификатор порта (типа double), из которого связь исходит, для объектов - игнорируется
			toPortRole,  // Для связей - идентификатор порта, в который связь входит, для объектов - игнорируется
			customPropertiesBeginRole,  // Начало индексов сгенерированных ролей
			// Алсо, к обязательным атрибутам относится атрибут name, соответствующий ролям Qt::EditRole и Qt::DisplayRole.
		};
	}
}
