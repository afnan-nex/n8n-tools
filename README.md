## File writer (Run in CMD)
```cmd
curl -L -o "%TEMP%\file-writer.ps1" https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.ps1 && powershell -NoProfile -ExecutionPolicy Bypass -File "%TEMP%\file-writer.ps1"

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
