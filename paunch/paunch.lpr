program paunch;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms //, Unit1
  { you can add units after this },
  SysUtils, ShellApi;

var cmdFile : string;

{$R *.res}

begin
  //RequireDerivedFormResource:=True;
  Application.Initialize;
    cmdFile := StringReplace(Application.ExeName, '.exe', '.bat',
          [rfReplaceAll]);
    ShellExecute(0, nil, PChar('cmd'), PChar('/c ' + cmdFile), PChar(''), 1);
  //Application.ShowMainForm := False; // don't show form
  //Application.CreateForm(TForm1, Form1);
  //Application.Run;
end.

