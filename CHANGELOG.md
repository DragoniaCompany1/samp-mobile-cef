# Changelog - SA:MP Mobile CEF

All notable changes to the **SA:MP Mobile CEF** project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.5.0-Upgraded] - 2026-08-17

### 🚀 Added
- **Server-Side Core Upgrades (`SAMPMobileCef.inc`)**:
  - `CefEmit(playerid, event_name, format_str, ...)` & `CefEmitToAll`: Formatted JSON event transmission directly from Pawn.
  - `CefClearBrowserCache(playerid)`: Remote clear of WebView localStorage, sessionStorage, and force refresh.
  - `CefIsClientConnected(playerid)`: Combined check for player connection and active CEF client library initialization.
- **Server-Side UI Framework Upgrades (`SAMPMobileCefUI.inc`)**:
  - `CefShowConfirmDialog(playerid, dialogid, title, message, btn_yes, btn_no)` with `OnCefConfirmDialogResponse(playerid, dialogid, response)`.
  - `CefShowContextMenu(playerid, menu_id, title, options_json)` with `OnCefContextMenuResponse(playerid, menu_id, item_index, item_value[])`.
- **Open-Source Client-Side Implementation (`client/`)**:
  - Added [client/cpp/SAMPMobileCef.h](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/cpp/SAMPMobileCef.h) & [client/cpp/SAMPMobileCef.cpp](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/cpp/SAMPMobileCef.cpp) (C++ NDK).
  - Added [client/java/CefJavaManager.java](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/java/CefJavaManager.java) & [client/java/CefClientManager.java](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/java/CefClientManager.java) (Java Android Studio).

---

## [1.4.0-Upgraded] - 2026-08-16

### 🚀 Added
- **Web List & Table Dialog System**: Added `CefShowListDialog` for interactive lists/tables with search bar.
- **HTML5 Audio / SFX System**: Added dynamic audio playback functions `CefPlayAudio`, `CefStopAudio`, and `CefSetAudioVolume`.
- **Interactive Inventory Grid Framework**: Added `CefOpenInventory` and `CefCloseInventory`.
- **Web UI Zoom & Screen Scaler**: Added `CefSetBrowserZoom`.
- **Anti-Spam Event Rate Limiter**: Added `CefSetEventRateLimit`.

---

## [1.3.0-Upgraded] - 2026-08-16

### 🚀 Added
- **New High-Level UI Include (`SAMPMobileCefUI.inc`)**: Dedicated high-level UI component framework for SA:MP server developers.

---

## [1.0.0] - 2024-05-15

### 🚀 Initial Release
- Initial release of SA:MP Mobile CEF library by Denis Akazuki.
