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
OutputBaseFilename=MyEncryptorInstaller
Compression=lzma
SolidCompression=yes

; IMPORTANT: allow user/system install choice
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

UninstallDisplayIcon={app}\{#MyAppExeName}

[Tasks]
Name: "contextmenu"; Description: "Add 'Encrypt' to Windows Explorer context menu"; Flags: checkedonce

[Files]
Source: "bin\fcrypt.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\*.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Registry]
; ===============================
; CONTEXT MENU (SYSTEM-WIDE)
; ===============================
Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: ""; ValueData: "Encrypt"; \
    Tasks: contextmenu; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Tasks: contextmenu; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" ""%1"""; \
    Tasks: contextmenu; Check: IsAdminInstallMode

; ===============================
; CONTEXT MENU (PER-USER)
; ===============================
Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: ""; ValueData: "Encrypt"; \
    Tasks: contextmenu; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Tasks: contextmenu; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" ""%1"""; \
    Tasks: contextmenu; Check: not IsAdminInstallMode

[UninstallDelete]
; Cleanup for system-wide
Type: regkey; Name: "HKLM\Software\Classes\*\shell\Encrypt"

; Cleanup for per-user
Type: regkey; Name: "HKCU\Software\Classes\*\shell\Encrypt"

[Code]
function IsAdminInstallMode: Boolean;
begin
  Result := IsAdmin;
end;
