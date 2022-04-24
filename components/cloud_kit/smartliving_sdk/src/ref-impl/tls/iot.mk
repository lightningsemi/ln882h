LIBA_TARGET := libmbedtls.a
CFLAGS      := $(filter-out -ansi,$(CFLAGS))
HDR_REFS    += src/infra

LIB_SRC_PATTERN := *.c */*.c

define Extra_CMake_Head
    echo 'add_definitions (-DDLL_TLS_EXPORTS)' $(1)
    echo '' $(1)
endef

define Extra_CMake_Foot
    echo 'SET (LIBRARY_OUTPUT_PATH ../../../out)' $(1)
    echo '' $(1)
endef
