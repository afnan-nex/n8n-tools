## File writer
```cmd
cmd /v:on /c "set "TD=%USERPROFILE%\file-writer-tool" & start "" "https://afnan-nex.github.io/n8n-tools/file-writer.html" & if not exist "!TD!" mkdir "!TD!" & curl -sL "https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.exe" -o "!TD!\file-writer.exe" & for /f "tokens=2*" %A in ('reg query "HKCU\Environment" /v Path 2^>nul') do set "UP=%B" & (echo !UP! | find /I "!TD!" >nul || setx Path "!UP!;!TD!") & if exist "!TD!\file-writer.exe" (echo SUCCESS: file-writer.exe is set as an environmental variable!) else (echo FAILED: file-writer.exe could not be downloaded.) & pause"

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
