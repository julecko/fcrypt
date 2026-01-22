#define MyAppName "Fcrypt"
#define MyAppExeName "fcrypt.exe"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Julecko"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
; AppPublisherURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputBaseFilename=FcryptInstaller
Compression=lzma
SolidCompression=yes

; Allow user/system install choice
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

UninstallDisplayIcon={app}\{#MyAppExeName}

[Tasks]
Name: "startmenu"; Description: "Add Fcrypt to Start Menu"; Flags: unchecked

[Files]
Source: "bin\fcrypt.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\*.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: startmenu

[Registry]
; ===============================
; CONTEXT MENU (SYSTEM-WIDE)
; ===============================
Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: ""; ValueData: "Encrypt"; \
    Flags: uninsdeletekey; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Flags: uninsdeletevalue; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" --encrypt ""%1"""; \
    Flags: uninsdeletekey; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Decrypt"; \
    ValueType: string; ValueName: ""; ValueData: "Decrypt"; \
    Flags: uninsdeletekey; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Decrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Flags: uninsdeletevalue; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Decrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" --decrypt ""%1"""; \
    Flags: uninsdeletekey; Check: IsAdminInstallMode

; Add to system PATH
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; \
    ValueType: string; ValueName: "Path"; \
    ValueData: "{olddata};{app}"; Flags: preservestringtype uninsdeletevalue; Check: IsAdminInstallMode

; ===============================
; CONTEXT MENU (PER-USER)
; ===============================
Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: ""; ValueData: "Encrypt"; \
    Flags: uninsdeletekey; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Flags: uninsdeletevalue; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" --encrypt ""%1"""; \
    Flags: uninsdeletekey; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Decrypt"; \
    ValueType: string; ValueName: ""; ValueData: "Decrypt"; \
    Flags: uninsdeletekey; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Decrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Flags: uninsdeletevalue; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Decrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" --decrypt ""%1"""; \
    Flags: uninsdeletekey; Check: not IsAdminInstallMode

; Add to user PATH
Root: HKCU; Subkey: "Environment"; \
    ValueType: string; ValueName: "Path"; \
    ValueData: "{olddata};{app}"; Flags: preservestringtype uninsdeletevalue; Check: not IsAdminInstallMode

[Code]
function IsAdminInstallMode: Boolean;
begin
  Result := IsAdmin;
end;
