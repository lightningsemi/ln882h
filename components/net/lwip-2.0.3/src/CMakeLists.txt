# COREFILES, CORE4FILES: The minimum set of files needed for lwIP.
set(COREFILES
    ${COMP_LWIP_DIR}/core/def.c
    ${COMP_LWIP_DIR}/core/dns.c
    ${COMP_LWIP_DIR}/core/inet_chksum.c
    ${COMP_LWIP_DIR}/core/init.c
    ${COMP_LWIP_DIR}/core/ip.c
    ${COMP_LWIP_DIR}/core/memp.c
    ${COMP_LWIP_DIR}/core/netif.c
    ${COMP_LWIP_DIR}/core/pbuf.c
    ${COMP_LWIP_DIR}/core/raw.c
    ${COMP_LWIP_DIR}/core/stats.c
    ${COMP_LWIP_DIR}/core/sys.c
    ${COMP_LWIP_DIR}/core/tcp_in.c
    ${COMP_LWIP_DIR}/core/tcp_out.c
    ${COMP_LWIP_DIR}/core/tcp.c
    ${COMP_LWIP_DIR}/core/timeouts.c
    ${COMP_LWIP_DIR}/core/udp.c
    # ${COMP_LWIP_DIR}/core/mem.c
)

set(CORE4FILES
    ${COMP_LWIP_DIR}/core/ipv4/autoip.c
    ${COMP_LWIP_DIR}/core/ipv4/dhcp.c
    ${COMP_LWIP_DIR}/core/ipv4/etharp.c
    ${COMP_LWIP_DIR}/core/ipv4/icmp.c
    ${COMP_LWIP_DIR}/core/ipv4/igmp.c
    ${COMP_LWIP_DIR}/core/ipv4/ip4_addr.c
    ${COMP_LWIP_DIR}/core/ipv4/ip4_frag.c
    ${COMP_LWIP_DIR}/core/ipv4/ip4.c
)

set(CORE6FILES
    ${COMP_LWIP_DIR}/core/ipv6/dhcp6.c
    ${COMP_LWIP_DIR}/core/ipv6/ethip6.c
    ${COMP_LWIP_DIR}/core/ipv6/icmp6.c
    ${COMP_LWIP_DIR}/core/ipv6/inet6.c
    ${COMP_LWIP_DIR}/core/ipv6/ip6_addr.c
    ${COMP_LWIP_DIR}/core/ipv6/ip6_frag.c
    ${COMP_LWIP_DIR}/core/ipv6/ip6.c
    ${COMP_LWIP_DIR}/core/ipv6/mld6.c
    ${COMP_LWIP_DIR}/core/ipv6/nd6.c
)

# APIFILES: The files which implement the sequential and socket APIs.
set(APIFILES
    ${COMP_LWIP_DIR}/api/api_lib.c
    ${COMP_LWIP_DIR}/api/api_msg.c
    ${COMP_LWIP_DIR}/api/err.c
    ${COMP_LWIP_DIR}/api/netbuf.c
    ${COMP_LWIP_DIR}/api/netdb.c
    ${COMP_LWIP_DIR}/api/netifapi.c
    ${COMP_LWIP_DIR}/api/sockets.c
    ${COMP_LWIP_DIR}/api/tcpip.c
)

# NETIFFILES: Files implementing various generic network interface functions
set(NETIFFILES
    ${COMP_LWIP_DIR}/netif/ethernet.c
    ${COMP_LWIP_DIR}/netif/ethernetif.c
)

# SIXLOWPAN: 6LoWPAN
set(SIXLOWPAN
    ${COMP_LWIP_DIR}/netif/lowpan6.c
)

# PPPFILES: PPP
set(PPPFILES
    ${COMP_LWIP_DIR}/netif/ppp/polarssl/arc4.c
    ${COMP_LWIP_DIR}/netif/ppp/polarssl/des.c
    ${COMP_LWIP_DIR}/netif/ppp/polarssl/md4.c
    ${COMP_LWIP_DIR}/netif/ppp/polarssl/md5.c
    ${COMP_LWIP_DIR}/netif/ppp/polarssl/sha1.c
    ${COMP_LWIP_DIR}/netif/ppp/auth.c
    ${COMP_LWIP_DIR}/netif/ppp/ccp.c
    ${COMP_LWIP_DIR}/netif/ppp/chap_ms.c
    ${COMP_LWIP_DIR}/netif/ppp/chap-md5.c
    ${COMP_LWIP_DIR}/netif/ppp/chap-new.c
    ${COMP_LWIP_DIR}/netif/ppp/demand.c
    ${COMP_LWIP_DIR}/netif/ppp/eap.c
    ${COMP_LWIP_DIR}/netif/ppp/ecp.c
    ${COMP_LWIP_DIR}/netif/ppp/eui64.c
    ${COMP_LWIP_DIR}/netif/ppp/fsm.c
    ${COMP_LWIP_DIR}/netif/ppp/ipcp.c
    ${COMP_LWIP_DIR}/netif/ppp/ipv6cp.c
    ${COMP_LWIP_DIR}/netif/ppp/lcp.c
    ${COMP_LWIP_DIR}/netif/ppp/magic.c
    ${COMP_LWIP_DIR}/netif/ppp/mppe.c
    ${COMP_LWIP_DIR}/netif/ppp/multilink.c
    ${COMP_LWIP_DIR}/netif/ppp/ppp.c
    ${COMP_LWIP_DIR}/netif/ppp/pppapi.c
    ${COMP_LWIP_DIR}/netif/ppp/pppcrypt.c
    ${COMP_LWIP_DIR}/netif/ppp/pppoe.c
    ${COMP_LWIP_DIR}/netif/ppp/pppol2tp.c
    ${COMP_LWIP_DIR}/netif/ppp/pppos.c
    ${COMP_LWIP_DIR}/netif/ppp/upap.c
    ${COMP_LWIP_DIR}/netif/ppp/utils.c
    ${COMP_LWIP_DIR}/netif/ppp/vj.c
)

