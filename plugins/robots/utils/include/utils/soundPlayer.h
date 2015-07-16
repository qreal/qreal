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

#pragma once

#include <QtCore/QObject>

#include "utilsDeclSpec.h"

class QMediaPlayer;

namespace utils {

/// Provides possibility to play sound files. One sound file can be played by one instance of SoundPlayer at a time.
/// Muting, unmuting and changing volume is performed through SettingsManager using bool "soundPlayerMuted"
/// and integer "soundPlayerVolume" keys.
class ROBOTS_UTILS_EXPORT SoundPlayer : public QObject
{
	Q_OBJECT

public:
	/// Returns single instance of the sound player. Useful for connecting signals-slots.
	/// @todo: "Singletonity" of sound player should be discussed.
	static SoundPlayer *instance();

	/// Starts playing the given sound file. If such file has relative path and does not exist in file system
	/// then the search will be performed in resources (by appending qrc:/, this is useful for hard-coded
	/// sound files like beep.wav). If some error occured during opening or playing sound file error() signal
	/// will be emitted. Playing is performed asynchroniousy (i.e. control flow will not be blocked by this method).
	/// Calling stop() or playing sound file to the end will emit finised() signal.
	static void startPlaying(const QString &file);

	/// Stops playing current audio file if such is playing right now. In that case finished() signal will be emitted,
	/// otherwise nothing will happen.
	static void stop();

signals:
	/// Emitted when playing finishes correctly: when audio file played to the end or stop() is called.
	void finished();

	/// Emitted when opening the file or playing process fired the error.
	/// @param: Error message that can be shown to user.
	void error(const QString &message);

private:
	SoundPlayer();
	~SoundPlayer();

	static QUrl bestPath(const QString &path);

	QScopedPointer<QMediaPlayer> mPlayer;
};

}
