# install_dependencies.ps1
param(
    [string]$vcpkgPath = "C:\vcpkg"
)

# Verifica se vcpkg è già installato
if (-Not (Test-Path $vcpkgPath)) {
    Write-Output "Clonazione di vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git $vcpkgPath
    & $vcpkgPath\bootstrap-vcpkg.bat
}

# Integra vcpkg con Visual Studio
Write-Output "Integrazione di vcpkg con Visual Studio..."
& $vcpkgPath\vcpkg integrate install

# Installazione delle librerie richieste
Write-Output "Installazione delle librerie richieste (GLEW e GLM)..."
& $vcpkgPath\vcpkg install glew glm

# Ottieni il percorso di installazione di GLEW e GLM
$glewPath = Join-Path $vcpkgPath "installed\x64-windows-static"
$glmPath = Join-Path $vcpkgPath "installed\x64-windows"

# Imposta le variabili d'ambiente
[System.Environment]::SetEnvironmentVariable("LUME_GLEW", $glewPath, [System.EnvironmentVariableTarget]::User)
[System.Environment]::SetEnvironmentVariable("LUME_GLM", $glmPath, [System.EnvironmentVariableTarget]::User)

Write-Output "Installazione completata con successo!"
Write-Output "Le variabili d'ambiente sono state impostate: LUME_GLEW=$glewPath, LUME_GLM=$glmPath"