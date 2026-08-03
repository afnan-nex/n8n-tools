# `file-writer.exe` — Command Line Tool & Automation Utility

## Overview
`file-writer.exe` is a lightweight, standalone Windows Command Line Interface (CLI) binary designed to take raw code strings and export them into designated file formats and system paths. Built specifically to integrate seamlessly with automated workflows (such as **n8n Execute Command nodes**), it eliminates manual file-creation overhead by processing input parameters directly from terminal executions.

---

## Purpose & Motivation
In workflow automation platforms like **n8n**, generating dynamic scripts or saving programmatically built code snippets to local disks can require complex file stream handling. Standard terminal commands often struggle with multi-line code strings, special symbol escaping, and path formatting.

`file-writer.exe` solves this by providing a unified binary interface that:
- Receives multi-line code payloads safely wrapped in string parameters.
- Accepts target file names (with custom extensions) and target output paths.
- Automatically handles directory creation if destination folders do not already exist.
- Executes natively without requiring runtime dependencies (e.g., Python interpreters or Node runtime packages).

---

## Historical Context & Previous State
### Initial Architecture
- **Compilation:** Early C++ CLI prototypes handled standard single-word argument parsing (`-n` filename, `-p` path, `-c` code payload).
- **Web Generator Interface:** The original web tool featured standard rounded UI containers with accent-driven dark themes.
- **Path & Name Handling:** Inputs expected unquoted or strictly formatted file names; paths containing spaces or forward slashes required manual correction prior to argument passing.

### Limitations Identified
1. **Unquoted File Names:** File names containing spaces (e.g., `this is my script.py`) broke argument indexes in Windows CMD when unquoted.
2. **UI Ergonomics:** The web interface allowed outer-page scrollbars on smaller screen viewports and featured unnecessary multi-color styling.
3. **Escaping Issues:** Multi-language code snippets containing internal double quotes (`"`) or backslashes (`\`) occasionally broke terminal string evaluation.
4. **Single Action Interface:** The web builder only allowed copying the creation command, leaving users to manually assemble the path and interpreter invocation for testing or execution.

---

## Updates & Improvements Included
The tool and its accompanying Web Command Generator underwent several key revisions:

### 1. Robust Argument Handling (`-n` Quote Wrapping)
- **C++ Parser Update:** File names passed with double quotes (`-n "my file.py"`) are now parsed cleanly using C++ standard filesystem paths.
- **HTML Sanitization:** The generator strips any pre-existing outer quotes entered by the user to avoid duplicate quotation marks (`""file.py""`), ensuring clean `-n "filename.ext"` formatting.

### 2. Path Sanitation & Auto-Directory Creation
- **Path Sanitization:** Automatically replaces all forward slashes (`/`) with standard Windows backslashes (`\`) and wraps paths safely in double quotes (`-p "C:\path"`).
- **Directory Guarantee:** `std::filesystem::create_directories` automatically builds missing nested directory hierarchies before attempting file creation.

### 3. Escaping Engine for Multi-Language Payloads
- **Character Escaping:** String inputs automatically escape internal backslashes (`\` $\rightarrow$ `\\`) and double quotes (`"` $\rightarrow$ `\"`).
- **Multi-Language Support:** Safely handles Python, C++, JavaScript, Shell, HTML/XML, and SQL payloads containing complex symbols and line breaks.

### 4. Minimalist High-Density Web Generator (`file-writer.html`)
- **Strict Viewport Lock (`100vh`):** Fully responsive, zero-scroll interface that fits any screen size without page scrollbars.
- **Minimalist Palette:** Monospace typography with pure black background (`#000`), subtle gray borders/labels (`gray`), crisp white text for user input, and bright green command previews (`#00ff00`).
- **Dual Copy Buttons:**
  - **Copy Command:** Copies the full `file-writer.exe` command string to generate the target file.
  - **Copy Execute Command:** Dynamically detects the file extension and copies the execution command using the appropriate runtime (`python "path\script.py"` for `.py`, `node "path\script.js"` for `.js`, `powershell -File` for `.ps1`, `bash` for `.sh`, or direct invocation for `.exe`/`.bat`/`.cmd`).
- **One-Click Deployment Automation:** Integrated single-line Windows CMD command using `cmd /v:on` that downloads the binary, opens the GitHub Pages UI, and updates the User `PATH` environment variable via Registry.

---

## Current Condition & Usage

### 1. CLI Execution Syntax
```cmd
file-writer.exe -n "this is my script.py" -p "C:\Users\Admin\Documents\Untitled" -c "# Code content here..."
```

#### Parameter Breakdown
| Parameter | Description |
| :--- | :--- |
| `-n` | **Filename & Extension:** The output file name enclosed in double quotes (supports spaces). |
| `-p` | **Destination Path:** The target folder path (automatically creates folders if missing). |
| `-c` | **Code String:** The escaped code snippet to be written into the file. |

---

### 2. Execution Command Auto-Detection
When using the web interface, clicking **Copy Execute Command** formats the target execution based on the file extension:

| File Extension | Target Execution Command |
| :--- | :--- |
| `.py` | `python "C:\path\file.py"` |
| `.js` | `node "C:\path\file.js"` |
| `.ps1` | `powershell -ExecutionPolicy Bypass -File "C:\path\file.ps1"` |
| `.sh` | `bash "C:\path\file.sh"` |
| `.exe` / `.bat` / `.cmd` / default | `"C:\path\file.exe"` |

---

### 3. Live Web Builder & Deployment
- **HTML Web Builder:** Hosted directly on GitHub Pages (`https://afnan-nex.github.io/n8n-tools/file-writer.html`).
- **Binary Repository:** Hosted directly on GitHub (`https://github.com/afnan-nex/n8n-tools/blob/main/file-writer.exe`).
- **Automated Installation Command:**
  ```cmd
  cmd /v:on /c "set "TD=%USERPROFILE%\file-writer-tool" & start "" "https://afnan-nex.github.io/n8n-tools/file-writer.html" & if not exist "!TD!" mkdir "!TD!" & curl -sL "https://raw.githubusercontent.com/afnan-nex/n8n-tools/main/file-writer.exe" -o "!TD!\file-writer.exe" & for /f "tokens=2*" %A in ('reg query "HKCU\Environment" /v Path 2^>nul') do set "UP=%B" & (echo !UP! | find /I "!TD!" >nul || setx Path "!UP!;!TD!") & if exist "!TD!\file-writer.exe" (echo SUCCESS: file-writer.exe is set as an environmental variable!) else (echo FAILED: file-writer.exe could not be downloaded.) & pause"
  ```