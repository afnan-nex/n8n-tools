# `file-writer.exe` — Command Line Tool & Automation Utility

## Overview
`file-writer.exe` is a lightweight, standalone Windows Command Line Interface (CLI) binary designed to take raw code strings and export them into designated file formats and system paths. Built specifically to integrate seamlessly with automated workflows (such as **n8n Execute Command nodes**), it eliminates manual file-creation overhead by processing input parameters directly from terminal executions.

---

## Purpose & Motivation
In workflow automation platforms like **n8n**, generating dynamic scripts or saving programmatically built code snippets to local disks can require complex file stream handling. Standard terminal commands often struggle with multi-line code strings, special symbol escaping, and path formatting.

`file-writer.exe` solves this by providing a unified binary interface that:
- Receives multi-line code payloads safely wrapped in string parameters or fed directly via standard input (`stdin`).
- Accepts target file names (with custom extensions) and target output paths.
- Automatically handles directory creation if destination folders do not already exist.
- Executes natively without requiring runtime dependencies (e.g., Python interpreters or Node runtime packages).

---

## Historical Context & Previous State
### Initial Architecture
- **Compilation:** Early C++ CLI prototypes handled standard single-word argument parsing (`-n` filename, `-p` path, `-c` code payload).
- **Web Generator Interface:** The original web tool featured standard rounded UI containers with accent-driven dark themes.
- **Path & Name Handling:** Inputs expected unquoted or strictly formatted file names; paths containing spaces or forward slashes required manual correction prior to argument passing.

### Limitations Identified & Windows CLI Quirks

1. **The Trailing Backslash Escape Bug (`-p "C:\Path\"`)**
   - *Quirk:* In Windows (MSVC command-line parsing rules), a backslash immediately preceding a double quote (`\"`) acts as an escape sequence for that quote.
   - *Failure:* If a path ends with a backslash and is wrapped in quotes (e.g., `-p "C:\Users\Admin\"`), the parser treats the closing quote as a literal quote character. It fails to close the argument and consumes all subsequent parameters (like `-c`) as part of the directory path, resulting in mismatched arguments.

2. **CMD Double-Quote Escaping & Redirection Bug**
   - *Quirk:* Windows Command Prompt (`cmd.exe`) does not recognize backslash-escaped double-quotes (`\"`) inside a double-quoted string. It treats the backslash as a literal backslash and the quote as the closing boundary.
   - *Failure:* Nested double quotes in a script payload (e.g., `print("Launching browser...")`) break out of the `-c` argument context. Subsequent symbols are evaluated as raw shell operators. For example, a `>` character (e.g., `if count() > 0:`) is treated as a stdout redirection operator, causing CMD to try to redirect output to a file named `0:`. Because colon `:` is an illegal character in relative filenames, the command crashes with:
     `The filename, directory name, or volume label syntax is incorrect.`

3. **Command-Line Length Limits**
   - *Quirk:* Windows CMD has a strict command-line buffer limit of **8191 characters**.
   - *Failure:* Writing large Python or JS scripts directly via the `-c` CLI parameter exceeds the buffer limit, causing the shell to truncate the command and fail execution.

