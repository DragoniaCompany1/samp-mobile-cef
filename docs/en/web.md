Language: **English** | [Українська](../uk/web.md)

# SA:MP Mobile CEF — Frontend Web API Documentation

Comprehensive JavaScript API reference for designing Web User Interfaces (HTML, CSS, JavaScript) inside **SA:MP Mobile CEF**.

---

## 🌐 JavaScript Web API Reference

The client engine automatically injects the global `Cef` object into the browser context.

### 1. Listening to Server Events

#### `Cef.registerEventCallback(eventName, callbackFunctionName)`
Registers a JavaScript function to listen for events sent from the SA:MP Pawn server.

- **Parameters**:
  - `eventName` (*String*): The event name sent from server (`CefSendEvent`).
  - `callbackFunctionName` (*String* | *Function*): The function name or closure executed when the event is received.

**Example Usage**:
```javascript
// Register callback for custom server event
Cef.registerEventCallback("show_welcome", function(dataJson) {
    const data = JSON.parse(dataJson);
    console.log("Welcome Message:", data[0]);
});
```

---

### 2. Sending Events Back to Server

#### `Cef.sendEvent(eventName, eventDataJson)`
Sends a JSON event payload from the Web UI back to the SA:MP Pawn server.

- **Parameters**:
  - `eventName` (*String*): The name of the client event.
  - `eventDataJson` (*String*): Serialized JSON Array string containing payload data.

**Example Usage**:
```javascript
function submitLoginForm(username, password) {
    const payload = [username, password];

    // Send event back to Pawn server
    Cef.sendEvent("login_submit", JSON.stringify(payload));
}
```

---

## 🎨 Built-in UI Framework JavaScript Handlers (`SAMPMobileCefUI`)

When using `SAMPMobileCefUI.inc` on the server, you can listen to the following standard events in your JavaScript UI:

### 🔔 1. Notifications & Toasts (`cef_ui_notification`)
```javascript
Cef.registerEventCallback("cef_ui_notification", function(eventDataJson) {
    const data = JSON.parse(eventDataJson);
    const title = data[0];
    const message = data[1];
    const type = data[2]; // "info", "success", "warning", "error"
    const duration = data[3]; // Duration in ms

    // Render toast notification in DOM
    showToast(title, message, type, duration);
});
```

### 💬 2. Web Dialog Modals (`cef_ui_dialog_show`)
```javascript
Cef.registerEventCallback("cef_ui_dialog_show", function(eventDataJson) {
    const data = JSON.parse(eventDataJson);
    const dialogId = data[0];
    const dialogType = data[1]; // "msgbox", "input", "password", "list", "confirm"
    const title = data[2];
    const message = data[3];
    const btn1 = data[4];
    const btn2 = data[5];

    // Show Modal Dialog in DOM and send response back when user clicks a button:
    // Cef.sendEvent("cef_ui_dialog_response", JSON.stringify([dialogId, 1, inputVal]));
});
```

### 🎵 3. HTML5 Audio Player (`cef_ui_audio_play`)
```javascript
let currentAudio = null;

Cef.registerEventCallback("cef_ui_audio_play", function(eventDataJson) {
    const data = JSON.parse(eventDataJson);
    const soundUrl = data[0];
    const isLoop = data[1];
    const volume = data[2];

    if (currentAudio) currentAudio.pause();
    currentAudio = new Audio(soundUrl);
    currentAudio.loop = isLoop;
    currentAudio.volume = volume;
    currentAudio.play();
});

Cef.registerEventCallback("cef_ui_audio_stop", function() {
    if (currentAudio) {
        currentAudio.pause();
        currentAudio = null;
    }
});
```

---

## 💡 Web Development Best Practices

1. **Keep Background Transparent**:
   Set `body { background-color: transparent; }` so that the GTA San Andreas game screen is visible behind your UI overlays.
2. **Use Hardware Acceleration & Touch Optimization**:
   Design UI elements with touch-friendly sizes (minimum 44x44px touch targets for mobile fingers).
3. **Local Storage & State Persistence**:
   You can use standard browser `localStorage` to save player preferences (e.g. volume settings, theme mode) across game sessions.

---

**Original Copyright © 2024 [Denis Akazuki](https://github.com/denis-akazuki)**  
**Upgraded Framework & Client Code Copyright © 2026 [drgxbytezone & Community](https://github.com/drgxbytezone)**  
Licensed under the **MIT License**.
