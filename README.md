## File writer
```cmd
powershell -NoProfile -ExecutionPolicy Bypass -Command "$TD = [Environment]::GetFolderPath('UserProfile') + '\file-writer-tool'; Start-Process 'https://afnan-nex.github.io/n8n-tools/file-writer.html'; if (!(Test-Path $TD)) { New-Item -ItemType Directory -Path $TD | Out-Null }; Stop-Process -Name 'file-writer' -Force -ErrorAction SilentlyContinue; Start-Sleep -Seconds 1; $exe = Join-Path $TD 'file-writer.exe'; if (Test-Path $exe) { Remove-Item -Path $exe -Force -ErrorAction SilentlyContinue }; Invoke-WebRequest -Uri ('https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.exe?v=' + (Get-Random)) -OutFile $exe; $userPath = [Environment]::GetEnvironmentVariable('Path', 'User'); if ($userPath -notlike ('*' + $TD + '*')) { [Environment]::SetEnvironmentVariable('Path', ($userPath + ';' + $TD), 'User') }; if (Test-Path $exe) { Write-Host 'SUCCESS: file-writer.exe updated and added to User PATH!' -ForegroundColor Green } else { Write-Host 'FAILED: Download failed.' -ForegroundColor Red }; Read-Host 'Press Enter to continue...'"

```








## To Enable the Disabled Nodes in and after v2

```
set NODES_EXCLUDE=[]
```
```
setx NODES_EXCLUDE "[]"
```
```
setx NODES_EXCLUDE "[]" /M
```
```
setx NODES_EXCLUDE "[]"
setx NODES_EXCLUDE "[]" /M

```
