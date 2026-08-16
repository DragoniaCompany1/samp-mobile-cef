/*
	EN: High-Level UI Component & Helper Framework for SA:MP Mobile CEF (Pro Edition).
	UK: Високорівневий фреймворк компонентів інтерфейсу для SA:MP Mobile CEF.

	Copyright © 2026 Denis Akazuki & Community

	Provides convenient high-level functions for Notifications, Web Dialogs, List/Table Dialogs,
	Audio/SFX Player, Inventory Grids, HUD/Speedometers, Progress Bars, and Dialog Callbacks.
*/

#if !defined SAMPMOBILECEF_UI_INC_
	#define SAMPMOBILECEF_UI_INC_

	#if !defined SAMPMOBILECEF_INC_
		#error "[CEF UI] Please include <SAMPMobileCef> before <SAMPMobileCefUI>."
	#endif

	#define CEF_UI_INC_VERSION "1.4.0-Upgraded"

	// Forward Callback Declarations for Custom Gamemode Hooks
	forward OnCefDialogResponse(playerid, dialogid, response, const inputtext[]);
	forward OnCefListDialogResponse(playerid, dialogid, response, item_index, const item_value[]);
	forward OnCefInventoryUseItem(playerid, slot_id, const item_name[]);

	/*
		=========================================================
		1. NOTIFICATION & TOAST HELPER FUNCTIONS
		=========================================================
	*/

	stock CefShowNotification(playerid, const title[], const message[], const type[] = "info", duration = 3000)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(
			JSON_String(title),
			JSON_String(message),
			JSON_String(type),
			JSON_Int(duration)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_notification", data_str);
		return true;
	}

	stock CefShowNotificationToAll(const title[], const message[], const type[] = "info", duration = 3000)
	{
		new Node:data_node = JSON_Array(
			JSON_String(title),
			JSON_String(message),
			JSON_String(type),
			JSON_Int(duration)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEventToAll("cef_ui_notification", data_str);
		return true;
	}

	/*
		=========================================================
		2. WEB DIALOG & MODAL SYSTEM
		=========================================================
	*/

	stock CefShowDialog(playerid, dialogid, const title[], const message[], const button1[], const button2[] = "")
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(
			JSON_Int(dialogid),
			JSON_String("msgbox"),
			JSON_String(title),
			JSON_String(message),
			JSON_String(button1),
			JSON_String(button2)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_dialog_show", data_str);
		CefChangeBrowserFocus(playerid, true);
		return true;
	}

	stock CefShowInputDialog(playerid, dialogid, const title[], const message[], const button1[], const button2[] = "", bool:is_password = false)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(
			JSON_Int(dialogid),
			JSON_String((is_password) ? ("password") : ("input")),
			JSON_String(title),
			JSON_String(message),
			JSON_String(button1),
			JSON_String(button2)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_dialog_show", data_str);
		CefChangeBrowserFocus(playerid, true);
		return true;
	}

	/**
	 * Display an interactive List/Table Web Dialog box to a player (with Search Bar).
	 * @param playerid Player ID.
	 * @param dialogid Unique integer dialog ID.
	 * @param title Dialog Title.
	 * @param items_json Packed JSON Array string of items: ["Item 1", "Item 2", "Item 3"].
	 * @param button1 Select button text.
	 * @param button2 Cancel button text.
	 */
	stock CefShowListDialog(playerid, dialogid, const title[], const items_json[], const button1[], const button2[] = "")
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:items_node;
		JSON_Parse(items_json, items_node);

		new Node:data_node = JSON_Array(
			JSON_Int(dialogid),
			JSON_String("list"),
			JSON_String(title),
			items_node,
			JSON_String(button1),
			JSON_String(button2)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_dialog_show", data_str);
		CefChangeBrowserFocus(playerid, true);
		return true;
	}

	/*
		=========================================================
		3. HTML5 AUDIO & SOUND PLAYER SYSTEM
		=========================================================
	*/

	/**
	 * Play an audio track / SFX sound in player's WebView.
	 * @param playerid Player ID.
	 * @param sound_url Direct URL to audio file (MP3, WAV, OGG).
	 * @param loop If true, audio will loop continuously.
	 * @param volume Audio volume (0.0 to 1.0).
	 */
	stock CefPlayAudio(playerid, const sound_url[], bool:loop = false, Float:volume = 1.0)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(
			JSON_String(sound_url),
			JSON_Bool(loop),
			JSON_Float(volume)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_audio_play", data_str);
		return true;
	}

	/**
	 * Stop active HTML5 audio in player's WebView.
	 */
	stock CefStopAudio(playerid)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		CefSendEvent(playerid, "cef_ui_audio_stop", "[]");
		return true;
	}

	/**
	 * Change volume of active HTML5 audio in player's WebView.
	 */
	stock CefSetAudioVolume(playerid, Float:volume)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(JSON_Float(volume));
		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_audio_volume", data_str);
		return true;
	}

	/*
		=========================================================
		4. INVENTORY GRID SYSTEM
		=========================================================
	*/

	/**
	 * Open an interactive Inventory Grid Web UI for a player.
	 * @param playerid Player ID.
	 * @param items_json JSON Array string of inventory slots.
	 */
	stock CefOpenInventory(playerid, const items_json[])
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:items_node;
		JSON_Parse(items_json, items_node);

		new Node:data_node = JSON_Array(items_node);
		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_inventory_open", data_str);
		CefChangeBrowserFocus(playerid, true);
		return true;
	}

	/**
	 * Close Inventory Grid Web UI.
	 */
	stock CefCloseInventory(playerid)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		CefSendEvent(playerid, "cef_ui_inventory_close", "[]");
		CefChangeBrowserFocus(playerid, false);
		return true;
	}

	/*
		=========================================================
		5. SPEEDOMETER & HUD HELPERS
		=========================================================
	*/

	stock CefUpdateSpeedometer(playerid, speed, fuel, Float:engine_health, bool:seatbelt, bool:lights)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(
			JSON_Int(speed),
			JSON_Int(fuel),
			JSON_Float(engine_health),
			JSON_Bool(seatbelt),
			JSON_Bool(lights)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_speedometer_update", data_str);
		return true;
	}

	stock CefUpdatePlayerHUD(playerid, money, bank, level, exp, const location[] = "")
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(
			JSON_Int(money),
			JSON_Int(bank),
			JSON_Int(level),
			JSON_Int(exp),
			JSON_String(location)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_hud_update", data_str);
		return true;
	}

	/*
		=========================================================
		6. PROGRESS BAR HELPERS
		=========================================================
	*/

	stock CefShowProgressBar(playerid, const title[], duration_ms)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		new Node:data_node = JSON_Array(
			JSON_String(title),
			JSON_Int(duration_ms)
		);

		new data_str[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(data_node, data_str);

		CefSendEvent(playerid, "cef_ui_progressbar_show", data_str);
		return true;
	}

	stock CefHideProgressBar(playerid)
	{
		if (!CefIsPlayerHasLibrary(playerid))
			return false;

		CefSendEvent(playerid, "cef_ui_progressbar_hide", "[]");
		return true;
	}

	/*
		=========================================================
		7. INTERNAL ROUTING CALLBACKS
		=========================================================
	*/

	forward _CefOnDialogResponseInternal(playerid, event_data[]);
	public _CefOnDialogResponseInternal(playerid, event_data[])
	{
		CefChangeBrowserFocus(playerid, false);

		new Node:event_node, Node:buffer_node;
		JSON_Parse(event_data, event_node);

		new dialogid, response;
		new inputtext[128];
		inputtext[0] = EOS;

		JSON_ArrayObject(event_node, 0, buffer_node);
		JSON_GetNodeInt(buffer_node, dialogid);

		JSON_ArrayObject(event_node, 1, buffer_node);
		JSON_GetNodeInt(buffer_node, response);

		if (JSON_ArrayLength(event_node) > 2)
		{
			JSON_ArrayObject(event_node, 2, buffer_node);
			JSON_GetNodeString(buffer_node, inputtext);
		}

		if (funcidx("OnCefDialogResponse") != -1)
		{
			CallLocalFunction("OnCefDialogResponse", "ddis", playerid, dialogid, response, inputtext);
		}

		return 1;
	}

	forward _CefOnListDialogResponseInternal(playerid, event_data[]);
	public _CefOnListDialogResponseInternal(playerid, event_data[])
	{
		CefChangeBrowserFocus(playerid, false);

		new Node:event_node, Node:buffer_node;
		JSON_Parse(event_data, event_node);

		new dialogid, response, item_index;
		new item_value[128];
		item_value[0] = EOS;

		JSON_ArrayObject(event_node, 0, buffer_node);
		JSON_GetNodeInt(buffer_node, dialogid);

		JSON_ArrayObject(event_node, 1, buffer_node);
		JSON_GetNodeInt(buffer_node, response);

		JSON_ArrayObject(event_node, 2, buffer_node);
		JSON_GetNodeInt(buffer_node, item_index);

		if (JSON_ArrayLength(event_node) > 3)
		{
			JSON_ArrayObject(event_node, 3, buffer_node);
			JSON_GetNodeString(buffer_node, item_value);
		}

		if (funcidx("OnCefListDialogResponse") != -1)
		{
			CallLocalFunction("OnCefListDialogResponse", "dddis", playerid, dialogid, response, item_index, item_value);
		}

		return 1;
	}

	forward _CefOnInventoryUseItemInternal(playerid, event_data[]);
	public _CefOnInventoryUseItemInternal(playerid, event_data[])
	{
		new Node:event_node, Node:buffer_node;
		JSON_Parse(event_data, event_node);

		new slot_id;
		new item_name[64];
		item_name[0] = EOS;

		JSON_ArrayObject(event_node, 0, buffer_node);
		JSON_GetNodeInt(buffer_node, slot_id);

		JSON_ArrayObject(event_node, 1, buffer_node);
		JSON_GetNodeString(buffer_node, item_name);

		if (funcidx("OnCefInventoryUseItem") != -1)
		{
			CallLocalFunction("OnCefInventoryUseItem", "dis", playerid, slot_id, item_name);
		}

		return 1;
	}

	public OnGameModeInit()
	{
		CefRegisterEventCallback("cef_ui_dialog_response", "_CefOnDialogResponseInternal");
		CefRegisterEventCallback("cef_ui_list_dialog_response", "_CefOnListDialogResponseInternal");
		CefRegisterEventCallback("cef_ui_inventory_use", "_CefOnInventoryUseItemInternal");

		#if defined CefUI_OnGameModeInit
			CefUI_OnGameModeInit();
		#endif

		return 1;
	}
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit CefUI_OnGameModeInit
	#if defined CefUI_OnGameModeInit
		forward CefUI_OnGameModeInit();
	#endif

#endif
