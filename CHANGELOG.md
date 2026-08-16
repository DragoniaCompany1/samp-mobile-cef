# Changelog - SA:MP Mobile CEF

All notable changes to the **SA:MP Mobile CEF** project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.2.0-Upgraded] - 2026-08-16

### 🚀 Added
- **`CefExecuteJavaScript(playerid, const code[])`**: Execute dynamic JavaScript strings in a player's WebView directly from Pawn.
- **`CefExecuteJavaScriptToAll(const code[])`**: Broadcast dynamic JavaScript execution to all connected players.
- **`CefGetInitializedPlayerCount()`**: Count total online players currently connected with CEF support.
- **`CefSetBrowserFocusAll(bool:is_focused)`**: Easily toggle browser touch/keyboard focus for all players.
- **`CefResetBrowser(playerid)`**: Complete reset helper for player WebView state (hides browser, removes focus, resets URL state).
- **Interactive Registration UI Example**: Added a complete, modern UCP Registration interface ([index.html](file:///home/drgxel/Documents/samp/samp-mobile-cef/example/register/index.html), [styles.css](file:///home/drgxel/Documents/samp/samp-mobile-cef/example/register/styles.css), [script.js](file:///home/drgxel/Documents/samp/samp-mobile-cef/example/register/script.js)) with glassmorphism UI and Discord integration support.

### ⚡ Improved
- Comprehensive bounds checking for `MAX_PLAYERS` across all library functions to prevent out-of-bound array memory access.
- Enhanced compatibility with both standard SA-MP servers (`a_samp`) and `open.mp` gamemodes.

---

## [1.1.0-Upgraded] - 2026-08-16

### 🚀 Added
- **Player State Tracking**: Real-time state tracking for WebView visibility (`cef_browser_shown[MAX_PLAYERS]`) and active URL (`cef_browser_url[MAX_PLAYERS][128]`).
- **`CefSendEventToAll(const event_name[], const event_data[])`**: Broadcast CEF events to all connected players.
- **`CefToggleBrowser(playerid, bool:show)`**: One-line helper function to show or hide player browser.
- **`CefIsBrowserShown(playerid)`**: Query if a player's WebView is currently visible.
- **`CefGetBrowserUrl(playerid, dest[], maxlen)`**: Retrieve active URL loaded in player's browser.
- **`CefReloadBrowser(playerid)`**: Reload current active URL in player browser.
- **`CefUnregisterEventCallback(const event_name[])`**: Remove registered event callback dynamically.
- **`CefHasEventCallback(const event_name[])`**: Check if an event callback is currently registered.

---

## [1.0.0] - 2024-05-15

### 🚀 Initial Release
- Initial release of SA:MP Mobile CEF library by Denis Akazuki.
- Basic RakNet packet integration (`ID_CUSTOM_CEF`).
- Core functions: `CefInitBrowser`, `CefDestroyBrowser`, `CefShowBrowser`, `CefHideBrowser`, `CefSetBrowserUrl`, `CefChangeBrowserFocus`, `CefSendEvent`, `CefRegisterEventCallback`.
- Example server script and basic alert notification Web UI demo.
