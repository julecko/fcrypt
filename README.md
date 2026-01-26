# Fcrypt

**fcrypt** — lightweight file encryption and decryption tool for Windows.

fcrypt lets you encrypt and decrypt files locally using a symmetric key (password). It’s designed for simplicity and ease of use, not as a replacement for production‑grade security libraries. Use at your own risk.

## Features

- Encrypt single files with a password.
- Decrypt files previously encrypted by fcrypt.
- Built with C/CMake for Windows.
- Libsodium for safe crypto.

## Contents

```
.
├── src/                # Source code
├── include/            # Headers
├── CMakeLists.txt      # Build script
├── build.bat           # Windows build helper
├── README.md           # This document
├── LICENSE             # MIT License
└── vcpkg.json          # Vcpkg manifest for libsodium
```

## Building

### Requirements

- Windows (tested with Windows 10/11)
- Visual Studio (MSVC toolchain)
- CMake 3.10+

### Build

```sh
.\build.bat
```

After building you should have a `fcrypt.exe` in the build output.


## Usage

```
fcrypt <command> <input-file>
fcrypt --version
fcrypt --help
```

### Commands

|  Command  |    Description     |
|-----------|--------------------|
| `encrypt` | Encrypt input file |
| `decrypt` | Decrypt input file |
