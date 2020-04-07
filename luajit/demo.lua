local ffi = require('ffi');

ffi.cdef[[
  typedef unsigned int UINT;
  typedef void* HLTK;
  typedef const char* LPCSTR;
  typedef void* HWND;
  typedef void (__stdcall *LtkCallback)();

  UINT __stdcall LtkInitialize();
  void __stdcall LtkRunMessageLoop();
  
  void __stdcall LtkRegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata);

  HLTK __stdcall LtkWindow_New_(LPCSTR source, int line);
  void __stdcall LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height);
  void __stdcall LtkWindow_UpdateTheme(HLTK self);
  
  void __stdcall PostQuitMessage(int nExitCode);
]]
local ltk = ffi.load([[C:\code\ltkcpp_open\luajit\ltkcpp.dll]])
local user32 = ffi.load("User32.dll");
local LTK_OBJECT_DELETE = 99;

local function OnDelete(userdata)
  print("OnDelete");
  user32.PostQuitMessage(0);
end

ltk.LtkInitialize();

local wnd = ltk.LtkWindow_New_(nil, 0);
ltk.LtkRegisterCallback(wnd, LTK_OBJECT_DELETE, OnDelete, nil);
ltk.LtkWindow_CreateCenter(wnd, nil, 400, 300);
ltk.LtkWindow_UpdateTheme(wnd);
ltk.LtkRunMessageLoop()
