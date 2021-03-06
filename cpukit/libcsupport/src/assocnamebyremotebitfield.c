/**
 *  @file
 *
 *  @brief RTEMS Associate Name by Remote Bitfield
 *  @ingroup Associativity
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define INSIDE_ASSOC

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */


char *rtems_assoc_name_by_remote_bitfield(
  const rtems_assoc_t *ap,
  uint32_t             value,
  char                *buffer
)
{
  uint32_t   b;

  *buffer = 0;

  for (b = 1; b; b <<= 1) {
    if (b & value) {
      if (*buffer)
	strcat(buffer, " ");
      strcat(buffer, rtems_assoc_name_by_remote(ap, b));
    }
  }

  return buffer;
}
