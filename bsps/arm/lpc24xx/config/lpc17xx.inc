#
# Config file for LPC17XX.
#

include $(RTEMS_ROOT)/make/custom/default.cfg

RTEMS_CPU = arm

CPU_CFLAGS = -mthumb -mcpu=cortex-m3

CFLAGS_OPTIMIZE_V = -O2 -g
CFLAGS_OPTIMIZE_V += -ffunction-sections -fdata-sections

LDFLAGS = -Wl,--gc-sections

