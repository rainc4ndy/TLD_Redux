#include "Twister.h"

#include <algorithm>

namespace CheatModule {
    namespace Player {
        Twister::Twister() : CCheatModule("装逼", "Twisted Character") {
        }

        bool CheatModule::Player::Twister::onProcess(stOnFootData *data) {
            switch (mode) {
                case TW_Twisted: {
                    data->fQuaternion[0] = static_cast<float>(rand() & 360);
                    data->fQuaternion[1] = static_cast<float>(rand() & 360);
                    data->fQuaternion[2] = static_cast<float>(rand() & 360);
                    data->fQuaternion[3] = static_cast<float>(rand() & 360);
                    switch (rand() % 5) {
                        case 0: {
                            data->sCurrentAnimationID = 972;
                            data->sAnimFlags = 12082;
                            break;
                        }

                        case 1: {
                            data->sCurrentAnimationID = 973;
                            data->sAnimFlags = 12082;
                            break;
                        }

                        case 2: {
                            data->sCurrentAnimationID = 974;
                            data->sAnimFlags = 12082;
                            break;
                        }

                        case 3: {
                            data->sCurrentAnimationID = 975;
                            data->sAnimFlags = 12082;
                            break;
                        }

                        case 4: {
                            data->sCurrentAnimationID = 977;
                            data->sAnimFlags = 12082;
                            break;
                        }
                    }
                    data->fMoveSpeed[2] = 0.01f;
                }
                case TW_MJ: {
                    D3DXQUATERNION Quat;
                    D3DXQUATERNION Quat2;
                    Quat.w = data->fQuaternion[0];
                    Quat.x = data->fQuaternion[1];
                    Quat.y = data->fQuaternion[2];
                    Quat.z = data->fQuaternion[3];
                    D3DXVECTOR3 Axis;
                    Axis.x = 0;
                    Axis.y = 0;
                    Axis.z = 1;
                    D3DXQuaternionRotationAxis(&Quat2, &Axis, 3.1415926f);
                    D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
                    data->fQuaternion[0] = Quat.w;
                    data->fQuaternion[1] = Quat.x;
                    data->fQuaternion[2] = Quat.y;
                    data->fQuaternion[3] = Quat.z;
                    break;
                }
                case TW_UPDOWN: {
                    D3DXQUATERNION Quat, Quat2;
                    Quat.w = data->fQuaternion[0];
                    Quat.x = data->fQuaternion[1];
                    Quat.y = data->fQuaternion[2];
                    Quat.z = data->fQuaternion[3];
                    D3DXVECTOR3 Axis;
                    Axis.x = 0;
                    Axis.y = 1;
                    Axis.z = 0;
                    D3DXQuaternionRotationAxis(&Quat2, &Axis, 3.1415926f);
                    D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
                    data->fQuaternion[0] = Quat.w;
                    data->fQuaternion[1] = Quat.x;
                    data->fQuaternion[2] = Quat.y;
                    data->fQuaternion[3] = Quat.z;
                    break;
                }
                case TW_Spin: {
                    static float fRotation = 0.0f;
                    data->fQuaternion[0] = cosf(fRotation);
                    data->fQuaternion[3] = sinf(fRotation);
                    fRotation += 1.0f;
                    if (fRotation >= 360.0f)
                        fRotation = 0.0f;
                    data->sUpDownKeys = 128;
                    data->sCurrentAnimationID = 0x3C5;
                    data->sAnimFlags = 0x1901;
                    data->fMoveSpeed[2] = 0.01f;
                }
            }
            return true;
        }

        void Twister::onSave(nlohmann::json &j) {
            saveData<decltype(mode)>(j, "mode", mode);
        }

        void Twister::onLoad(const nlohmann::json &j) {
            loadData<decltype(mode)>(j, "mode", mode);
        }
    }
}
