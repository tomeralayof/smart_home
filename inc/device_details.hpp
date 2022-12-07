#ifndef BADGE_SENSOR_HPP
#define BADGE_SENSOR_HPP

#include <string>

namespace device {

class DeviceDetails {
private:
    using StrRef = const std::string&;
    using Str = const std::string;
    
public:
    DeviceDetails(StrRef m_deviceId, StrRef a_type, StrRef a_room, StrRef a_floor, StrRef a_log,
                    StrRef a_config);
    DeviceDetails(const DeviceDetails& a_other) = default;
    DeviceDetails& operator=(const DeviceDetails& a_other) = default;

    StrRef DeviceId();
    StrRef Type();
    StrRef Room();
    StrRef Floor();
    StrRef Log();
    StrRef Config();

private:
    Str m_deviceId;
    Str m_type;
    Str m_room;
    Str m_floor;
    Str m_log;
    Str m_config;
};

} // namespace

#include "./inl/device_details_inl.hxx"

#endif /* BADGE_SENSOR_HPP */