@echo off
rem start script for MSVC compiled qreal:
start reposerver\release\server.exe
sleep 1
start release\qreal.exe
start RepoClientIce\server\release\reposerverice.exe
