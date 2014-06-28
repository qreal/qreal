#include "settings.h"

#include <qrkernel/settingsManager.h>

using namespace twoDModel::model;

Settings::Settings()
{
	rereadNoiseSettings();
}

bool Settings::realisticPhysics() const
{
	return mRealisticPhysics;
}

bool Settings::realisticSensors() const
{
	return mRealisticSensors;
}

bool Settings::realisticMotors() const
{
	return mRealisticMotors;
}

void Settings::rereadNoiseSettings()
{
	bool const oldPhysics = mRealisticPhysics;
	mRealisticPhysics = qReal::SettingsManager::value("2DModelRealisticPhysics").toBool();
	if (oldPhysics != mRealisticPhysics) {// || !mPhysicsEngine) {
		emit physicsChanged(mRealisticPhysics);
		/// @todo:
//		physics::PhysicsEngineBase *oldEngine = mPhysicsEngine;
//		if (mRealisticPhysics) {
//			mPhysicsEngine = new physics::RealisticPhysicsEngine(mWorldModel);
//		} else {
//			mPhysicsEngine = new physics::SimplePhysicsEngine(mWorldModel);
//		}

//		delete oldEngine;
	}

	mRealisticSensors = qReal::SettingsManager::value("enableNoiseOfSensors").toBool();
	mRealisticMotors = qReal::SettingsManager::value("enableNoiseOfMotors").toBool();
}
