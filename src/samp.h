#ifndef C89B6DEF_86C1_4DA0_AC83_4D522C40B023
#define C89B6DEF_86C1_4DA0_AC83_4D522C40B023

#include <d3dx9.h>
#include <stdint.h>
#include <string>
#include <sampapi/CNetGame.h>


namespace samp = sampapi::v037r1;

constexpr int GetCurrentNetgameVersion();
std::string GetCurrentVersionString();

enum eBone {
	BONE_PELVIS1 = 1,
	BONE_PELVIS,
	BONE_SPINE1,
	BONE_UPPERTORSO,
	BONE_NECK,
	BONE_HEAD2,
	BONE_HEAD1,
	BONE_HEAD,
	BONE_RIGHTUPPERTORSO = 21,
	BONE_RIGHTSHOULDER,
	BONE_RIGHTELBOW,
	BONE_RIGHTWRIST,
	BONE_RIGHTHAND,
	BONE_RIGHTTHUMB,
	BONE_LEFTUPPERTORSO = 31,
	BONE_LEFTSHOULDER,
	BONE_LEFTELBOW,
	BONE_LEFTWRIST,
	BONE_LEFTHAND,
	BONE_LEFTTHUMB,
	BONE_LEFTHIP = 41,
	BONE_LEFTKNEE,
	BONE_LEFTANKLE,
	BONE_LEFTFOOT,
	BONE_RIGHTHIP = 51,
	BONE_RIGHTKNEE,
	BONE_RIGHTANKLE,
	BONE_RIGHTFOOT
};

enum DialogStyle
{
	DIALOG_STYLE_MSGBOX,
	DIALOG_STYLE_INPUT,
	DIALOG_STYLE_LIST,
	DIALOG_STYLE_PASSWORD,
};

enum Limits
{
	SAMP_MAX_ACTORS = 1000,
	SAMP_MAX_PLAYERS = 1004,
	SAMP_MAX_VEHICLES = 2000,
	SAMP_MAX_PICKUPS = 4096,
	SAMP_MAX_OBJECTS = 1000,
	SAMP_MAX_GANGZONES = 1024,
	SAMP_MAX_3DTEXTS = 2048,
	SAMP_MAX_TEXTDRAWS = 2048,
	SAMP_MAX_PLAYERTEXTDRAWS = 256,
	SAMP_MAX_CLIENTCMDS = 144,
	SAMP_MAX_MENUS = 128,
	SAMP_MAX_PLAYER_NAME = 24,
	SAMP_ALLOWED_PLAYER_NAME_LENGTH = 20,
};

enum ChatMessageType
{
	CHAT_TYPE_NONE = 0,
	CHAT_TYPE_CHAT = 2,
	CHAT_TYPE_INFO = 4,
	CHAT_TYPE_DEBUG = 8
};

enum ChatDisplayMode
{
	CHAT_WINDOW_MODE_OFF = 0,
	CHAT_WINDOW_MODE_LIGHT = 1,
	CHAT_WINDOW_MODE_FULL = 2
};

enum Gamestate
{
	GAMESTATE_WAIT_CONNECT = 9,
	GAMESTATE_CONNECTING = 13,
	GAMESTATE_AWAIT_JOIN = 15,
	GAMESTATE_CONNECTED = 14,
	GAMESTATE_RESTARTING = 18
};

enum PlayerState
{
	PLAYER_STATE_NONE = 0,
	PLAYER_STATE_ONFOOT = 17,
	PLAYER_STATE_PASSENGER,
	PLAYER_STATE_DRIVER,
	PLAYER_STATE_WASTED = 32,
	PLAYER_STATE_SPAWNED
};

enum MarkersMode
{
	PLAYER_MARKERS_MODE_OFF,
	PLAYER_MARKERS_MODE_GLOBAL,
	PLAYER_MARKERS_MODE_STREAMED,
};

