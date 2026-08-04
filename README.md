## File writer (Run in Powershell)
```cmd
Start-Process powershell -Verb RunAs -ArgumentList "-NoProfile -ExecutionPolicy Bypass -Command `$TD=[Environment]::GetFolderPath('CommonApplicationData')+'\file-writer-tool'; Start-Process 'https://afnan-nex.github.io/n8n-tools/file-writer.html'; [System.IO.Directory]::CreateDirectory(`$TD); Get-Process | Where-Object { `$_.Name -like '*file-writer*' } | Stop-Process -Force -ErrorAction SilentlyContinue; Start-Sleep -Seconds 2; `$exe=Join-Path `$TD 'file-writer.exe'; if(Test-Path `$exe){ Remove-Item `$exe -Force -ErrorAction Stop }; [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri ('https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.exe?v='+(Get-Random)) -OutFile `$exe -Headers @{'Cache-Control'='no-cache'}; `$p=[Environment]::GetEnvironmentVariable('Path','Machine'); if(`$p -notlike ('*'+`$TD+'*')){ [Environment]::SetEnvironmentVariable('Path',(`$p+';'+`$TD),'Machine') }; Write-Host 'SUCCESS: file-writer.exe replaced and updated in SYSTEM PATH!' -ForegroundColor Green; Read-Host 'Press Enter to exit...'"

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
