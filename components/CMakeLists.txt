if(COMP_WIFI_SUPPORT)
  add_subdirectory(${COMP_WIFI_DIR})
endif(COMP_WIFI_SUPPORT)

if(COMP_LWIP_SUPPORT)
  add_subdirectory(${COMP_LWIP_DIR})
endif(COMP_LWIP_SUPPORT)

if(COMP_DHCPD_SUPPORT)
  add_subdirectory(${COMP_DHCPD_DIR})
endif(COMP_DHCPD_SUPPORT)
