#pragma once

namespace ROC
{

class IVRManager
{
public:
    enum DeviceType : unsigned char
    {
        DT_Hmd = 0U,
        DT_Controller,
        DT_Tracker,
        DT_BaseStation,
        DT_Display
    };
    enum HandRole : unsigned char
    {
        HR_Left = 0U,
        HR_Right,
        HR_OptOut,
        HR_Treadmill,
        HR_Stylus
    };
    enum VREye : unsigned char
    {
        VRE_Left = 0U,
        VRE_Right
    };

    virtual bool IsVREnabled() const = 0;
    virtual bool IsDeviceActive(unsigned int p_id) const = 0;
    virtual bool IsDeviceConnected(unsigned int p_id) const = 0;
    virtual bool GetDeviceType(unsigned int p_id, unsigned char &p_type) const = 0;
    virtual bool GetDevicePosition(unsigned int p_id, glm::vec3 &p_pos) const = 0;
    virtual bool GetDeviceRotation(unsigned int p_id, glm::quat &p_rot) const = 0;
    virtual bool GetDeviceVelocity(unsigned int p_id, glm::vec3 &p_val) const = 0;
    virtual bool GetDeviceAngularVelocity(unsigned int p_id, glm::vec3 &p_val) const = 0;
    virtual bool GetDeviceHandRole(unsigned int p_id, unsigned char &p_role) const = 0;
    virtual bool GetDeviceByHandRole(unsigned char p_role, unsigned int &p_id) const = 0;
    virtual bool GetDeviceAxisValues(unsigned int p_id, unsigned int p_axis, glm::vec2 &p_val) const = 0;
    virtual const glm::uvec2& GetTargetsSize() const = 0;
    virtual bool DrawEyeImage(unsigned char p_side, const glm::vec2 &p_pos, const glm::vec2 &p_size, float p_rot, const glm::vec4 &p_color) = 0;
    virtual bool DrawEyeImage(unsigned char p_side, const glm::vec3 &p_pos, const glm::quat &p_rot, const glm::vec2 &p_size, const glm::bvec4 &p_params) = 0;
};

}