enum SpecialAction
{
	SPECIAL_ACTION_NONE,
	SPECIAL_ACTION_DUCK,
	SPECIAL_ACTION_USEJETPACK,
	SPECIAL_ACTION_ENTER_VEHICLE,
	SPECIAL_ACTION_EXIT_VEHICLE,
	SPECIAL_ACTION_DANCE1,
	SPECIAL_ACTION_DANCE2,
	SPECIAL_ACTION_DANCE3,
	SPECIAL_ACTION_DANCE4,
	SPECIAL_ACTION_HANDSUP,
	SPECIAL_ACTION_USECELLPHONE,
	SPECIAL_ACTION_SITTING,
	SPECIAL_ACTION_STOPUSECELLPHONE,
	SPECIAL_ACTION_DRINK_BEER = 20,
	SPECIAL_ACTION_SMOKE_CIGGY,
	SPECIAL_ACTION_DRINK_WINE,
	SPECIAL_ACTION_DRINK_SPRUNK,
	SPECIAL_ACTION_CUFFED,
	SPECIAL_ACTION_CARRY,
};


//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SAMP STRUCTURES ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
struct stSAMPPools
{
	struct stActorPool		*pActor;
	struct stObjectPool		*pObject;
	struct stGangzonePool	*pGangzone;
	struct stTextLabelPool	*pText3D;
	struct stTextdrawPool	*pTextdraw;
	void					*pPlayerLabels;
	struct stPlayerPool		*pPlayer;
	struct stVehiclePool	*pVehicle;
	struct stPickupPool		*pPickup;
};

struct stSAMP
{
	void					*pUnk0;
	struct stServerInfo			*pServerInfo;
	uint8_t					byteSpace[24];
	char					szIP[257];
	char					szHostname[259];
	bool					bNametagStatus; // changes by /nametagstatus
	uint32_t				ulPort;
	uint32_t				ulMapIcons[100];
	int						iLanMode;
	int						iGameState;
	uint32_t				ulConnectTick;
	struct stServerPresets	*pSettings;
	void					*pRakClientInterface;
	struct stSAMPPools		*pPools;
};

struct stServerInfo{
	uint32_t 			uiIP;
	uint16_t 			usPort;

	// ...

};

struct stServerPresets
{
	uint8_t byteCJWalk;
	int m_iDeathDropMoney;
	float	fWorldBoundaries[4];
	bool m_bAllowWeapons;
	float	fGravity;
	uint8_t byteDisableInteriorEnterExits;
	uint32_t ulVehicleFriendlyFire;
	bool m_byteHoldTime;
  	bool m_bInstagib;
  	bool m_bZoneNames;
  	bool m_byteFriendlyFire;
	int		iClassesAvailable;
	float	fNameTagsDistance;
	 bool m_bManualVehicleEngineAndLight;
	uint8_t byteWorldTime_Hour;
	uint8_t byteWorldTime_Minute;
	uint8_t byteWeather;
	uint8_t byteNoNametagsBehindWalls;
	int iPlayerMarkersMode;
	float	fGlobalChatRadiusLimit;
	uint8_t byteShowNameTags;
 	bool m_bLimitGlobalChatRadius;
};



struct stDialogInfo
{
	IDirect3DDevice9	*m_pD3DDevice;
	int	iTextPoxX;
	int	iTextPoxY;
	uint32_t	uiDialogSizeX;
	uint32_t	uiDialogSizeY;
	int	iBtnOffsetX;
	int	iBtnOffsetY;
	class _CDXUTDialog						*pDialog;
	class _CDXUTListBox						*pList;
	class _CDXUTIMEEditBox						*pEditBox;
	int	iIsActive;
	int	iType;
	uint32_t	DialogID;
	char		*pText;
	uint32_t	uiTextWidth;
	uint32_t	uiTextHeight;
	char		szCaption[65];
	int		bServerside;
};

