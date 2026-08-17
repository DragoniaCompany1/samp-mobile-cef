Language: **English** | [Українська](../uk/client.md)

# SA:MP Mobile CEF — Client-Side Integration Guide

This guide explains how to integrate the open-source **SA:MP Mobile CEF Client Engine** into an Android SA:MP Client application (Android NDK C++ and Android Studio Java).

---

## 📌 Important Information
- All client-side logs, including WebView errors and console messages, are automatically appended to `SAMP/cef.log` inside the game storage path.
- The client engine uses an Android **WebView (Chromium)** overlay managed via JNI bridge to ensure lightweight APK size and high-performance GPU hardware acceleration.

---

## 🛠️ Installation & Setup (C++ NDK Layer)

### Method A: Direct Open-Source Compilation (Recommended)
Copy the open-source C++ files from `client/cpp/` into your client project's `vendor/cef/` directory:
- [SAMPMobileCef.h](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/cpp/SAMPMobileCef.h)
- [SAMPMobileCef.cpp](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/cpp/SAMPMobileCef.cpp)

1. In your `Android.mk` build manifest:
   ```makefile
   LOCAL_SRC_FILES += vendor/cef/SAMPMobileCef.cpp
   ```

2. In your client's `main.cpp`:
   ```cpp
   #include "vendor/cef/SAMPMobileCef.h"

   // Inside InitSAMP() function:
   cef::setGamePath(g_pszStorage);
   ```

3. In your client's `net/netgame.cpp`:
   ```cpp
   #include "vendor/cef/SAMPMobileCef.h"

   // In CNetGame constructor:
   cef::initNetwork(m_pRakClient, ID_CUSTOM_CEF); // Default packet ID is 252

   // In CNetGame::UpdateNetwork (packet processing loop):
   switch (packetIdentifier)
   {
       case ID_CUSTOM_CEF:
           cef::handlePacket(pkt);
           break;
   }

   // In CNetGame::Packet_ConnectionSucceeded:
   cef::handleServerConnection();
   ```

---

## 📱 Installation & Setup (Java Android Studio Layer)

Copy the open-source Java files from `client/java/` into package `com.samp.cef`:
- [CefJavaManager.java](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/java/CefJavaManager.java)
- [CefClientManager.java](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/java/CefClientManager.java)

1. In `NvEventQueueActivity.java`:
   ```java
   private CefJavaManager mJavaManager = null;
   private CefClientManager mClientManager = null;

   // Inside systemInit():
   mJavaManager = new CefJavaManager(mRootFrame, getInstance());
   mClientManager = new CefClientManager(getInstance());

   mJavaManager.setClientManager(mClientManager);
   mClientManager.setJavaManager(mJavaManager);

   // Inside setPauseState(boolean isPaused):
   public void setPauseState(boolean isPaused) {
       runOnUiThread(() -> {
           if (mJavaManager.isShow()) {
               if (isPaused)
                   mJavaManager.hideBrowserView();
               else
                   mJavaManager.showBrowserView();
           }
       });
   }
   ```

---

## 📖 C++ Native API Reference

| Function | Signature | Description |
|---|---|---|
| `setGamePath` | `cef::setGamePath(const char* szPath)` | Configures client game storage path for logging (`SAMP/cef.log`). |
| `initNetwork` | `cef::initNetwork(void* pRakClient, uint8_t packetId)` | Binds RakClient pointer and custom packet ID (e.g. 252). |
| `sendClientEvent` | `cef::sendClientEvent(const char* name, const char* data)` | Serializes and sends client JavaScript event to SA:MP Server. |
| `handlePacket` | `cef::handlePacket(void* pPacket)` | Parses incoming RakNet BitStream packets from SA:MP Server. |
| `handleServerConnection` | `cef::handleServerConnection()` | Sends initial `RPC_LibraryInit` packet to SA:MP Server. |
| `isInitialized` | `bool:cef::isInitialized()` | Returns `true` if network layer is initialized. |
| `log` | `cef::log(const char* fmt, ...)` | Writes formatted log entries to `SAMP/cef.log`. |

---

**Original Copyright © 2024 [Denis Akazuki](https://github.com/denis-akazuki)**  
**Upgraded Framework & Client Code Copyright © 2026 [drgxbytezone & Community](https://github.com/drgxbytezone)**  
Licensed under the **MIT License**.
