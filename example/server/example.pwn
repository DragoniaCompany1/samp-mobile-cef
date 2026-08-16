#include <open.mp> // or #include <a_samp> for basic SA-MP server

#include <Pawn.RakNet>
#include <json>
#include <gvar>
#include <SAMPMobileCef>

#define CEF_PACKET_ID 252

enum
{
    ALERT_SET_SKIN = 1
};

main()
{
    print("CEF Example Server (Upgraded Edition)");
    print("GitHub: https://github.com/denis-akazuki/samp-mobile-cef");
}

forward OnAlertResponse(playerid, event_data[]);
public OnAlertResponse(playerid, event_data[])
{
    CefChangeBrowserFocus(playerid, false);

    new Node:event_data_node, Node:element_buffer_node;

    JSON_Parse(event_data, event_data_node);

    new alert_id;
    JSON_ArrayObject(event_data_node, 0, element_buffer_node);
    JSON_GetNodeInt(element_buffer_node, alert_id);

    new bool:response;
    JSON_ArrayObject(event_data_node, 1, element_buffer_node);
    JSON_GetNodeBool(element_buffer_node, response);

    switch (alert_id)
    {
        case ALERT_SET_SKIN:
        {
            if (!response)
                return 1;

            SetPlayerSkin(playerid, 150);
            SendClientMessage(playerid, -1, "[CEF] New skin: 150");
        }
    }

    return 1;
}

public OnCefBrowserInit(playerid, is_init, error_code)
{
    if (!is_init)
    {
        SendClientMessage(playerid, -1, "[CEF] It looks like you don't have CEF support!");
        return 1;
    }

    SendClientMessage(playerid, -1, "[CEF] All is okay! Browser initialized successfully.");
    CefShowBrowser(playerid);

    return 1;
}

public OnGameModeInit()
{
    CefSetPacketId(CEF_PACKET_ID);
    CefRegisterEventCallback("alert_response", "OnAlertResponse");

    return 1;
}

public OnGameModeExit()
{
    return 1;
}

public OnPlayerConnect(playerid)
{
    /* EN: android_asset - app/src/main/assets/ directory in Android application */
    CefInitBrowser(playerid, "file:///android_asset/cef/index.html");
    CefChangeBrowserFocus(playerid, false);

    SetSpawnInfo(playerid, NO_TEAM, 0, 2481.2441, -1911.9337, 21.4856, 90.0000, WEAPON_SAWEDOFF, 36, WEAPON_UZI, 150, WEAPON_FIST, 0);
    SpawnPlayer(playerid);

    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    CefDestroyBrowser(playerid);

    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    SpawnPlayer(playerid);

    return 1;
}

public OnPlayerSpawn(playerid)
{
    TogglePlayerControllable(playerid, true);

    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/change_skin", true))
    {
        if (CefIsPlayerHasLibrary(playerid))
        {
            new Node:event_data_node = JSON_Array(
                JSON_Int(ALERT_SET_SKIN),
                JSON_String("Change skin"),
                JSON_String("Do you want to change the skin to female?")
            );

            new event_data[CEF_MAX_EVENT_DATA_LENGTH];
            JSON_Stringify(event_data_node, event_data);

            CefSendEvent(playerid, "alert_show", event_data);
            CefChangeBrowserFocus(playerid, true);
        }
        return 1;
    }

    // NEW UPGRADED COMMAND: Toggle CEF browser visibility
    if (!strcmp(cmdtext, "/toggle_cef", true))
    {
        if (CefIsPlayerHasLibrary(playerid))
        {
            new bool:is_shown = CefIsBrowserShown(playerid);
            CefToggleBrowser(playerid, !is_shown);
            
            new msg[64];
            format(msg, sizeof(msg), "[CEF] Browser visibility toggled: %s", (!is_shown) ? ("SHOWN") : ("HIDDEN"));
            SendClientMessage(playerid, -1, msg);
        }
        return 1;
    }

    // NEW UPGRADED COMMAND: Broadcast CEF event to all online players
    if (!strcmp(cmdtext, "/broadcast_cef", true))
    {
        new Node:event_data_node = JSON_Array(
            JSON_Int(0),
            JSON_String("Server Announcement"),
            JSON_String("Hello to all CEF SA:MP Mobile Players!")
        );

        new event_data[CEF_MAX_EVENT_DATA_LENGTH];
        JSON_Stringify(event_data_node, event_data);

        CefSendEventToAll("alert_show", event_data);
        SendClientMessage(playerid, -1, "[CEF] Broadcast event sent to all players!");
        return 1;
    }

    return 0;
}