struct stTextDrawTransmit
{
	union
	{
		BYTE byteFlags;
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1;
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float fLetterWidth;
	float fLetterHeight;
	DWORD dwLetterColor;
	float fBoxWidth;
	float fBoxHeight;
	DWORD dwBoxColor;
	BYTE byteShadow;
	BYTE byteOutline;
	DWORD dwBackgroundColor;
	BYTE byteStyle;
	BYTE byteUNK;
	float fX;
	float fY;
	uint16_t sModel;
	float fRot[3];
	float fZoom;
	WORD sColor[2];
};

struct stTextdraw
{
	char		szText[800 + 1];
	char		szString[1600 + 2];
	float		fLetterWidth;
	float		fLetterHeight;
	DWORD		dwLetterColor;
	uint8_t		byte_unk;	// always = 01 (?)
	BYTE		byteCenter;
	BYTE		byteBox;
	float		fBoxSizeX;
	float		fBoxSizeY;
	DWORD		dwBoxColor;
	BYTE		byteProportional;
	DWORD		dwShadowColor;
	BYTE		byteShadowSize;
	BYTE		byteOutline;
	BYTE		byteLeft;
	BYTE		byteRight;
	int			iStyle;		// font style/texture/model
	float		fX;
	float		fY;
	byte		unk[8];
	DWORD		dword99B;	// -1 by default
	DWORD		dword99F;	// -1 by default
	DWORD		index;		// -1 if bad
	BYTE		byte9A7;	// = 1; 0 by default
	uint16_t	sModel;
	float		fRot[3];
	float		fZoom;
	WORD		sColor[2];
	BYTE		f9BE;
	BYTE		byte9BF;
	BYTE		byte9C0;
	DWORD		dword9C1;
	DWORD		dword9C5;
	DWORD		dword9C9;
	DWORD		dword9CD;
	BYTE		byte9D1;
	DWORD		dword9D2;
};

struct stTextdrawPool
{
	int					iIsListed[SAMP_MAX_TEXTDRAWS];
	int					iPlayerTextDraw[SAMP_MAX_PLAYERTEXTDRAWS];
	struct stTextdraw	*textdraw[SAMP_MAX_TEXTDRAWS];
	struct stTextdraw	*playerTextdraw[SAMP_MAX_PLAYERTEXTDRAWS];
};

struct stPickup
{
	int		iModelID;
	int		iType;
	float	fPosition[3];
};

struct stPickupPool
{
	int				iPickupsCount;
	uint32_t		ul_GTA_PickupID[SAMP_MAX_PICKUPS];
	int				iPickupID[SAMP_MAX_PICKUPS];
	int				iTimePickup[SAMP_MAX_PICKUPS];
	uint8_t			unk[SAMP_MAX_PICKUPS * 3];
	struct stPickup pickup[SAMP_MAX_PICKUPS];
};

struct stPlayerPool
{
	uint32_t				ulMaxPlayerID;
	uint16_t				sLocalPlayerID;
	void					*pVTBL_txtHandler;
	std::string				strLocalPlayerName;
	struct stLocalPlayer	*pLocalPlayer;
	int						iLocalPlayerPing;
	int						iLocalPlayerScore;
	struct stRemotePlayer	*pRemotePlayer[SAMP_MAX_PLAYERS];
	int						iIsListed[SAMP_MAX_PLAYERS];
	DWORD						dwPlayerIP[SAMP_MAX_PLAYERS]; // always 0
};

struct stSAMPKeys
{
	uint8_t keys_primaryFire : 1;
	uint8_t keys_horn__crouch : 1;
	uint8_t keys_secondaryFire__shoot : 1;
	uint8_t keys_accel__zoomOut : 1;
	uint8_t keys_enterExitCar : 1;
	uint8_t keys_decel__jump : 1;			// on foot: jump or zoom in
	uint8_t keys_circleRight : 1;
	uint8_t keys_aim : 1;					// hydra auto aim or on foot aim
	uint8_t keys_circleLeft : 1;
	uint8_t keys_landingGear__lookback : 1;
	uint8_t keys_unknown__walkSlow : 1;
	uint8_t keys_specialCtrlUp : 1;
	uint8_t keys_specialCtrlDown : 1;
	uint8_t keys_specialCtrlLeft : 1;
	uint8_t keys_specialCtrlRight : 1;
	uint8_t keys__unused : 1;
};

struct stOnFootData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fPosition[3];
	float		fQuaternion[4];
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSpecialAction;
	float		fMoveSpeed[3];
	float		fSurfingOffsets[3];
	uint16_t	sSurfingVehicleID;
	short		sCurrentAnimationID;
	short		sAnimFlags;
};