# LWIPNOAPPSFILES: All LWIP files without apps
set(LWIPNOAPPSFILES
    ${COREFILES}
    ${CORE4FILES}
    ${CORE6FILES}
    ${APIFILES}
    ${NETIFFILES}
    ${PPPFILES}
    ${SIXLOWPAN}
)

# HTTPDFILES: HTTP server
set(HTTPDFILES
    ${COMP_LWIP_DIR}/apps/httpd/fs.c
    ${COMP_LWIP_DIR}/apps/httpd/fsdata.c
    ${COMP_LWIP_DIR}/apps/httpd/httpd.c
)

# LWIPERFFILES: IPERF server
set(LWIPERFFILES
    ${COMP_LWIP_DIR}/apps/lwiperf/lwiperf.c
)

# MDNSFILES: MDNS responder
set(MDNSFILES
    ${COMP_LWIP_DIR}/apps/mdns/mdns.c
)

# MQTTFILES: MQTT client files
set(MQTTFILES
    ${COMP_LWIP_DIR}/apps/mqtt/mqtt.c
)

# NETBIOSNSFILES: NetBIOS name server
set(NETBIOSNSFILES
    ${COMP_LWIP_DIR}/apps/netbiosns/netbiosns.c
)

# SNMPFILES: SNMPv2c agent
set(SNMPFILES
    ${COMP_LWIP_DIR}/apps/snmp/snmp_asn1.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_core.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2_icmp.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2_interfaces.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2_ip.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2_snmp.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2_system.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2_tcp.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2_udp.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_mib2.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_msg.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_netconn.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_pbuf_stream.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_raw.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_scalar.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_table.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_threadsync.c
    ${COMP_LWIP_DIR}/apps/snmp/snmp_traps.c
    ${COMP_LWIP_DIR}/apps/snmp/snmpv3_dummy.c
    ${COMP_LWIP_DIR}/apps/snmp/snmpv3_mbedtls.c
    ${COMP_LWIP_DIR}/apps/snmp/snmpv3.c
)

# SNTPFILES: SNTP client
set(SNTPFILES
    ${COMP_LWIP_DIR}/apps/sntp/sntp.c
)

# TFTPFILES: TFTP server files
set(TFTPFILES
    ${COMP_LWIP_DIR}/apps/tftp/tftp_server.c
)

# LWIPAPPFILES: All LWIP APPs
set(LWIPAPPFILES
    ${SNMPFILES}
    ${HTTPDFILES}
    ${LWIPERFFILES}
    ${SNTPFILES}
    ${MDNSFILES}
    ${NETBIOSNSFILES}
    ${TFTPFILES}
    ${MQTTFILES}
)

set(LWIP_SRC_C
    ${COREFILES}
    ${CORE4FILES}
    ${APIFILES}
    ${NETIFFILES}
    ${COMP_LWIP_DIR}/port/arch/lwip_hooks.c
    ${COMP_LWIP_DIR}/port/arch/sys_arch.c
)

set(lwip_static_target "lwip")
add_library(${lwip_static_target} STATIC ${LWIP_SRC_C})
add_library(ln::lwip ALIAS ${lwip_static_target})

target_include_directories(${lwip_static_target}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/project/${USER_PROJECT}/bsp
    ${CMAKE_SOURCE_DIR}/project/${USER_PROJECT}/cfg
    ${MCU_LN882X_DIR}/ln88xx
    ${MCU_LN882X_DIR}/CMSIS_5.3.0
    ${COMP_LWIP_DIR}/include
    ${COMP_LWIP_DIR}/include/lwip
    ${COMP_LWIP_DIR}/include/lwip/prot
    ${COMP_LWIP_DIR}/include/lwip/priv
    ${COMP_LWIP_DIR}/include/netif
    ${COMP_LWIP_DIR}/include/posix
    ${COMP_LWIP_DIR}/include/posix/sys
    ${COMP_LWIP_DIR}/port
    ${COMP_LWIP_DIR}/port/arch
    ${COMP_DHCPD_DIR}
    ${COMP_WIFI_DIR}/wifi_manager
    ${COMP_KERNEL_DIR}
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/include
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/components
)
