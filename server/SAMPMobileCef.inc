/*
	EN: Server include of CEF library for SA:MP Mobile (Pro Edition).
	UK: Серверний заголовний файл бібліотеки CEF для SA:MP Mobile.

	Copyright © 2024 Denis Akazuki <https://github.com/denis-akazuki>

	EN: Out of respect, please do not delete this comment if you use code from this project.
	UK: З поваги, будь ласка, не видаляйте цей коментар, якщо використовуєте код із цього проєкту.
*/

#if !defined SAMPMOBILECEF_INC_
	#define SAMPMOBILECEF_INC_

	#if !defined PAWNRAKNET_INC_
		#error "[CEF] Pawn.RakNet (https://github.com/katursis/Pawn.RakNet) is not included in the gamemode."
	#endif
	
	#if !defined _json_included
		#error "[CEF] JSON (https://github.com/Southclaws/pawn-json) is not included in the gamemode."
	#endif
	
	#if !defined SetGVarString
		#error "[CEF] gvar (https://github.com/samp-incognito/samp-gvar-plugin) is not included in the gamemode."
	#endif

	#define CEF_PREFIX "[CEF] "
	#define CEF_INC_VERSION "1.4.0-Upgraded"

	#if !defined CEF_MAX_EVENT_NAME_LENGTH
		#define CEF_MAX_EVENT_NAME_LENGTH 48
	#endif
	#if !defined CEF_MAX_EVENT_CALLBACK_LENGTH
		#define CEF_MAX_EVENT_CALLBACK_LENGTH 64
	#endif
	#if !defined CEF_MAX_EVENT_DATA_LENGTH
		#define CEF_MAX_EVENT_DATA_LENGTH 2048
	#endif

	new cef_packet_id = -1;
	new cef_initialized_players[MAX_PLAYERS];
	new bool:cef_browser_shown[MAX_PLAYERS];
	new cef_browser_url[MAX_PLAYERS][128];
	new Float:cef_browser_zoom[MAX_PLAYERS];
	new cef_rate_limit_max = 20; // Default max events per second per player
	new cef_event_count[MAX_PLAYERS];
	new cef_event_timestamp[MAX_PLAYERS];

	enum
	{
		RPC_InitBrowser = 1,
		RPC_DestroyBrowser,
		RPC_ShowBrowser,
		RPC_HideBrowser,
		RPC_SetBrowserUrl,
		RPC_ChangeBrowserFocus,
		RPC_ServerEvent,

		RPC_LibraryInit,
		RPC_BrowserInit,
		RPC_ClientEvent,
	};

	stock IsPacketIdSet()
	{
		if (cef_packet_id == -1)
		{
			printf("%s Unable to continue work of CEF include. Cef package id was not specified", CEF_PREFIX);
			return false;
		}
		
		return true;
	}

	stock CefSetPacketId(packet_id)
	{
		cef_packet_id = packet_id;
		
		printf("%s Include version: %s", CEF_PREFIX, CEF_INC_VERSION);
		printf("%s New cef packet id: %d", CEF_PREFIX, cef_packet_id);
	}

	stock CefSetEventRateLimit(max_events_per_sec)
	{
		cef_rate_limit_max = max_events_per_sec;
		printf("%s Event rate limit set to %d events/sec", CEF_PREFIX, cef_rate_limit_max);
	}

	stock CefInitBrowser(playerid, const url[])
	{
		if (!IsPacketIdSet())
			return;
		
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			format(cef_browser_url[playerid], sizeof(cef_browser_url[]), "%s", url);
			cef_browser_zoom[playerid] = 1.0;
		}
		
		new BitStream:bs = BS_New();
		
		BS_WriteValue(
			bs,
			PR_UINT16, cef_packet_id,
			PR_UINT32, _:RPC_InitBrowser,
			PR_UINT16, strlen(url),
			PR_STRING, url
		);
		PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE);
		
		BS_Delete(bs);
	}

	stock CefDestroyBrowser(playerid)
	{
		if (!IsPacketIdSet())
			return;
		
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			cef_browser_shown[playerid] = false;
			cef_browser_url[playerid][0] = EOS;
			cef_browser_zoom[playerid] = 1.0;
		}

		new BitStream:bs = BS_New();
		
		BS_WriteValue(
			bs,
			PR_UINT16, cef_packet_id,
			PR_UINT32, _:RPC_DestroyBrowser
		);
		PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE);
		
		BS_Delete(bs);
	}

	stock CefShowBrowser(playerid)
	{
		if (!IsPacketIdSet())
			return;
		
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			cef_browser_shown[playerid] = true;
		}

		new BitStream:bs = BS_New();
		
		BS_WriteValue(
			bs,
			PR_UINT16, cef_packet_id,
			PR_UINT32, _:RPC_ShowBrowser
		);
		PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE);
		
		BS_Delete(bs);
	}

	stock CefHideBrowser(playerid)
	{
		if (!IsPacketIdSet())
			return;
		
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			cef_browser_shown[playerid] = false;
		}

		new BitStream:bs = BS_New();
		
		BS_WriteValue(
			bs,
			PR_UINT16, cef_packet_id,
			PR_UINT32, _:RPC_HideBrowser
		);
		PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE);
		
		BS_Delete(bs);
	}

	stock CefToggleBrowser(playerid, bool:show)
	{
		if (show)
			CefShowBrowser(playerid);
		else
			CefHideBrowser(playerid);
	}

	stock bool:CefIsBrowserShown(playerid)
	{
		if (playerid < 0 || playerid >= MAX_PLAYERS)
			return false;

		return cef_browser_shown[playerid];
	}

	stock CefGetBrowserUrl(playerid, dest[], maxlen = sizeof(dest))
	{
		if (playerid < 0 || playerid >= MAX_PLAYERS)
			return 0;

		return format(dest, maxlen, "%s", cef_browser_url[playerid]);
	}

	stock CefReloadBrowser(playerid)
	{
		if (playerid < 0 || playerid >= MAX_PLAYERS)
			return;

		if (cef_browser_url[playerid][0] != EOS)
		{
			CefSetBrowserUrl(playerid, cef_browser_url[playerid]);
		}
	}

	stock CefResetBrowser(playerid)
	{
		if (playerid < 0 || playerid >= MAX_PLAYERS)
			return;

		CefHideBrowser(playerid);
		CefChangeBrowserFocus(playerid, false);
		cef_browser_url[playerid][0] = EOS;
		cef_browser_zoom[playerid] = 1.0;
	}

	stock CefSetBrowserZoom(playerid, Float:scale)
	{
		if (playerid < 0 || playerid >= MAX_PLAYERS)
			return;

		cef_browser_zoom[playerid] = scale;
		new js_code[64];
		format(js_code, sizeof(js_code), "document.body.style.zoom = '%.2f';", scale);
		CefExecuteJavaScript(playerid, js_code);
	}

	stock Float:CefGetBrowserZoom(playerid)
	{
		if (playerid < 0 || playerid >= MAX_PLAYERS)
			return 1.0;

		return cef_browser_zoom[playerid];
	}

	stock CefSetBrowserUrl(playerid, const url[])
	{
		if (!IsPacketIdSet())
			return;
		
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			format(cef_browser_url[playerid], sizeof(cef_browser_url[]), "%s", url);
		}

		new BitStream:bs = BS_New();
		
		BS_WriteValue(
			bs,
			PR_UINT16, cef_packet_id,
			PR_UINT32, _:RPC_SetBrowserUrl,
			PR_UINT16, strlen(url),
			PR_STRING, url
		);
		PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE);
		
		BS_Delete(bs);
	}

	stock CefChangeBrowserFocus(playerid, bool:is_focused)
	{
		if (!IsPacketIdSet())
			return;
		
		new BitStream:bs = BS_New();
		
		BS_WriteValue(
			bs,
			PR_UINT16, cef_packet_id,
			PR_UINT32, _:RPC_ChangeBrowserFocus,
			PR_BOOL, is_focused
		);
		PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE);
		
		BS_Delete(bs);
	}

	stock CefSetBrowserFocusAll(bool:is_focused)
	{
		if (!IsPacketIdSet())
			return;

		#if defined foreach
			foreach (new i : Player)
			{
				if (CefIsPlayerHasLibrary(i))
				{
					CefChangeBrowserFocus(i, is_focused);
				}
			}
		#else
			for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
			{
				if (IsPlayerConnected(i) && CefIsPlayerHasLibrary(i))
				{
					CefChangeBrowserFocus(i, is_focused);
				}
			}
		#endif
	}

	stock CefSendEvent(playerid, const event_name[], const event_data[])
	{
		if (!IsPacketIdSet())
			return;
		
		new BitStream:bs = BS_New();
		
		BS_WriteValue(
			bs,
			PR_UINT16, cef_packet_id,
			PR_UINT32, _:RPC_ServerEvent,
			PR_UINT16, strlen(event_name),
			PR_STRING, event_name,
			PR_UINT16, strlen(event_data),
			PR_STRING, event_data
		);
		PR_SendPacket(bs, playerid, PR_HIGH_PRIORITY, PR_RELIABLE);
		
		BS_Delete(bs);
	}

	stock CefExecuteJavaScript(playerid, const code[])
	{
		if (!IsPacketIdSet() || !CefIsPlayerHasLibrary(playerid))
			return;

		new Node:eval_node = JSON_Array(JSON_String(code));
		new eval_data[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(eval_node, eval_data);

		CefSendEvent(playerid, "_cef_eval", eval_data);
	}

	stock CefExecuteJavaScriptToAll(const code[])
	{
		if (!IsPacketIdSet())
			return;

		new Node:eval_node = JSON_Array(JSON_String(code));
		new eval_data[CEF_MAX_EVENT_DATA_LENGTH];
		JSON_Stringify(eval_node, eval_data);

		CefSendEventToAll("_cef_eval", eval_data);
	}

	stock CefSendEventToAll(const event_name[], const event_data[])
	{
		if (!IsPacketIdSet())
			return;

		#if defined foreach
			foreach (new i : Player)
			{
				if (CefIsPlayerHasLibrary(i))
				{
					CefSendEvent(i, event_name, event_data);
				}
			}
		#else
			for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
			{
				if (IsPlayerConnected(i) && CefIsPlayerHasLibrary(i))
				{
					CefSendEvent(i, event_name, event_data);
				}
			}
		#endif
	}

	stock CefGetInitializedPlayerCount()
	{
		new count = 0;
		#if defined foreach
			foreach (new i : Player)
			{
				if (CefIsPlayerHasLibrary(i)) count++;
			}
		#else
			for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
			{
				if (IsPlayerConnected(i) && CefIsPlayerHasLibrary(i)) count++;
			}
		#endif
		return count;
	}

	stock CefRegisterEventCallback(const event_name[], const callback[])
	{
		if (!IsPacketIdSet())
			return;
		
		if (strlen(callback) > CEF_MAX_EVENT_CALLBACK_LENGTH) {
			printf("%s Callback function name can't be higher than %d (event: %s)", CEF_PREFIX, CEF_MAX_EVENT_CALLBACK_LENGTH, event_name);
			return;
		}
		
		if (GetGVarType(event_name) != GLOBAL_VARTYPE_NONE)
		{
			DeleteGVar(event_name);
		}
		
		SetGVarString(event_name, callback);
	}

	stock bool:CefUnregisterEventCallback(const event_name[])
	{
		if (GetGVarType(event_name) != GLOBAL_VARTYPE_NONE)
		{
			DeleteGVar(event_name);
			return true;
		}
		return false;
	}

	stock bool:CefHasEventCallback(const event_name[])
	{
		return (GetGVarType(event_name) != GLOBAL_VARTYPE_NONE);
	}
	
	stock bool:CefIsPlayerHasLibrary(playerid)
	{
		if (!IsPacketIdSet() || playerid < 0 || playerid >= MAX_PLAYERS)
			return false;
		
		return bool:cef_initialized_players[playerid];
	}

	public OnIncomingPacket(playerid, packetid, BitStream:bs)
	{
		if (IsPacketIdSet() && packetid == cef_packet_id)
		{
			new rpc_id;

			BS_IgnoreBits(bs, 16);
			BS_ReadValue(bs, PR_UINT32, rpc_id);
				
			switch (rpc_id)
			{
				case RPC_LibraryInit:
				{
					cef_initialized_players[playerid] = 1;
				}
				case RPC_BrowserInit:
				{
					new is_init, error_code;
					
					BS_ReadValue(bs, PR_BOOL, is_init);
					BS_ReadValue(bs, PR_INT16, error_code);
					
					CallLocalFunction("OnCefBrowserInit", "ddd", playerid, is_init, error_code);
				}
				case RPC_ClientEvent:
				{
					// Anti-Spam Rate Limiter Check
					new current_time = gettime();
					if (current_time != cef_event_timestamp[playerid])
					{
						cef_event_timestamp[playerid] = current_time;
						cef_event_count[playerid] = 0;
					}

					cef_event_count[playerid]++;
					if (cef_event_count[playerid] > cef_rate_limit_max)
					{
						printf("%s WARNING: Player %d exceeded event rate limit (%d/sec). Event ignored.", CEF_PREFIX, playerid, cef_rate_limit_max);
						return 0; // Block event execution
					}

					new event_name_length;
					new event_name[CEF_MAX_EVENT_NAME_LENGTH];
					event_name[0] = EOS;
					
					BS_ReadValue(bs, PR_UINT16, event_name_length);
					BS_ReadValue(bs, PR_STRING, event_name, event_name_length);
					
					new event_data_length;
					new event_data[CEF_MAX_EVENT_DATA_LENGTH];
					event_data[0] = EOS;
					
					BS_ReadValue(bs, PR_UINT16, event_data_length);
					BS_ReadValue(bs, PR_STRING, event_data, event_data_length);
					
					if (GetGVarType(event_name) != GLOBAL_VARTYPE_NONE)
					{					
						new event_callback[CEF_MAX_EVENT_CALLBACK_LENGTH];
						event_callback[0] = EOS;
						GetGVarString(event_name, event_callback, sizeof event_callback);

						CallLocalFunction(event_callback, "is", playerid, event_data);
					}
				}
			}
		}
		
		#if defined Cef_OnIncomingPacket
			Cef_OnIncomingPacket(playerid, packetid, bs);
		#endif
		
		return 1;
	}
	#if defined _ALS_OnIncomingPacket
		#undef OnIncomingPacket
	#else
		#define _ALS_OnIncomingPacket
	#endif
	#define OnIncomingPacket Cef_OnIncomingPacket
	#if defined Cef_OnIncomingPacket
		forward Cef_OnIncomingPacket(playerid, packetid, BitStream:bs);
	#endif
	
	public OnPlayerConnect(playerid)
	{
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			cef_initialized_players[playerid] = 0;
			cef_browser_shown[playerid] = false;
			cef_browser_url[playerid][0] = EOS;
			cef_browser_zoom[playerid] = 1.0;
			cef_event_count[playerid] = 0;
			cef_event_timestamp[playerid] = 0;
		}
		
		#if defined Cef_OnPlayerConnect
			Cef_OnPlayerConnect(playerid);
		#endif
		
		return 1;
	}
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect Cef_OnPlayerConnect
	#if defined Cef_OnPlayerConnect
		forward Cef_OnPlayerConnect(playerid);
	#endif
	
	public OnPlayerDisconnect(playerid, reason)
	{
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			cef_initialized_players[playerid] = -1;
			cef_browser_shown[playerid] = false;
			cef_browser_url[playerid][0] = EOS;
			cef_browser_zoom[playerid] = 1.0;
			cef_event_count[playerid] = 0;
			cef_event_timestamp[playerid] = 0;
		}
		
		#if defined Cef_OnPlayerDisconnect
			Cef_OnPlayerDisconnect(playerid, reason);
		#endif
		
		return 1;
	}
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect Cef_OnPlayerDisconnect
	#if defined Cef_OnPlayerDisconnect
		forward Cef_OnPlayerDisconnect(playerid, reason);
	#endif
	
	forward OnCefBrowserInit(playerid, is_init, error_code);

#endif