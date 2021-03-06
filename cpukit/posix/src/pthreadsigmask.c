/**
 * @file
 *
 * @ingroup POSIXAPI
 *
 * @brief Examine and/or change the calling thread's signal mask
 */

/*
 *  3.3.5 Examine and Change Blocked Signals, P1003.1b-1993, p. 73
 *
 *  NOTE: P1003.1c/D10, p. 37 adds pthread_sigmask().
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include <rtems/posix/pthreadimpl.h>
#include <rtems/posix/psignalimpl.h>
#include <rtems/seterr.h>

int pthread_sigmask(
  int               how,
  const sigset_t   *__restrict set,
  sigset_t         *__restrict oset
)
{
  POSIX_API_Control  *api;

  if ( !set && !oset )
    rtems_set_errno_and_return_minus_one( EINVAL );

  api = _Thread_Get_executing()->API_Extensions[ THREAD_API_POSIX ];

  if ( oset )
    *oset = ~api->signals_unblocked;

  if ( !set )
    return 0;

  switch ( how ) {
    case SIG_BLOCK:
      api->signals_unblocked &= ~*set;
      break;
    case SIG_UNBLOCK:
      api->signals_unblocked |= *set;
      break;
    case SIG_SETMASK:
      api->signals_unblocked = ~*set;
      break;
    default:
      rtems_set_errno_and_return_minus_one( EINVAL );
  }

  /* XXX are there critical section problems here? */

  /* XXX evaluate the new set */

  if ( api->signals_unblocked &
       (api->signals_pending | _POSIX_signals_Pending) ) {
    _Thread_Dispatch();
  }

  return 0;
}
