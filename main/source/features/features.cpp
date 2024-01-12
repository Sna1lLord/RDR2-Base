#include "features.hpp"
#include "../../utility/invoker/natives.hpp"
#include "../script/script.hpp"
#include "../ui/ui.hpp"
#include "../../utility/memory/signatures/signatures.hpp"
#include "../../utility/logger/logger.hpp"

static Features* featuresInstance = nullptr;

Features* Features::Instance() {
	if (!featuresInstance)
		featuresInstance = new Features();

	return featuresInstance;
}

void Features::Initialize() {
    // in case you need to initialize feature variables
}

void Features::Uninitialize() {
	featuresInstance = nullptr;
    delete this;
}

NativeVector Features::RotationToDirection(NativeVector rotation) {
	float radiansZ = rotation.z * 0.0174532924f;
	float radiansX = rotation.x * 0.0174532924f;
	float num = abs((float)cos((double)radiansX));

	NativeVector result;
	result.x = (float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num);
	result.y = (float)((double)((float)cos((double)radiansZ)) * (double)num);
	result.z = (float)sin((double)radiansX);

	return result;
}

NativeVector Features::LoadGround(NativeVector location) {
    NativeVector result = location;
    float groundZ;

    for (int i = 0; i < 30; i++) {
        for (float z = 1000.f; z >= 0.f; z -= 100.f) {
            STREAMING::REQUEST_COLLISION_AT_COORD(location.x, location.y, z);
            ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), location.x, location.y, z, false, false, false);

            Script::Current()->ScriptYield();
        }

        if (MISC::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, 1000.f, &groundZ, false)) {
            result.z = groundZ + 1.f;
            return result;
        }
    }

    result.z = 900.f;
    return result;
}

static int noClipCamera = 0;

void Features::Tick() {
    localPlayer = PLAYER::PLAYER_ID();
    localPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(localPlayer);
    localHorse = PED::GET_MOUNT(localPed);

    if (Ui::Instance()->opened) {
        PAD::DISABLE_CONTROL_ACTION(2, 0x7f8d09b8, true);
        PAD::DISABLE_CONTROL_ACTION(2, 0xa987235f, true);
        PAD::DISABLE_CONTROL_ACTION(2, 0xd2047988, true);
        PAD::DISABLE_CONTROL_ACTION(2, 0xc0651d40, true);
        PAD::DISABLE_CONTROL_ACTION(2, 0x8ed92e16, true);
        PAD::DISABLE_CONTROL_ACTION(2, 0x08f8bc6d, true);
        PAD::DISABLE_CONTROL_ACTION(2, 0xa1eb1353, true);

        PAD::DISABLE_CONTROL_ACTION(2, 0x07ce1e61, true);
        PAD::DISABLE_CONTROL_ACTION(2, 0xf84fa74f, true);
    }

    ENTITY::SET_ENTITY_INVINCIBLE(localPed, Config::Instance()->self.godmode);

    if (Config::Instance()->self.demiGodmode) {
        ENTITY::SET_ENTITY_HEALTH(localPed, ENTITY::GET_ENTITY_MAX_HEALTH(localPed, true), 0);
    }

    if (Config::Instance()->self.noClip) {
        int entity = localPed;
		if (PED::IS_PED_ON_MOUNT(Features::Instance()->localPed))
            entity = localHorse;

        NativeVector coords = ENTITY::GET_ENTITY_COORDS(entity, true, true);
		NativeVector cameraCoords = CAM::GET_GAMEPLAY_CAM_COORD();
		NativeVector rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);

        if (!CAM::DOES_CAM_EXIST(noClipCamera)) {
			noClipCamera = CAM::CREATE_CAM(xorstr_("DEFAULT_SCRIPTED_CAMERA"), 1);
			CAM::SET_CAM_COORD(noClipCamera, cameraCoords.x, cameraCoords.y, cameraCoords.z);
		}

		CAM::SET_CAM_ROT(noClipCamera, rotation.x, rotation.y, rotation.z, 2);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, coords.x, coords.y, coords.z, false, false, false);

        NativeVector scriptedCoords = CAM::GET_CAM_COORD(noClipCamera);
		NativeVector direction = RotationToDirection(rotation);

		if (GetAsyncKeyState('W')) {
			NativeVector second = direction * Config::Instance()->self.noClipSpeed;
			NativeVector c = scriptedCoords + second;
			CAM::SET_CAM_COORD(noClipCamera, c.x, c.y, c.z);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, c.x, c.y, c.z - 1.7f, false, false, false);
		}

		if (GetAsyncKeyState('S')) {
			NativeVector second = direction * -Config::Instance()->self.noClipSpeed;
			NativeVector c = scriptedCoords + second;
			CAM::SET_CAM_COORD(noClipCamera, c.x, c.y, c.z);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, c.x, c.y, c.z - 1.7f, false, false, false);
		}
    }
    else {
		CAM::DESTROY_CAM(noClipCamera, true);
	}
}