/*
    SA:MP Mobile CEF - UCP Registration Server Example (Pawn Script)
    File: example/register/register_example.pwn
*/

#include <open.mp> // or #include <a_samp>
#include <Pawn.RakNet>
#include <json>
#include <gvar>
#include <SAMPMobileCef>

#define CEF_PACKET_ID 252

main()
{
    print("CEF Register Example Server initialized.");
}

// Forward Callback Declarations
forward OnRegisterSubmit(playerid, event_data[]);
forward OnRegisterDiscord(playerid, event_data[]);
forward OnRegisterCancel(playerid, event_data[]);
forward OnRegisterFinish(playerid, event_data[]);

public OnGameModeInit()
{
    // Set network packet ID for CEF
    CefSetPacketId(CEF_PACKET_ID);

    // Register Web UI callbacks to Pawn functions
    CefRegisterEventCallback("register_submit", "OnRegisterSubmit");
    CefRegisterEventCallback("register_discord", "OnRegisterDiscord");
    CefRegisterEventCallback("register_cancel", "OnRegisterCancel");
    CefRegisterEventCallback("register_finish", "OnRegisterFinish");

    return 1;
}

public OnPlayerConnect(playerid)
{
    // Load Register UI when player joins server
    CefInitBrowser(playerid, "file:///android_asset/cef/register/index.html");
    CefChangeBrowserFocus(playerid, true); // Enable touch/mouse focus on Web UI

    return 1;
}

public OnCefBrowserInit(playerid, is_init, error_code)
{
    if (is_init)
    {
        CefShowBrowser(playerid);
    }
    return 1;
}

// Callback when player submits the UCP registration form in Web UI
public OnRegisterSubmit(playerid, event_data[])
{
    new Node:event_node, Node:buffer_node;
    JSON_Parse(event_data, event_node);

    new ucp_name[32], char_name[32], password[64];
    
    JSON_ArrayObject(event_node, 0, buffer_node);
    JSON_GetNodeString(buffer_node, ucp_name);

    JSON_ArrayObject(event_node, 1, buffer_node);
    JSON_GetNodeString(buffer_node, char_name);

    JSON_ArrayObject(event_node, 2, buffer_node);
    JSON_GetNodeString(buffer_node, password);

    printf("[SERVER CEF] Register Attempt -> UCP: %s | Char: %s", ucp_name, char_name);

    // SIMULATION: Generate 6-digit PIN for UCP Account
    new random_pin = 100000 + random(899999);
    new pin_str[8];
    format(pin_str, sizeof(pin_str), "%d", random_pin);

    // SIMULATION: Check if registration is successful (Always true for demo)
    new bool:is_success = true;

    // Send result back to Web UI
    new Node:response_node = JSON_Array(
        JSON_Bool(is_success),
        JSON_String("Registrasi Berhasil!"),
        JSON_String(pin_str),
        JSON_String(ucp_name),
        JSON_String(char_name)
    );

    new response_json[512];
    JSON_Stringify(response_node, response_json);

    // Send event back to Web UI to display UCP PIN & Success screen
    CefSendEvent(playerid, "register_response", response_json);

    return 1;
}

// Callback when player clicks "Hubungkan Akun Discord"
public OnRegisterDiscord(playerid, event_data[])
{
    SendClientMessage(playerid, -1, "[CEF] Membuka otentikasi Discord...");
    // Here you can send OAuth URL or trigger bot verification logic
    return 1;
}

// Callback when player cancels registration
public OnRegisterCancel(playerid, event_data[])
{
    CefHideBrowser(playerid);
    CefChangeBrowserFocus(playerid, false);
    Kick(playerid);
    return 1;
}

// Callback when player clicks "Masuk ke Dalam Game" after saving PIN
public OnRegisterFinish(playerid, event_data[])
{
    CefHideBrowser(playerid);
    CefChangeBrowserFocus(playerid, false);

    SendClientMessage(playerid, 0x00FF00FF, "[SERVER] Selamat datang di server! Karakter Anda telah di-spawn.");
    SetSpawnInfo(playerid, NO_TEAM, 0, 2481.2441, -1911.9337, 21.4856, 90.0000, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);

    return 1;
}
