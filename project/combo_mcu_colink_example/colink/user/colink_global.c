#include "colink_global.h"


static CoLinkDeviceMode dev_mode = DEVICE_MODE_INVALID;

void coLinkSetDeviceMode(CoLinkDeviceMode mode)
{
    dev_mode = mode;
}

CoLinkDeviceMode coLinkGetDeviceMode(void)
{
    return dev_mode;
}


