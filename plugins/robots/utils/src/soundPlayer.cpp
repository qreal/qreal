/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "utils/soundPlayer.h"

#include <QtCore/QFile>
#include <QtMultimedia/QMediaPlayer>

#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>

using namespace utils;

SoundPlayer::SoundPlayer()
	: mPlayer(new QMediaPlayer)
{
	qReal::SettingsListener::listen("soundPlayerMuted", mPlayer.data(), &QMediaPlayer::setMuted);
	qReal::SettingsListener::listen("soundPlayerVolume", mPlayer.data(), &QMediaPlayer::setVolume);
	mPlayer->setMuted(qReal::SettingsManager::value("soundPlayerMuted").toBool());
	mPlayer->setVolume(qReal::SettingsManager::value("soundPlayerVolume").toInt());

	connect(mPlayer.data(), static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error error)>(&QMediaPlayer::error), [=](){
		emit error(mPlayer->errorString().isEmpty() ? tr("Can't play sound file") : mPlayer->errorString());
	});
	connect(mPlayer.data(), &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state) {
		if (QMediaPlayer::StoppedState == state) {
			emit finished();
		}
	});
}

SoundPlayer::~SoundPlayer()
{
}

SoundPlayer *SoundPlayer::instance()
{
	static SoundPlayer player;
	return &player;
}

void SoundPlayer::startPlaying(const QString &file)
{
	if (instance()->mPlayer->state() == QMediaPlayer::PlayingState
			&& !instance()->mPlayer->currentMedia().canonicalUrl().toString().contains(file))
	{
		stop();
	}

	const QUrl url = bestPath(file);
	if (!url.isValid()) {
		emit instance()->error(tr("File %1 not found").arg(file));
		return;
	}

	instance()->mPlayer->setMedia(url);
	instance()->mPlayer->play();
}

void SoundPlayer::stop()
{
	instance()->mPlayer->stop();
}

QUrl SoundPlayer::bestPath(const QString &path)
{
	if (QFile::exists(path)) {
		return QUrl::fromLocalFile(path);
	}

	if (QFile::exists(":/" + path)) {
		return QUrl("qrc:/" + path);
	}

	return QUrl();
}
