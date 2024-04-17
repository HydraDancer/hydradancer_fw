# C programming and formatting rules

## Coding rules

* Use standard types from stdbool.h/stdint.h => uint8_t/uint16_t/uint32_t ...
* Avoid any memory allocation like `malloc()/free()` (if required a dedicated memory allocator shall be used)
* All string functions that do not take an explicit buffer size argument are forbidden.
  * This includes but is not limited to `strcpy()`, `sprintf()`, etc.
* Use of C string manipulation functions with an explicit size, such as `snprintf()`, is acceptable.
* Field width specifiers are required on all `sscanf()` calls that write to strings. For example, %30s rather than %s.
* Avoid big stack usage (as the stack is very limited)
* Do not use reserved keywords for files (like CON, PRN, AUX, NUL, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, and LPT9) This is documented on: https://docs.microsoft.com/en-us/windows/desktop/FileIO/naming-a-file

## C code conventions

The reference for how the code should be formatted is the `.clang_format` file in the top directory. When writing new C code, please adhere to these conventions.

The coding style is based on the Linux Kernel coding style (https://www.kernel.org/doc/html/latest/process/coding-style.html) with modifications.

## (Favored) Use pre-commit hooks

Pre-commit is a Python package that simplifies the use and creation of custom Git hooks.

The pre-commit Git hook is triggered before actually commiting to the repository. By adding a hook, you can make the commit fail if the files added do not meet a certain standard.

First, you need to install pre-commit : https://pre-commit.com (mainly
```shell
pip install pre-commit
```
in your Python virtual environment).

Then, run `pre-commit install` from the top directory, so that `pre-commit` sets up the Git hooks. It will install the required submodules (Python packages for instance) without polluting your global installation.

After that, you can test pre-commit using `pre-commit run -a` to run pre-commit on all files.

When you try committing, pre-commit will run and display errors if there are some. It will also apply fixes in some cases, try commiting again in that case.

## Formatting using Clang-format and autopep8

Formatting can be automatically applied to all subprojects using `sh format.sh all` from the top folder, or by using `sh format.sh` in a specific subproject.

You'll need `clang-format` and `autopep8` at the versions fixed in `requirements.txt`.

First, you'll need `python3` and `venv` installed, which can be done like this if you're using a Debian-based distribution like Ubuntu:

```shell
sudo apt update && sudo apt install python3 python3-venv
python3 -m venv venv
source venv/bin/activate
```

Then, install `clang-format` and `autopep8` using `requirements.txt` to get the right versions :

```shell
pip install -r requirements.txt
```