struct stInCarData
{
	uint16_t	sVehicleID;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fQuaternion[4];
	float		fPosition[3];
	float		fMoveSpeed[3];
	float		fVehicleHealth;
	uint8_t		bytePlayerHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSiren;
	uint8_t		byteLandingGearState;
	uint16_t	sTrailerID;
	union
	{
		uint16_t	HydraThrustAngle[2];	//nearly same value
		float		fTrainSpeed;
	};
};

struct stAimData
{
	BYTE	byteCamMode;
	float	vecAimf1[3];
	float	vecAimPos[3];
	float	fAimZ;
	BYTE	byteCamExtZoom : 6;		// 0-63 normalized
	BYTE	byteWeaponState : 2;	// see eWeaponState
	BYTE	bUnk;
};

struct stTrailerData
{
	uint16_t	sTrailerID;
	float		fPosition[3];
	//float		fRoll[3];
	//float		fDirection[3];
	float		fQuaternion[4]; // not tested
	float		fSpeed[3];
	float		fUnk[2];
	uint32_t	pad;
};

struct stPassengerData
{
	uint16_t	sVehicleID;
	uint8_t		byteSeatID;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stDamageData
{
	uint16_t	sVehicleID_lastDamageProcessed;
	int			iBumperDamage;
	int			iDoorDamage;
	uint8_t		byteLightDamage;
	uint8_t		byteWheelDamage;
};

struct stSurfData
{
	int			iIsSurfing;
	float		fSurfPosition[3];
	int			iUnk0;
	uint16_t	sSurfingVehicleID;
	uint32_t	ulSurfTick;
	struct stSAMPVehicle *pSurfingVehicle;
	int			iUnk1;
	int			iSurfMode;	//0 = not surfing, 1 = moving (unstable surf), 2 = fixed on vehicle
};

struct stUnoccupiedData
{
	int16_t sVehicleID;
	uint8_t byteSeatID;
	float	fRoll[3];
	float	fDirection[3];
	float	fPosition[3];
	float	fMoveSpeed[3];
	float	fTurnSpeed[3];
	float	fHealth;
};

struct stBulletData
{
	uint8_t		byteType;
	uint16_t	sTargetID;
	float		fOrigin[3];
	float		fTarget[3];
	float		fCenter[3];
	uint8_t		byteWeaponID;
};

struct stSpectatorData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stStatsData
{
	int iMoney;
	int iAmmo;	// ?
};

struct stHeadSync
{
	float	fHeadSync[3];
	int		iHeadSyncUpdateTick;
	int		iHeadSyncLookTick;
};

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// SUPPORT VARIABLES //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
struct stTranslateGTASAMP_vehiclePool
{
	// the index is the position in the vehicle pool
	//
	// translation data
	int iSAMPID[SAMP_MAX_VEHICLES];
};

struct stTranslateGTASAMP_pedPool
{
	// the index is the position in the ped pool
	//
	// translation data
	int iSAMPID[SAMP_MAX_PLAYERS];
};


enum RPCEnumeration
{
	RPC_ClickPlayer					= 23,
	RPC_ClientJoin					= 25,
	RPC_EnterVehicle				= 26,
	RPC_EnterEditObject				= 27,
	RPC_ScriptCash					= 31,
	RPC_ServerCommand				= 50,
	RPC_Spawn						= 52,
	RPC_Death						= 53,
	RPC_NPCJoin						= 54,
	RPC_DialogResponse				= 62,
	RPC_ClickTextDraw				= 83,
	RPC_SCMEvent					= 96,
	RPC_Chat						= 101,
	RPC_SrvNetStats					= 102,
	RPC_ClientCheck					= 103,
	RPC_DamageVehicle				= 106,
	RPC_GiveTakeDamage				= 115,
	RPC_EditAttachedObject			= 116,
	RPC_EditObject					= 117,
	RPC_SetInteriorId				= 118,
	RPC_MapMarker					= 119,
	RPC_RequestClass				= 128,
	RPC_RequestSpawn				= 129,
	RPC_PickedUpPickup				= 131,
	RPC_MenuSelect					= 132,
	RPC_VehicleDestroyed			= 136,
	RPC_MenuQuit					= 140,
	RPC_ExitVehicle					= 154,
	RPC_UpdateScoresPingsIPs		= 155,

