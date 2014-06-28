#pragma once

#include <QtCore/QObject>

namespace twoDModel {
namespace model {

/// Incapsulates settings used by 2D model.
class Settings : public QObject
{
	Q_OBJECT

public:
	Settings();

	/// Returns true is user selected realistic physical engine.
	bool realisticPhysics() const;

	/// Returns true is user wants to add some noise to sensors values.
	bool realisticSensors() const;

	/// Returns true is user wants to add some noise to motors work.
	bool realisticMotors() const;

	/// Rereads all settings related to realistic emulation.
	void rereadNoiseSettings();

signals:
	/// Emitted each time when user modifies physical preferences.
	void physicsChanged(bool isRealistic);

private:
	bool mRealisticPhysics;
	bool mRealisticSensors;
	bool mRealisticMotors;
};

}
}
