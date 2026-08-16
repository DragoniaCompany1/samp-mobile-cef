# Changelog - SA:MP Mobile CEF

All notable changes to the **SA:MP Mobile CEF** project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.4.0-Upgraded] - 2026-08-16

### 🚀 Added
- **Web List & Table Dialog System**: Added `CefShowListDialog(playerid, dialogid, title, items_json[], button1, button2)` for interative lists and tables with search bar, routing to `OnCefListDialogResponse(playerid, dialogid, response, item_index, item_value[])`.
- **HTML5 Audio / SFX System**: Added dynamic audio playback functions `CefPlayAudio`, `CefStopAudio`, and `CefSetAudioVolume`.
- **Interactive Inventory Grid Framework**: Added high-level functions `CefOpenInventory` and `CefCloseInventory`, routing item use actions to `OnCefInventoryUseItem(playerid, slot_id, item_name[])`.
- **Web UI Zoom & Screen Scaler**: Added `CefSetBrowserZoom(playerid, scale)` and `CefGetBrowserZoom(playerid)` to automatically scale UI for high-res mobile displays (720p/1080p/2K).
- **Anti-Spam Event Rate Limiter**: Built-in server protection against client JavaScript event flooding using `CefSetEventRateLimit(max_events_per_sec)`.

---

## [1.3.0-Upgraded] - 2026-08-16

### 🚀 Added
- **New High-Level UI Include (`SAMPMobileCefUI.inc`)**: Dedicated high-level UI component framework for SA:MP server developers.
  - **Notification & Toast System**: `CefShowNotification`, `CefShowNotificationToAll`
  - **Web Dialog & Modal System**: `CefShowDialog`, `CefShowInputDialog` with automatic routing to `OnCefDialogResponse(playerid, dialogid, response, inputtext[])`.
  - **Vehicle Speedometer Helper**: `CefUpdateSpeedometer(playerid, speed, fuel, engine_health, seatbelt, lights)`
  - **Player HUD Helper**: `CefUpdatePlayerHUD(playerid, money, bank, level, exp, location)`
  - **Action Progress Bar Helper**: `CefShowProgressBar(playerid, title, duration_ms)`, `CefHideProgressBar(playerid)`

---

## [1.2.0-Upgraded] - 2026-08-16

### 🚀 Added
- **`CefExecuteJavaScript(playerid, const code[])`**: Execute dynamic JavaScript strings in a player's WebView directly from Pawn.
- **`CefExecuteJavaScriptToAll(const code[])`**: Broadcast dynamic JavaScript execution to all connected players.
- **`CefGetInitializedPlayerCount()`**: Count total online players currently connected with CEF support.
- **`CefSetBrowserFocusAll(bool:is_focused)`**: Easily toggle browser touch/keyboard focus for all players.
- **`CefResetBrowser(playerid)`**: Complete reset helper for player WebView state.
- **Interactive Registration UI Example**: Added a complete, modern UCP Registration interface ([index.html](file:///home/drgxel/Documents/samp/samp-mobile-cef/example/register/index.html), [styles.css](file:///home/drgxel/Documents/samp/samp-mobile-cef/example/register/styles.css), [script.js](file:///home/drgxel/Documents/samp/samp-mobile-cef/example/register/script.js)).

---

## [1.1.0-Upgraded] - 2026-08-16

### 🚀 Added
- **Player State Tracking**: Real-time state tracking for WebView visibility (`cef_browser_shown[MAX_PLAYERS]`) and active URL (`cef_browser_url[MAX_PLAYERS][128]`).
- **`CefSendEventToAll(const event_name[], const event_data[])`**: Broadcast CEF events to all connected players.
- **`CefToggleBrowser(playerid, bool:show)`**: One-line helper function to show or hide player browser.
- **`CefIsBrowserShown(playerid)`**: Query if a player's WebView is currently visible.
- **`CefGetBrowserUrl(playerid, dest[], maxlen)`**: Retrieve active URL loaded in player's browser.
- **`CefReloadBrowser(playerid)`**: Reload current active URL in player browser.

---

## [1.0.0] - 2024-05-15

### 🚀 Initial Release
- Initial release of SA:MP Mobile CEF library by Denis Akazuki.