4. **Directory Creation Failures on Drive Roots (`C:`)**
   - *Quirk:* C++ `std::filesystem::create_directories` fails or throws exceptions when executed on root-only paths or drive specifiers (like `C:` or `C:\`), since drive roots are protected and cannot be created as directories.
   - *Failure:* Attempting to write files directly to drive root contexts crashed the binary with `Filename too long` or directory creation failures.

---

## Updates & Improvements Included
The tool and its accompanying Web Command Generator underwent several key revisions:

### 1. Robust Argument Handling & Quoting Bypass (`\q`)
- **C++ Parser Update:** File names passed with double quotes (`-n "my file.py"`) are now parsed cleanly using C++ standard filesystem paths.
- **Bypassing CMD Escaping via `\q`:** Since standard CMD does not support double-quote escaping (`\"`), the HTML generator now escapes double quotes as `\q`. The CLI parser converts `\q` back into normal double quotes (`"`) when writing the file. This completely eliminates argument truncation and syntax/redirection errors in CMD and n8n.
- **HTML Sanitization:** The generator strips any pre-existing outer quotes entered by the user to avoid duplicate quotation marks (`""file.py""`), ensuring clean `-n "filename.ext"` formatting.

### 2. Path Sanitation & Auto-Directory Creation
- **Path Sanitization:** Automatically replaces all forward slashes (`/`) with standard Windows backslashes (`\`) and wraps paths safely in double quotes (`-p "C:\path"`). It also automatically strips trailing backslashes to prevent the path quote from escaping next parameters.
- **Directory Guarantee:** `std::filesystem::create_directories` automatically builds missing nested directory hierarchies before attempting file creation, skipping root-only paths like `C:` or `C:\` which would otherwise throw OS path-creation errors.

### 3. Pipeline & Stdin Support
- **Standard Input (`stdin`) Integration:** If `-c` is omitted (or if `-c -` is passed), `file-writer.exe` automatically reads the raw payload from standard input. This allows users to pipe scripts directly (e.g., `type code.txt | file-writer.exe -n script.py -p C:\Target`) bypassing all CLI escaping and length limits.

### 4. CLI Flags for Help & Version checking
- Added standard CLI argument checking:
  - `-h` or `--help` prints the help instructions and syntax usage.
  - `-v` or `--version` outputs the version numbers (`1.1.0`).

### 5. Multi-Scope PATH Installation
- The PowerShell installer script [`file-writer.ps1`](file:///C:/Users/Admin/Desktop/FIle-writer/file-writer.ps1) updates both System (Machine) and User `PATH` environments, and applies the path update immediately to the current shell session.

### 6. Minimalist High-Density Web Generator (`file-writer.html`)
- **Strict Viewport Lock (`100vh`):** Fully responsive, zero-scroll interface that fits any screen size without page scrollbars.
- **Minimalist Palette:** Monospace typography with pure black background (`#000`), subtle gray borders/labels (`gray`), crisp white text for user input, and bright green command previews (`#00ff00`).
- **Triple Action Buttons:**
  - **Copy CMD Command:** Copies the safe `file-writer.exe` command using `\q` escaping.
  - **Copy CMD Pipe Command:** Copies a base64 pipelined PowerShell decoding command that runs natively in CMD/n8n.
  - **Copy Execute Command:** Dynamically detects the file extension and copies the execution command using the appropriate runtime (`python "path\script.py"` for `.py`, `node "path\script.js"` for `.js`, `powershell -File` for `.ps1`, `bash` for `.sh`, or direct invocation for `.exe`/`.bat`/`.cmd`).

---

## Current Condition & Usage

### 1. CLI Execution Syntax
```cmd
file-writer.exe -n "this is my script.py" -p "C:\Users\Admin\Documents\Untitled" -c "print(\qHello World\q)"
```

#### Parameter Breakdown
| Parameter | Description |
| :--- | :--- |
| `-n` | **Filename & Extension:** The output file name enclosed in double quotes (supports spaces). |
| `-d` | **Force-Delete Filename:** Same as `-n` but deletes the existing file first before writing. |
| `-p` | **Destination Path:** The target folder path (automatically creates folders if missing). |
| `-c` | **Code String:** The escaped code snippet to be written into the file. Converts `\q` to `"` and `\n` to newline. Set as `-` or omit to read from stdin. |
| `-h` / `--help` | Prints usage and command instructions. |
| `-v` / `--version` | Prints tool version. |

---

### 2. Direct Execution Command
When using the web interface, clicking **Copy Execute Command** copies the direct absolute path of the generated script:

```cmd
"C:\path\file.ext"
```

This allows Windows/n8n to run the file directly using the operating system's default file type associations (e.g. running `.py` files with the default Python interpreter).

---

### 3. Live Web Builder & Deployment
- **HTML Web Builder:** Hosted directly on GitHub Pages (`https://afnan-nex.github.io/n8n-tools/file-writer.html`).
- **Binary Repository:** Hosted directly on GitHub (`https://github.com/afnan-nex/n8n-tools/blob/main/file-writer.exe`).
- **Automated Installation Command:**
  ```cmd
  cmd /v:on /c "set "TD=%USERPROFILE%\file-writer-tool" & start "" "https://afnan-nex.github.io/n8n-tools/file-writer.html" & if not exist "!TD!" mkdir "!TD!" & curl -sL "https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.exe" -o "!TD!\file-writer.exe" & for /f "tokens=2*" %A in ('reg query "HKCU\Environment" /v Path 2^>nul') do set "UP=%B" & (echo !UP! | find /I "!TD!" >nul || setx Path "!UP!;!TD!") & if exist "!TD!\file-writer.exe" (echo SUCCESS: file-writer.exe is set as an environmental variable!) else (echo FAILED: file-writer.exe could not be downloaded.) & pause"
  ```