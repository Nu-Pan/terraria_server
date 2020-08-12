@setlocal
@echo off

cd %~d0
docker build -t nupan/terraria-server-base .
if errorlevel 1 (
    exit /b %errorlevel%
)

exit /b 0
