# Student Management System (C++)

Console-based student record management system with file persistence and statistics.

## Features

- Add student records: ID, Name, Age, Course, Marks
- Display all records in tabular format
- Search by Student ID or Name
- Persistent storage in `data/students.txt`
- Statistics:
  - Average marks
  - Pass/fail counts and percentages (custom pass threshold)
  - Highest and lowest scorer
- Input validation:
  - Unique student ID (case-insensitive)
  - Name must contain at least one letter
  - Course must contain at least one letter
  - Age must be a positive integer
  - Marks must be between `0` and `100`

## Project Structure

```text
student_management_system/
├── include/
│   ├── Student.h
│   ├── Database.h
│   └── Utils.h
├── src/
│   ├── main.cpp
│   ├── Student.cpp
│   ├── Database.cpp
│   └── Utils.cpp
├── data/
│   └── students.txt
├── tests/
│   ├── phase4_core_test.cpp
│   ├── phase6_statistics_test.cpp
│   └── phase7_validation_test.cpp
├── build/
└── Makefile
```

## Requirements

- Windows with `g++` available (MSYS2 MinGW-w64 recommended)
- Project root:
  - `C:\Users\TAO-TYRO\CLND PROJECTS\student_management_system`

## Quick Start (PowerShell)

1. Open PowerShell and go to the project:

```powershell
cd "C:\Users\TAO-TYRO\CLND PROJECTS\student_management_system"
```

2. Add MinGW compiler to current terminal PATH:

```powershell
$env:PATH='C:\msys64\mingw64\bin;' + $env:PATH
```

3. Build the app:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/Student.cpp src/Database.cpp src/Utils.cpp -o build/student_management_system.exe
```

4. Run the app:

```powershell
.\build\student_management_system.exe
```

## Quick Start (CMD)

1. Open Command Prompt and go to the project:

```cmd
cd /d "C:\Users\TAO-TYRO\CLND PROJECTS\student_management_system"
```

2. Add MinGW compiler to current terminal PATH:

```cmd
set "PATH=C:\msys64\mingw64\bin;%PATH%"
```

3. Build the app:

```cmd
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src\main.cpp src\Student.cpp src\Database.cpp src\Utils.cpp -o build\student_management_system.exe
```

4. Run the app:

```cmd
build\student_management_system.exe
```

## Step-by-Step Usage

1. Start the program.
2. Choose `1` to add a student.
3. Enter:
   - Student ID (must be unique)
   - Name (must include letters)
   - Age (positive integer)
   - Course (must include letters)
   - Marks (`0-100`)
4. Choose `2` to display all students.
5. Choose `3` to search by ID or part of name.
6. Choose `4` to view statistics:
   - Enter pass threshold (`0-100`)
   - View average, pass/fail, highest, lowest
7. Choose `5` to save and exit.

## Data File

- File path: `data/students.txt`
- Format per line:

```text
ID|Name|Age|Course|Marks
```

Example:

```text
BICT0823|Tao Tyro|23|Programming in C++|78.00
```

## Build and Run Tests

### PowerShell

```powershell
$env:PATH='C:\msys64\mingw64\bin;' + $env:PATH

g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/phase4_core_test.cpp src/Student.cpp src/Database.cpp src/Utils.cpp -o build/phase4_core_test.exe
.\build\phase4_core_test.exe

g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/phase6_statistics_test.cpp src/Student.cpp src/Database.cpp src/Utils.cpp -o build/phase6_statistics_test.exe
.\build\phase6_statistics_test.exe

g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/phase7_validation_test.cpp src/Student.cpp src/Database.cpp src/Utils.cpp -o build/phase7_validation_test.exe
.\build\phase7_validation_test.exe
```

### CMD

```cmd
set "PATH=C:\msys64\mingw64\bin;%PATH%"

g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests\phase4_core_test.cpp src\Student.cpp src\Database.cpp src\Utils.cpp -o build\phase4_core_test.exe
build\phase4_core_test.exe

g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests\phase6_statistics_test.cpp src\Student.cpp src\Database.cpp src\Utils.cpp -o build\phase6_statistics_test.exe
build\phase6_statistics_test.exe

g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests\phase7_validation_test.cpp src\Student.cpp src\Database.cpp src\Utils.cpp -o build\phase7_validation_test.exe
build\phase7_validation_test.exe
```

## Maintenance Commands

### Check compiler

PowerShell:

```powershell
g++ --version
```

CMD:

```cmd
g++ --version
```

### View saved data

PowerShell:

```powershell
Get-Content data\students.txt
```

CMD:

```cmd
type data\students.txt
```

### Backup data

PowerShell:

```powershell
Copy-Item data\students.txt data\students_backup.txt -Force
```

CMD:

```cmd
copy /Y data\students.txt data\students_backup.txt
```

### Restore data

PowerShell:

```powershell
Copy-Item data\students_backup.txt data\students.txt -Force
```

CMD:

```cmd
copy /Y data\students_backup.txt data\students.txt
```

### Reset student data (clear file)

PowerShell:

```powershell
Set-Content -NoNewline -Path data\students.txt -Value ''
```

CMD:

```cmd
type nul > data\students.txt
```

### Clean built executables

PowerShell:

```powershell
Remove-Item build\*.exe -Force -ErrorAction SilentlyContinue
```

CMD:

```cmd
del /Q build\*.exe
```

## Troubleshooting

- `Permission denied` when building `student_management_system.exe`:
  - App is likely still running. Close it, then rebuild.
  - PowerShell:

```powershell
Get-Process student_management_system -ErrorAction SilentlyContinue | Stop-Process -Force
```

  - CMD:

```cmd
taskkill /IM student_management_system.exe /F
```

- App runs old behavior after code changes:
  - Rebuild executable using the compile command, then run again.

- `g++ is not recognized`:
  - Ensure PATH is set for current terminal session as shown in Quick Start.
