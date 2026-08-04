# Self-elevation to admin
if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Start-Process powershell -Verb RunAs -ArgumentList "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`""
    exit
}

# Configuration
$ToolDir = [Environment]::GetFolderPath('CommonApplicationData') + '\file-writer-tool'
$ExePath = Join-Path $ToolDir 'file-writer.exe'
$DownloadUrl = 'https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.exe'

# Open browser
Start-Process 'https://afnan-nex.github.io/n8n-tools/file-writer.html'

# Create directory
[System.IO.Directory]::CreateDirectory($ToolDir) | Out-Null

# Kill existing process
Get-Process | Where-Object { $_.Name -like '*file-writer*' } | Stop-Process -Force -ErrorAction SilentlyContinue

# Wait
Start-Sleep -Seconds 2

# Remove old exe
if (Test-Path $ExePath) { Remove-Item $ExePath -Force -ErrorAction Stop }

# Download new exe
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
Invoke-WebRequest -Uri ($DownloadUrl + '?v=' + (Get-Random)) -OutFile $ExePath -Headers @{'Cache-Control'='no-cache'}

# Add to system PATH
$CurrentPath = [Environment]::GetEnvironmentVariable('Path', 'Machine')
if ($CurrentPath -notlike ('*' + $ToolDir + '*')) {
    [Environment]::SetEnvironmentVariable('Path', ($CurrentPath + ';' + $ToolDir), 'Machine')
}

# Done
Write-Host 'SUCCESS: file-writer.exe replaced and updated in SYSTEM PATH!' -ForegroundColor Green
Read-Host 'Press Enter to exit...'