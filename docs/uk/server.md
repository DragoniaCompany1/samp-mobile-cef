Language: [English](../en/server.md) | **Українська**

# SA:MP Mobile CEF — Серверна документація API

Повна серверна документація Pawn API для **`SAMPMobileCef.inc`** та **`SAMPMobileCefUI.inc`**.

---

## 📌 Обмеження пам'яті
Якщо довжина даних перевищує стандартні ліміти, перевизначте макроси **до** підключення `SAMPMobileCef.inc`:

```pawn
#define CEF_MAX_EVENT_NAME_LENGTH 64      // Standard: 48
#define CEF_MAX_EVENT_CALLBACK_LENGTH 72  // Standard: 64
#define CEF_MAX_EVENT_DATA_LENGTH 4096    // Standard: 2048

#include <Pawn.RakNet>
#include <json>
#include <gvar>

#include <SAMPMobileCef>
#include <SAMPMobileCefUI>
```

---

## ⚙️ Налаштування серверної частини

1. Скопіюйте `server/SAMPMobileCef.inc` та `server/SAMPMobileCefUI.inc` у директорію `pawno/include/`.
2. Підключіть файли у вашому гейммоді:
   ```pawn
   #include <Pawn.RakNet>
   #include <json>
   #include <gvar>

   #include <SAMPMobileCef>
   #include <SAMPMobileCefUI>
   ```
3. Вкажіть ID мережевого пакета у `OnGameModeInit`:
   ```pawn
   #define CEF_PACKET_ID 252

   public OnGameModeInit()
   {
       CefSetPacketId(CEF_PACKET_ID);
       CefSetEventRateLimit(20);
       return 1;
   }
   ```

---

## 📖 Основні функції (`SAMPMobileCef.inc`)

- `CefSetPacketId(packet_id)`: Налаштування network packet ID.
- `CefSetEventRateLimit(max_events_per_sec)`: Налаштування ліміту подій проти спаму.
- `CefInitBrowser(playerid, const url[])`: Ініціалізація браузера гравця.
- `CefDestroyBrowser(playerid)`: Знищення екземпляра браузера.
- `CefShowBrowser(playerid)`: Відображення браузера.
- `CefHideBrowser(playerid)`: Приховування браузера.
- `CefToggleBrowser(playerid, bool:show)`: Перемикання видимості.
- `bool:CefIsBrowserShown(playerid)`: Перевірка видимості браузера.
- `CefSetBrowserZoom(playerid, Float:scale)`: Зміна масштабу UI.
- `CefClearBrowserCache(playerid)`: Очищення кешу та localStorage.
- `CefSendEvent(playerid, const event_name[], const event_data[])`: Відправка JSON події.
- `CefSendEventToAll(const event_name[], const event_data[])`: Бродкаст JSON події всім гравцям.
- `CefEmit(playerid, const event_name[], const format_str[], ...)`: Пряма відправка відформатованих даних.
- `CefExecuteJavaScript(playerid, const code[])`: Виконання JS коду у гравця.

---

## 🎨 Компоненти UI (`SAMPMobileCefUI.inc`)

- `CefShowNotification(playerid, title, message, type, duration)`
- `CefShowDialog(playerid, dialogid, title, message, button1, button2)`
- `CefShowInputDialog(playerid, dialogid, title, message, button1, button2, is_password)`
- `CefShowListDialog(playerid, dialogid, title, items_json, button1, button2)`
- `CefShowConfirmDialog(playerid, dialogid, title, message, btn_yes, btn_no)`
- `CefShowContextMenu(playerid, menu_id, title, options_json)`
- `CefPlayAudio(playerid, sound_url, loop, volume)`
- `CefOpenInventory(playerid, items_json)`

---

**Original Copyright © 2024 [Denis Akazuki](https://github.com/denis-akazuki)**  
**Upgraded Framework & Client Code Copyright © 2026 [drgxbytezone & Community](https://github.com/drgxbytezone)**  
Licensed under the **MIT License**.
