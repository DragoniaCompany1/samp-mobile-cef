Language: **English** | [Українська](../uk/server.md)

# SA:MP Mobile CEF — Server API Documentation

Comprehensive server-side Pawn API reference for **`SAMPMobileCef.inc`** and **`SAMPMobileCefUI.inc`**.

---

## 📌 Memory & Configuration Limits
If the data length of events exceeds default limits, you can override macros **before** including `SAMPMobileCef.inc`:

```pawn
#define CEF_MAX_EVENT_NAME_LENGTH 64      // Default: 48
#define CEF_MAX_EVENT_CALLBACK_LENGTH 72  // Default: 64
#define CEF_MAX_EVENT_DATA_LENGTH 4096    // Default: 2048

#include <Pawn.RakNet>
#include <json>
#include <gvar>

#include <SAMPMobileCef>
#include <SAMPMobileCefUI>
```

---

## ⚙️ Installation & Setup

1. Copy `server/SAMPMobileCef.inc` and `server/SAMPMobileCefUI.inc` into your `pawno/include/` directory.
2. Include dependencies and headers in your gamemode:
   ```pawn
   #include <Pawn.RakNet>
   #include <json>
   #include <gvar>

   #include <SAMPMobileCef>
   #include <SAMPMobileCefUI>
   ```
3. Set the network packet ID in `OnGameModeInit`:
   ```pawn
   #define CEF_PACKET_ID 252

   public OnGameModeInit()
   {
       CefSetPacketId(CEF_PACKET_ID);
       CefSetEventRateLimit(20); // Limit to 20 events/sec per player
       return 1;
   }
   ```

---

## 📖 Core Server Functions (`SAMPMobileCef.inc`)

### Browser Management
- `CefSetPacketId(packet_id)`: Configures custom RakNet network packet ID.
- `CefSetEventRateLimit(max_events_per_sec)`: Configures anti-spam event rate limiter per player.
- `CefInitBrowser(playerid, const url[])`: Initializes player WebView overlay.
- `CefDestroyBrowser(playerid)`: Destroys player WebView instance.
- `CefShowBrowser(playerid)`: Shows player WebView overlay.
- `CefHideBrowser(playerid)`: Hides player WebView overlay.
- `CefToggleBrowser(playerid, bool:show)`: Toggles browser visibility.
- `bool:CefIsBrowserShown(playerid)`: Returns `true` if browser is currently shown.
- `CefSetBrowserZoom(playerid, Float:scale)`: Scales UI zoom factor (e.g. 1.25 for 1080p).
- `Float:CefGetBrowserZoom(playerid)`: Returns player's active zoom scale factor.
- `CefClearBrowserCache(playerid)`: Clears localStorage, sessionStorage, and reloads browser.
- `CefSetBrowserUrl(playerid, const url[])`: Changes active URL.
- `CefGetBrowserUrl(playerid, dest[], maxlen)`: Retrieves active URL string.
- `CefReloadBrowser(playerid)`: Reloads active URL.
- `CefResetBrowser(playerid)`: Resets player WebView state.
- `CefChangeBrowserFocus(playerid, bool:is_focused)`: Changes input focus.
- `CefSetBrowserFocusAll(bool:is_focused)`: Changes input focus for all connected players.

### Event Communication
- `CefSendEvent(playerid, const event_name[], const event_data[])`: Sends JSON event to player.
- `CefSendEventToAll(const event_name[], const event_data[])`: Broadcasts JSON event to all players.
- `CefEmit(playerid, const event_name[], const format_str[], ...)`: Sends formatted JSON event directly.
- `CefEmitToAll(const event_name[], const format_str[], ...)`: Broadcasts formatted JSON event directly.
- `CefExecuteJavaScript(playerid, const code[])`: Executes custom JS string in player WebView.
- `CefExecuteJavaScriptToAll(const code[])`: Broadcasts custom JS string execution to all players.
- `CefRegisterEventCallback(const event_name[], const callback[])`: Registers Pawn callback.
- `bool:CefUnregisterEventCallback(const event_name[])`: Removes registered callback.
- `bool:CefHasEventCallback(const event_name[])`: Checks if callback is registered.
- `bool:CefIsPlayerHasLibrary(playerid)`: Checks if player client supports CEF.
- `bool:CefIsClientConnected(playerid)`: Checks if player is connected with CEF initialized.
- `CefGetInitializedPlayerCount()`: Returns count of active CEF players online.

---

## 🎨 High-Level Framework Functions (`SAMPMobileCefUI.inc`)

### 1. Notifications & Toasts
```pawn
CefShowNotification(playerid, "Success", "Purchase completed!", "success", 3000);
CefShowNotificationToAll("Event Announcement", "Server restart in 10 minutes", "warning", 5000);
```

### 2. Web Dialogs & Modals
```pawn
CefShowDialog(playerid, DIALOG_RULES, "Rules", "Follow server rules!", "Agree", "Close");
CefShowInputDialog(playerid, DIALOG_LOGIN, "Login", "Enter password:", "Login", "Cancel", true);
CefShowConfirmDialog(playerid, DIALOG_BUY, "Confirm", "Buy Infernus for $1,500,000?", "Yes", "No");

new list_json[] = "[\"Infernus - $1.5M\", \"Turismo - $1.2M\"]";
CefShowListDialog(playerid, DIALOG_GARAGE, "Garage", list_json, "Select", "Cancel");

new menu_json[] = "[\"Inspect\", \"Trade\", \"Invite\"]";
CefShowContextMenu(playerid, MENU_PLAYER, "Player Actions", menu_json);
```

### Gamemode Callback Hooks
```pawn
public OnCefDialogResponse(playerid, dialogid, response, const inputtext[]) { ... }
public OnCefListDialogResponse(playerid, dialogid, response, item_index, const item_value[]) { ... }
public OnCefConfirmDialogResponse(playerid, dialogid, response) { ... }
public OnCefContextMenuResponse(playerid, menu_id, item_index, const item_value[]) { ... }
```

### 3. HTML5 Audio & Sound Player
```pawn
CefPlayAudio(playerid, "https://example.com/sfx/welcome.mp3", false, 0.8);
CefStopAudio(playerid);
CefSetAudioVolume(playerid, 0.5);
```

### 4. Inventory Grid Framework
```pawn
new items_json[] = "[{\"id\":1, \"name\":\"Medkit\", \"count\":3}]";
CefOpenInventory(playerid, items_json);
CefCloseInventory(playerid);

public OnCefInventoryUseItem(playerid, slot_id, const item_name[]) { ... }
```

---

## 📡 Callbacks Reference

### `OnCefBrowserInit(playerid, is_init, error_code)`
Called after loading URL specified during initialization or manual URL change.
- `playerid`: Player ID.
- `is_init`: `true` if URL was loaded successfully, `false` otherwise.
- `error_code`: `-1` on success, `0` on unknown error, or HTTP status code.

---

**Original Copyright © 2024 [Denis Akazuki](https://github.com/denis-akazuki)**  
**Upgraded Framework & Client Code Copyright © 2026 [drgxbytezone & Community](https://github.com/drgxbytezone)**  
Licensed under the **MIT License**.
