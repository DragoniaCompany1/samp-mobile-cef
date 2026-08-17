Language: [English](../en/web.md) | **Українська**

# SA:MP Mobile CEF — Документація Frontend Web API

Повна інструкція з розробки веб-інтерфейсів (HTML, CSS, JavaScript) для **SA:MP Mobile CEF**.

---

## 🌐 Документація JavaScript API

Браузерний об'єкт `Cef` ін'єктується автоматично у глобальний контекст `window`.

### 1. `Cef.registerEventCallback(eventName, callbackFunctionName)`
Реєструє функцію зворотного виклику для обробки подій з сервера Pawn (`CefSendEvent` / `CefEmit`).

```javascript
Cef.registerEventCallback("show_welcome", function(dataJson) {
    const data = JSON.parse(dataJson);
    console.log("Welcome Message:", data[0]);
});
```

---

### 2. `Cef.sendEvent(eventName, eventDataJson)`
Надсилає подію та масив JSON даних з веб-інтерфейсу на сервер SA:MP.

```javascript
function submitLoginForm(username, password) {
    const payload = [username, password];
    Cef.sendEvent("login_submit", JSON.stringify(payload));
}
```

---

## 🎨 Обробники компонентів `SAMPMobileCefUI`

- **Повідомлення Notification**: `cef_ui_notification`
- **Модальні діалоги Dialog**: `cef_ui_dialog_show`
- **Програвач аудіо Audio**: `cef_ui_audio_play` та `cef_ui_audio_stop`
- **Інвентар Inventory**: `cef_ui_inventory_open` та `cef_ui_inventory_close`

---

**Original Copyright © 2024 [Denis Akazuki](https://github.com/denis-akazuki)**  
**Upgraded Framework & Client Code Copyright © 2026 [drgxbytezone & Community](https://github.com/drgxbytezone)**  
Licensed under the **MIT License**.
