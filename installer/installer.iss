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

; Allow user/system install choice
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
    Flags: uninsdeletekey; Tasks: contextmenu; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Flags: uninsdeletevalue; Tasks: contextmenu; Check: IsAdminInstallMode

Root: HKLM; Subkey: "Software\Classes\*\shell\Encrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" ""%1"""; \
    Flags: uninsdeletekey; Tasks: contextmenu; Check: IsAdminInstallMode

; ===============================
; CONTEXT MENU (PER-USER)
; ===============================
Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: ""; ValueData: "Encrypt"; \
    Flags: uninsdeletekey; Tasks: contextmenu; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt"; \
    ValueType: string; ValueName: "Icon"; ValueData: "{app}\{#MyAppExeName}"; \
    Flags: uninsdeletevalue; Tasks: contextmenu; Check: not IsAdminInstallMode

Root: HKCU; Subkey: "Software\Classes\*\shell\Encrypt\command"; \
    ValueType: string; ValueName: ""; \
    ValueData: """{app}\{#MyAppExeName}"" ""%1"""; \
    Flags: uninsdeletekey; Tasks: contextmenu; Check: not IsAdminInstallMode

[Code]
function IsAdminInstallMode: Boolean;
begin
  Result := IsAdmin;
end;