	// server RPC's
	RPC_SetPlayerName				= 11,
	RPC_SetPlayerPos				= 12,
	RPC_SetPlayerPosFindZ			= 13,
	RPC_SetPlayerHealth				= 14,
	RPC_TogglePlayerControllable	= 15,
	RPC_PlaySound					= 16,
	RPC_SetPlayerWorldBounds		= 17,
	RPC_GivePlayerMoney				= 18,
	RPC_SetPlayerFacingAngle		= 19,
	RPC_ResetPlayerMoney			= 20,
	RPC_ResetPlayerWeapons			= 21,
	RPC_GivePlayerWeapon			= 22,
	RPC_SetVehicleParamsEx			= 24,
	RPC_CancelEdit					= 28,
	RPC_SetPlayerTime				= 29,
	RPC_ToggleClock					= 30,
	RPC_WorldPlayerAdd				= 32,
	RPC_SetPlayerShopName			= 33,
	RPC_SetPlayerSkillLevel			= 34,
	RPC_SetPlayerDrunkLevel			= 35,
	RPC_Create3DTextLabel			= 36,
	RPC_DisableCheckpoint			= 37,
	RPC_SetRaceCheckpoint			= 38,
	RPC_DisableRaceCheckpoint		= 39,
	RPC_GameModeRestart				= 40,
	RPC_PlayAudioStream				= 41,
	RPC_StopAudioStream				= 42,
	RPC_RemoveBuildingForPlayer		= 43,
	RPC_CreateObject				= 44,
	RPC_SetObjectPos				= 45,
	RPC_SetObjectRot				= 46,
	RPC_DestroyObject				= 47,
	RPC_DeathMessage				= 55,
	RPC_SetPlayerMapIcon			= 56,
	RPC_RemoveVehicleComponent		= 57,
	RPC_Update3DTextLabel			= 58,
	RPC_ChatBubble					= 59,
	RPC_UpdateSystemTime			= 60,
	RPC_ShowDialog					= 61,
	RPC_DestroyPickup				= 63,
	RPC_WeaponPickupDestroy			= 64,
	RPC_LinkVehicleToInterior		= 65,
	RPC_SetPlayerArmour				= 66,
	RPC_SetPlayerArmedWeapon		= 67,
	RPC_SetSpawnInfo				= 68,
	RPC_SetPlayerTeam				= 69,
	RPC_PutPlayerInVehicle			= 70,
	RPC_RemovePlayerFromVehicle		= 71,
	RPC_SetPlayerColor				= 72,
	RPC_DisplayGameText				= 73,
	RPC_ForceClassSelection			= 74,
	RPC_AttachObjectToPlayer		= 75,
	RPC_InitMenu					= 76,
	RPC_ShowMenu					= 77,
	RPC_HideMenu					= 78,
	RPC_CreateExplosion				= 79,
	RPC_ShowPlayerNameTagForPlayer	= 80,
	RPC_AttachCameraToObject		= 81,
	RPC_InterpolateCamera			= 82,
	RPC_SetObjectMaterial			= 84,
	RPC_GangZoneStopFlash			= 85,
	RPC_ApplyAnimation				= 86,
	RPC_ClearAnimations				= 87,
	RPC_SetPlayerSpecialAction		= 88,
	RPC_SetPlayerFightingStyle		= 89,
	RPC_SetPlayerVelocity			= 90,
	RPC_SetVehicleVelocity			= 91,
	RPC_SetPlayerDrunkVisuals		= 92,
	RPC_ClientMessage				= 93,
	RPC_SetWorldTime				= 94,
	RPC_CreatePickup				= 95,
	RPC_SetVehicleTireStatus		= 98,
	RPC_MoveObject					= 99,
	RPC_EnableStuntBonusForPlayer	= 104,
	RPC_TextDrawSetString			= 105,
	RPC_SetCheckpoint				= 107,
	RPC_GangZoneCreate				= 108,
	RPC_PlayCrimeReport				= 112,
	RPC_SetPlayerAttachedObject		= 113,
	RPC_GangZoneDestroy				= 120,
	RPC_GangZoneFlash				= 121,
	RPC_StopObject					= 122,
	RPC_SetNumberPlate				= 123,
	RPC_TogglePlayerSpectating		= 124,
	RPC_PlayerSpectatePlayer		= 126,
	RPC_PlayerSpectateVehicle		= 127,
	RPC_SetPlayerWantedLevel		= 133,
	RPC_ShowTextDraw				= 134,
	RPC_TextDrawHideForPlayer		= 135,
	RPC_ServerJoin					= 137,
	RPC_ServerQuit					= 138,
	RPC_InitGame					= 139,
	RPC_RemovePlayerMapIcon			= 144,
	RPC_SetPlayerAmmo				= 145,
	RPC_SetPlayerGravity			= 146,
	RPC_SetVehicleHealth			= 147,
	RPC_AttachTrailerToVehicle		= 148,
	RPC_DetachTrailerFromVehicle	= 149,
	RPC_SetPlayerDrunkHandling		= 150,
	RPC_DestroyPickups				= 151,
	RPC_SetWeather					= 152,
	RPC_SetPlayerSkin				= 153,
	RPC_SetPlayerInterior			= 156,
	RPC_SetPlayerCameraPos			= 157,
	RPC_SetPlayerCameraLookAt		= 158,
	RPC_SetVehiclePos				= 159,
	RPC_SetVehicleZAngle			= 160,
	RPC_SetVehicleParamsForPlayer	= 161,
	RPC_SetCameraBehindPlayer		= 162,
	RPC_WorldPlayerRemove			= 163,
	RPC_WorldVehicleAdd				= 164,
	RPC_WorldVehicleRemove			= 165,
	RPC_WorldPlayerDeath 			= 166,
};

enum PacketEnumeration
{
	ID_INTERNAL_PING = 6,
	ID_PING,
	ID_PING_OPEN_CONNECTIONS,
	ID_CONNECTED_PONG,
	ID_REQUEST_STATIC_DATA,
	ID_CONNECTION_REQUEST,
	ID_AUTH_KEY,
	ID_BROADCAST_PINGS = 15,
	ID_SECURED_CONNECTION_RESPONSE,
	ID_SECURED_CONNECTION_CONFIRMATION,
	ID_RPC_MAPPING,
	ID_SET_RANDOM_NUMBER_SEED = 21,
	ID_RPC,
	ID_RPC_REPLY,
	ID_DETECT_LOST_CONNECTIONS,
	ID_OPEN_CONNECTION_REQUEST,
	ID_OPEN_CONNECTION_REPLY,
	ID_RSA_PUBLIC_KEY_MISMATCH = 28,
	ID_CONNECTION_ATTEMPT_FAILED,
	ID_NEW_INCOMING_CONNECTION,
	ID_NO_FREE_INCOMING_CONNECTIONS,
	ID_DISCONNECTION_NOTIFICATION,
	ID_CONNECTION_LOST,
	ID_CONNECTION_REQUEST_ACCEPTED,
	ID_INITIALIZE_ENCRYPTION,
	ID_CONNECTION_BANNED,
	ID_INVALID_PASSWORD,
	ID_MODIFIED_PACKET,
	ID_PONG,
	ID_TIMESTAMP,
	ID_RECEIVED_STATIC_DATA,
	ID_REMOTE_DISCONNECTION_NOTIFICATION,
	ID_REMOTE_CONNECTION_LOST,
	ID_REMOTE_NEW_INCOMING_CONNECTION,
	ID_REMOTE_EXISTING_CONNECTION,
	ID_REMOTE_STATIC_DATA,
	ID_ADVERTISE_SYSTEM = 56,

	ID_VEHICLE_SYNC = 200,
	ID_AIM_SYNC = 203,
	ID_BULLET_SYNC = 206,
	ID_PLAYER_SYNC = 207,
	ID_MARKERS_SYNC,
	ID_UNOCCUPIED_SYNC,
	ID_TRAILER_SYNC,
	ID_PASSENGER_SYNC,
	ID_SPECTATOR_SYNC,
	ID_RCON_COMMAND,
	ID_RCON_RESPONCE,
	ID_WEAPONS_UPDATE,
	ID_STATS_UPDATE,
};



#endif /* C89B6DEF_86C1_4DA0_AC83_4D522C40B023 */
