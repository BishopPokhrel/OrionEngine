#pragma once

#ifdef OE_SCRIPTING_BUILD
	#define OE_API __declspec(dllexport)
#else
	#define OE_API __declspec(dllimport)
#endif

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// ALIASES
	typedef const char* OEString;
	typedef unsigned long long OEUint64_t;
	typedef unsigned long OEUint32_t;
	typedef OEUint64_t OEAssetIdentifierHandle;

	// Basic mathematical data structures
	typedef struct OEVector
	{
		float X;
	} OEVector;

	typedef struct OEVector2
	{
		float X;
		float Y;
	} OEVector2;

	typedef struct OEVector3
	{
		float X;
		float Y;
		float Z;
	} OEVector3;

	typedef struct OEVector4
	{
		float X;
		float Y;
		float Z;
		float A;
	} OEVector4;

	// ---------------------------------
	// SECTION LOGGING
	// ----------------------------------

	typedef enum OELogLevel
	{
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL
	} OELogLevel;

	OE_API void LogMessage(OELogLevel level, const char* message);

	// SECTION CORE
	OE_API OEString GetEngineVersionNumber();
	OE_API OEUint32_t GetABIVersion(void);

	// ---------------------------
	// SECTION ENTITY
	// --------------------------

	typedef struct OECameraValues
	{
		float Left;
		float Right;
		float Top;
		float Bottom;
	} OECameraValues;

	typedef struct OETransformComponent
	{
		OEVector3 Position;
		OEVector3 Rotation;
		OEVector3 Scale;
	} OETransformComponent;

	typedef struct OEPhysicsComponent
	{
		float Mass;
		OEVector3 Velocity;
		OEVector3 Acceleration;
		bool IsStatic;
		float LinearDrag;
		float Radius;
	} OEPhysicsComponent;

	typedef struct OERenderableComponent
	{
		bool Visible;

		OEAssetIdentifierHandle MeshIdentifierHandle;
		OEAssetIdentifierHandle MaterialIdentifierHandle;

	} OERenderableComponent;

	typedef struct OECameraComponent
	{
		OECameraValues Camera;

		bool Primary;
		float NearClip;
		float FarClip;
	} OECameraComponent;

	typedef OEUint64_t OEGameEntityID;

	OE_API OEGameEntityID CreateGameEntity(OEString name);
	OE_API void DeleteGameEntity(OEGameEntityID id);
	OE_API OEGameEntityID CopyGameEntity(OEGameEntityID originalEntityID);

	OE_API OEGameEntityID GetEntityIDByName(OEString name);
	OE_API bool GetEntityNameByID(OEGameEntityID id, char* buffer, OEUint32_t bufferSize);

	OE_API void SetEntityName(OEGameEntityID id, OEString newEntityName);
	OE_API bool IsGameEntityValid(OEGameEntityID id);

	// Components
	OE_API void AddTransformComponent(OEGameEntityID entityID, OETransformComponent component);
	OE_API void DeleteTransformComponent(OEGameEntityID entityID);
	OE_API void AddPhysicsComponent(OEGameEntityID entityID, OEPhysicsComponent component);
	OE_API void DeletePhysicsComponent(OEGameEntityID entityID);
	OE_API void AddRenderableComponent(OEGameEntityID entityID, OERenderableComponent component);
	OE_API void DeleteRenderableComponent(OEGameEntityID entityID);
	OE_API void AddCameraComponent(OEGameEntityID entityID, OECameraComponent component);
	OE_API void DeleteCameraComponent(OEGameEntityID entityID);

	OE_API OETransformComponent GetTransformComponent(OEGameEntityID id);
	OE_API OEPhysicsComponent GetPhysicsComponent(OEGameEntityID id);
	OE_API OERenderableComponent GetRenderableComponent(OEGameEntityID id);
	OE_API OECameraComponent GetCameraComponent(OEGameEntityID id);

	OE_API bool HasTransformComponent(OEGameEntityID id);
	OE_API bool HasPhysicsComponent(OEGameEntityID id);
	OE_API bool HasRenderableComponent(OEGameEntityID id);
	OE_API bool HasCameraComponent(OEGameEntityID id);

	// SECTION ASSET MANAGEMENT
	
	typedef enum OEAssetType
	{
		OE_ASSET_TYPE_UNKNOWN = 0,
		OE_ASSET_TYPE_MESH,
		OE_ASSET_TYPE_MATERIAL,
		OE_ASSET_TYPE_TEXTURE,
	} OEAssetType;

	OE_API OEAssetIdentifierHandle RegisterAsset(OEAssetType type, OEString assetPath);
	OE_API OEAssetIdentifierHandle LoadAsset(OEAssetType type, OEString assetPath);
	OE_API bool GetAssetPath(OEAssetIdentifierHandle identifierHandle, char* buffer, OEUint32_t bufferSize);
	OE_API OEAssetIdentifierHandle GetAssetIdentifierHandle(OEString assetPath);

	// SECTION TIME

	OE_API float GetDeltaTime();
	OE_API double GetElapsedTime();

	// ----------------------
	// SECTION INPUT
	// --------------------

	typedef enum OEKeyCodes
	{
		OE_KEY_UNKNOWN = 0,
		
		OE_KEY_SPACE,
		OE_KEY_ESCAPE,
		OE_KEY_ENTER,
		OE_KEY_TAB,
		OE_KEY_SHIFT,
		OE_KEY_CTRL,
		OE_KEY_ALT,

		OE_KEY_UP,
		OE_KEY_DOWN,
		OE_KEY_LEFT,
		OE_KEY_RIGHT,
		
		OE_KEY_0,
		OE_KEY_1,
		OE_KEY_2,
		OE_KEY_3,
		OE_KEY_4,
		OE_KEY_5,
		OE_KEY_6,
		OE_KEY_7,
		OE_KEY_8,
		OE_KEY_9,

		OE_KEY_A,
		OE_KEY_B,
		OE_KEY_C,
		OE_KEY_D,
		OE_KEY_E,
		OE_KEY_F,
		OE_KEY_G,
		OE_KEY_H,
		OE_KEY_I,
		OE_KEY_J,
		OE_KEY_K,
		OE_KEY_L,
		OE_KEY_M,
		OE_KEY_N,
		OE_KEY_O,
		OE_KEY_P,
		OE_KEY_Q,
		OE_KEY_R,
		OE_KEY_S,
		OE_KEY_T,
		OE_KEY_U,
		OE_KEY_V,
		OE_KEY_W,
		OE_KEY_X,
		OE_KEY_Y,
		OE_KEY_Z
	} OEKeyCodes;

	typedef enum OEMouseButton
	{
		OE_MOUSE_BUTTON_1 = 0,
		OE_MOUSE_BUTTON_2 = 1,
		OE_MOUSE_BUTTON_3 = 2,
		OE_MOUSE_BUTTON_4 = 3,
		OE_MOUSE_BUTTON_5 = 4,
		OE_MOUSE_BUTTON_6 = 5,
		OE_MOUSE_BUTTON_7 = 6,
		OE_MOUSE_BUTTON_8 = 7,

		OE_MOUSE_BUTTON_LEFT = OE_MOUSE_BUTTON_1,
		OE_MOUSE_BUTTON_RIGHT = OE_MOUSE_BUTTON_2,
		OE_MOUSE_BUTTON_MIDDLE = OE_MOUSE_BUTTON_3
	} OEMouseButton;

	OE_API bool IsKeyPressed(OEKeyCodes key);
	OE_API bool IsKeyReleased(OEKeyCodes key);
	OE_API bool IsKeyHeld(OEKeyCodes key);

	OE_API bool IsMouseButtonPressed(OEMouseButton mouseButton);
	OE_API bool IsMouseButtonReleased(OEMouseButton mouseButton);
	OE_API bool IsMouseButtonHeld(OEMouseButton mouseButton);

	OE_API OEVector2 GetMousePosition();
	OE_API OEVector2 GetMouseDelta();
	OE_API int GetScrollDelta();

#ifdef __cplusplus
}
#endif
