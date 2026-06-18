@echo off
title Pokemon Trainer Editor
echo Iniciando o Editor de Treinadores Pokemon...
echo.

:: Verifica se o Python está instalado
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python não encontrado. Por favor, instale Python primeiro.
    pause
    exit /b
)

:: Navega até a pasta onde o batch está sendo executado
cd /d "%~dp0"

:: Executa o script
python CFRU_Trainer_Editor.py

:: Mantém a janela aberta após a execução
pause