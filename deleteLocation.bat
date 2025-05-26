@echo off
REM Vai alla directory dei progetti
cd .metadata\.plugins\org.eclipse.core.resources\.projects

REM Scansiona tutte le sottodirectory per trovare i file .location
for /d %%D in (*) do (
    set "LOCATION_FILE=%%D\.location"
    if exist "%%D\.location" (
        echo Trovato: %%D\.location. Procedo con l'eliminazione.
        del /f /q "%%D\.location"
        if not exist "%%D\.location" (
            echo File %%D\.location eliminato con successo.
        ) else (
            echo Errore: impossibile eliminare il file %%D\.location.
        )
    ) else (
        echo Nessun file .location trovato in %%D.
    )
)

pause
