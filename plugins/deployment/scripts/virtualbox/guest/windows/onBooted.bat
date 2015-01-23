echo off

:: Throwing event that the system was booted
VBoxControl guestproperty set OSBooted true

:: Then waiting a bit for all listeners to process it
timeout 5

:: And unsetting this parameter for future sessions
VBoxControl guestproperty unset OSBooted
