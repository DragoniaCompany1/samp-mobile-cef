Language: [English](../en/client.md) | **Українська**

# SA:MP Mobile CEF — Інструкція з інтеграції клієнтської частини

Цей посібник пояснює, як інтегрувати відкритий вихідний код **SA:MP Mobile CEF Client Engine** в Android застосунок SA:MP (Android NDK C++ та Android Studio Java).

---

## 📌 Важлива інформація
- Усі клієнтські логи, включаючи повідомлення консолі WebView, автоматично зберігаються у файл `SAMP/cef.log` в директорії гри.
- Клієнтський рушій використовує оверлей Android **WebView (Chromium)** з апаратним прискоренням GPU через міст JNI.

---

## 🛠️ Інсталяція та налаштування (C++ NDK Layer)

Скопіюйте C++ файли з `client/cpp/` у директорію вашого проєкту `vendor/cef/`:
- [SAMPMobileCef.h](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/cpp/SAMPMobileCef.h)
- [SAMPMobileCef.cpp](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/cpp/SAMPMobileCef.cpp)

1. У файлі `Android.mk`:
   ```makefile
   LOCAL_SRC_FILES += vendor/cef/SAMPMobileCef.cpp
   ```

2. У файлі `main.cpp`:
   ```cpp
   #include "vendor/cef/SAMPMobileCef.h"

   // У функції InitSAMP():
   cef::setGamePath(g_pszStorage);
   ```

3. У файлі `net/netgame.cpp`:
   ```cpp
   #include "vendor/cef/SAMPMobileCef.h"

   // У конструкторі CNetGame:
   cef::initNetwork(m_pRakClient, ID_CUSTOM_CEF); // Default packet ID 252

   // У CNetGame::UpdateNetwork:
   switch (packetIdentifier)
   {
       case ID_CUSTOM_CEF:
           cef::handlePacket(pkt);
           break;
   }

   // У CNetGame::Packet_ConnectionSucceeded:
   cef::handleServerConnection();
   ```

---

## 📱 Інсталяція та налаштування (Java Android Studio Layer)

Скопіюйте Java файли з `client/java/` у пакет `com.samp.cef`:
- [CefJavaManager.java](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/java/CefJavaManager.java)
- [CefClientManager.java](file:///home/drgxel/Documents/samp/samp-mobile-cef/client/java/CefClientManager.java)

1. У файлі `NvEventQueueActivity.java`:
   ```java
   private CefJavaManager mJavaManager = null;
   private CefClientManager mClientManager = null;

   // У функції systemInit():
   mJavaManager = new CefJavaManager(mRootFrame, getInstance());
   mClientManager = new CefClientManager(getInstance());

   mJavaManager.setClientManager(mClientManager);
   mClientManager.setJavaManager(mJavaManager);

   // У функції setPauseState(boolean isPaused):
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

**Original Copyright © 2024 [Denis Akazuki](https://github.com/denis-akazuki)**  
**Upgraded Framework & Client Code Copyright © 2026 [drgxbytezone & Community](https://github.com/drgxbytezone)**  
Licensed under the **MIT License**.
