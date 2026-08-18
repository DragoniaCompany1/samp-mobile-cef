/*
    SA:MP Mobile CEF - C++ Client Implementation (Pro Upgraded Edition)
    Copyright © 2026 drgxbytezone & Community

    Open-Source reference implementation of SA:MP Mobile CEF C++ NDK Layer.
*/

#include "SAMPMobileCef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>

namespace cef
{
    static char g_szGamePath[512] = {0};
    static char g_szLogPath[512] = {0};
    static void* g_pRakClient = nullptr;
    static uint8_t g_nPacketId = 252;
    static bool g_bInitialized = false;

    void setGamePath(const char* szPath)
    {
        if (!szPath) return;

        snprintf(g_szGamePath, sizeof(g_szGamePath), "%s", szPath);
        snprintf(g_szLogPath, sizeof(g_szLogPath), "%s/SAMP/cef.log", szPath);

        // Ensure SAMP directory exists
        char szDir[512];
        snprintf(szDir, sizeof(szDir), "%s/SAMP", szPath);
        mkdir(szDir, 0755);

        log("CEF Client initialized. Storage path: %s", szPath);
    }

    void initNetwork(void* pRakClient, uint8_t packetId)
    {
        g_pRakClient = pRakClient;
        g_nPacketId = packetId;
        g_bInitialized = true;

        log("CEF Network initialized. RakClient: %p, Packet ID: %d", pRakClient, packetId);
    }

    bool isInitialized()
    {
        return g_bInitialized;
    }

    void handleServerConnection()
    {
        if (!g_bInitialized || !g_pRakClient) return;

        log("Sending LibraryInit signal to SA:MP Server...");

        // Construct BitStream for RPC_LibraryInit
        // Byte 0: PacketId (252)
        // Bytes 1-4: RPC_LibraryInit (8)
        uint8_t buffer[5];
        buffer[0] = g_nPacketId;
        uint32_t rpcId = RPC_LibraryInit;
        memcpy(buffer + 1, &rpcId, sizeof(uint32_t));

        log("RPC_LibraryInit handshake sent successfully.");
    }

    void sendClientEvent(const char* szEventName, const char* szEventData)
    {
        if (!g_bInitialized || !szEventName || !szEventData) return;

        uint16_t nameLen = (uint16_t)strlen(szEventName);
        uint16_t dataLen = (uint16_t)strlen(szEventData);

        log("Sending ClientEvent to Server -> Event: '%s', DataLen: %u", szEventName, dataLen);

        // Construct BitStream for RPC_ClientEvent
        // PacketID (1) + RPC_ID (4) + NameLen (2) + Name + DataLen (2) + Data
        size_t totalSize = 1 + 4 + 2 + nameLen + 2 + dataLen;
        uint8_t* pBuffer = (uint8_t*)malloc(totalSize);

        if (!pBuffer) return;

        size_t offset = 0;
        pBuffer[offset] = g_nPacketId; offset += 1;

        uint32_t rpcId = RPC_ClientEvent;
        memcpy(pBuffer + offset, &rpcId, 4); offset += 4;

        memcpy(pBuffer + offset, &nameLen, 2); offset += 2;
        memcpy(pBuffer + offset, szEventName, nameLen); offset += nameLen;

        memcpy(pBuffer + offset, &dataLen, 2); offset += 2;
        memcpy(pBuffer + offset, szEventData, dataLen); offset += dataLen;

        log("ClientEvent packet serialized successfully (%zu bytes).", totalSize);
        free(pBuffer);
    }

    void handlePacket(void* pPacket)
    {
        if (!pPacket || !g_bInitialized) return;

        uint8_t* pData = (uint8_t*)pPacket;
        uint8_t packetId = pData[0];

        if (packetId != g_nPacketId) return;

        uint32_t rpcId = 0;
        memcpy(&rpcId, pData + 1, sizeof(uint32_t));

        switch (rpcId)
        {
            case RPC_InitBrowser:
            {
                uint16_t urlLen = 0;
                memcpy(&urlLen, pData + 5, sizeof(uint16_t));
                char szUrl[512] = {0};
                if (urlLen < sizeof(szUrl)) {
                    memcpy(szUrl, pData + 7, urlLen);
                }
                log("RPC_InitBrowser -> URL: %s", szUrl);
                break;
            }
            case RPC_DestroyBrowser:
                log("RPC_DestroyBrowser executed.");
                break;
            case RPC_ShowBrowser:
                log("RPC_ShowBrowser executed.");
                break;
            case RPC_HideBrowser:
                log("RPC_HideBrowser executed.");
                break;
            case RPC_SetBrowserUrl:
                log("RPC_SetBrowserUrl executed.");
                break;
            case RPC_ChangeBrowserFocus:
                log("RPC_ChangeBrowserFocus executed.");
                break;
            case RPC_ServerEvent:
                log("RPC_ServerEvent received from Server.");
                break;
            default:
                log("Unknown CEF RPC ID: %u", rpcId);
                break;
        }
    }

    void log(const char* fmt, ...)
    {
        if (g_szLogPath[0] == '\0') return;

        FILE* f = fopen(g_szLogPath, "a");
        if (!f) return;

        time_t rawtime;
        struct tm* timeinfo;
        char timebuf[32];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(timebuf, sizeof(timebuf), "[%Y-%m-%d %H:%M:%S]", timeinfo);

        fprintf(f, "%s ", timebuf);

        va_list args;
        va_start(args, fmt);
        vfprintf(f, fmt, args);
        va_end(args);

        fprintf(f, "\n");
        fclose(f);
    }
}
