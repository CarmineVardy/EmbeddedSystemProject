@echo off
REM Percorso relativo al file da eliminare
set "RELATIVE_PATH=.metadata\.plugins\org.eclipse.core.resources\.projects\MidTermProject\.location"

REM Controlla se il file esiste
if exist "%RELATIVE_PATH%" (
    echo Il file %RELATIVE_PATH% esiste. Procedo con l'eliminazione.
    del /f /q "%RELATIVE_PATH%"
    if not exist "%RELATIVE_PATH%" (
        echo File eliminato con successo.
    ) else (
        echo Errore: impossibile eliminare il file.
    )
) else (
    echo Il file %RELATIVE_PATH% non esiste.
)

pause
