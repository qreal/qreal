#pragma once

#include <functional>
#include <QtCore/QMap>
#include <QtCore/QVariant>

namespace twoDModel {
namespace constraints {
namespace details {

class Event;

enum class Glue
{
	And = 0
	, Or
};

typedef function<bool()> Condition;
typedef function<void()> Trigger;
typedef function<QVariant()> Value;

typedef QMap<QString, QVariant> Variables;
typedef QMap<QString, Event *> Events;
typedef QMap<QString, QObject *> Objects;

}
}
}
