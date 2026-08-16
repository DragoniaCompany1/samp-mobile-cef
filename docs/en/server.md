Language: **English** | [Українська](../uk/server.md)

# SA:MP Mobile CEF (Server API Documentation)

## Important Information
- If the length of the information you send to/from the callback exceeds the limit specified in `SAMPMobileCef.inc` (default is 2048), it is possible to extend it manually:
> [!NOTE]
> The client-side uses dynamic memory allocation, so **length restrictions only apply to the server-side**. For the convenience of developers, the ability to manually extend the length of data on the server-side has been added.
```pawn
/* Use this method before including the SAMPMobileCef.inc header file */
#define CEF_MAX_EVENT_NAME_LENGTH 64 // extend the maximum length of the callback name (default is 48)
#define CEF_MAX_EVENT_CALLBACK_LENGTH 72 // extend the maximum length of the callback function name (default is 64)
#define CEF_MAX_EVENT_DATA_LENGTH 4096 // extend the maximum length of callback data (default is 2048)
```

---

## Installation and Setup of the Server-side
- Copy `server/SAMPMobileCef.inc` to your `pawno/include` directory.
- Install dependencies: [`Pawn.RakNet`](https://github.com/katursis/Pawn.RakNet), [`pawn-json`](https://github.com/Southclaws/pawn-json), and [`SA-MP GVar Plugin`](https://github.com/samp-incognito/samp-gvar-plugin).
- Include dependencies and header:
  ```pawn
  #include <Pawn.RakNet>
  #include <json>
  #include <gvar>
  #include <SAMPMobileCef>
  ```
- Declare packet ID macro:
  ```pawn
  #define CEF_PACKET_ID 252
  ```
- Set packet ID in `OnGameModeInit`:
  ```pawn
  public OnGameModeInit()
  {
      CefSetPacketId(CEF_PACKET_ID);
      return 1;
  }
  ```

---

## List of Functions

### Core Functions

#### `CefSetPacketId(packet_id)`
Sets the network packet ID for interacting with CEF.

#### `CefInitBrowser(playerid, const url[])`
Initializes the browser for the specified player with the given URL.

#### `CefDestroyBrowser(playerid)`
Destroys the browser for the specified player.

#### `CefShowBrowser(playerid)`
Shows the browser to the specified player.

#### `CefHideBrowser(playerid)`
Hides the browser for the specified player.

#### `CefToggleBrowser(playerid, bool:show)`
Toggles browser visibility for the player (`true` to show, `false` to hide).

#### `bool:CefIsBrowserShown(playerid)`
Returns `true` if the specified player's browser is currently visible, `false` otherwise.

#### `CefSetBrowserUrl(playerid, const url[])`
Changes the active URL loaded in the specified player's browser.

#### `CefGetBrowserUrl(playerid, dest[], maxlen)`
Retrieves the active URL currently loaded in the player's browser.

#### `CefReloadBrowser(playerid)`
Reloads the currently loaded URL in the player's browser.

#### `CefResetBrowser(playerid)`
Resets the player's browser state (hides browser, clears focus, and resets stored URL).

#### `CefChangeBrowserFocus(playerid, bool:is_focused)`
Changes touch/keyboard focus for the specified player.

#### `CefSetBrowserFocusAll(bool:is_focused)`
Changes touch/keyboard focus for all connected players with CEF support.

---

### Event & Scripting Functions

#### `CefSendEvent(playerid, const event_name[], const event_data[])`
Sends a JSON event payload to the specified player's browser.

#### `CefSendEventToAll(const event_name[], const event_data[])`
Broadcasts a JSON event payload to all online players with CEF support.

#### `CefExecuteJavaScript(playerid, const code[])`
Executes a dynamic JavaScript code string inside the specified player's browser.

#### `CefExecuteJavaScriptToAll(const code[])`
Broadcasts dynamic JavaScript code execution to all online players with CEF support.

#### `CefRegisterEventCallback(const event_name[], const callback[])`
Registers a Pawn callback function to handle events sent from the client JavaScript.

#### `bool:CefUnregisterEventCallback(const event_name[])`
Unregisters a previously registered event callback.

#### `bool:CefHasEventCallback(const event_name[])`
Returns `true` if a callback is registered for the specified event name.

#### `bool:CefIsPlayerHasLibrary(playerid)`
Returns `true` if the player's client supports CEF.

#### `CefGetInitializedPlayerCount()`
Returns the total number of connected players currently online who have CEF initialized.

---

## List of Callbacks

### `OnCefBrowserInit(playerid, is_init, error_code)`
Called after loading the URL specified during browser initialization or manual change.
- `playerid`: Player ID.
- `is_init`: `true` if URL was loaded successfully, `false` otherwise.
- `error_code`: `-1` on success, `0` on unknown error, or HTTP status code.

---

**Copyright © 2024 [Denis Akazuki](https://github.com/denis-akazuki).**
