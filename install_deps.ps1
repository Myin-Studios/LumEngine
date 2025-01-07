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
Write-Output "Installazione delle librerie richieste (GLEW, GLM e ASIO)..."
& $vcpkgPath\vcpkg install glew glm asio

# Ottieni il percorso di installazione
$lumeDepsPath = Join-Path $vcpkgPath "installed\x64-windows"

# Imposta le variabili d'ambiente
[System.Environment]::SetEnvironmentVariable("LUME_DEPS", $lumeDepsPath, [System.EnvironmentVariableTarget]::User)

Write-Output "Installazione completata con successo!"
Write-Output "La variabile d'ambiente è stata impostata: LUME_DEPS=$lumeDepsPath"