/**
 * @file
 *
 * @ingroup POSIXAPI
 *
 * @brief Attempt to Obtain a Write Lock on a RWLock Instance
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/posix/rwlockimpl.h>
#include <rtems/posix/posixapi.h>

int pthread_rwlock_trywrlock(
  pthread_rwlock_t  *rwlock
)
{
  POSIX_RWLock_Control *the_rwlock;
  Thread_queue_Context  queue_context;
  Status_Control        status;

  the_rwlock = _POSIX_RWLock_Get( rwlock );
  POSIX_RWLOCK_VALIDATE_OBJECT( the_rwlock );

  _Thread_queue_Context_initialize( &queue_context );
  status = _CORE_RWLock_Seize_for_writing(
    &the_rwlock->RWLock,
    false,                 /* we are not willing to wait */
    &queue_context
  );
  return _POSIX_Get_error( status );
}
