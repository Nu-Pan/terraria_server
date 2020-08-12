@setlocal
@echo off

cd %~d0
docker push nupan/terraria-server-base:latest
if errorlevel 1 (
    exit /b %errorlevel%
)

exit /b 0
