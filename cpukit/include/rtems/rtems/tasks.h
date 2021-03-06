/**
 * @file
 *
 * @ingroup ClassicTasks
 *
 * @brief Classic Task Manager API
 */

/*
 * COPYRIGHT (c) 1989-2014.
 * On-Line Applications Research Corporation (OAR).
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_RTEMS_TASKS_H
#define _RTEMS_RTEMS_TASKS_H

#include <rtems/rtems/attr.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/types.h>
#include <rtems/score/context.h>
#include <rtems/score/smp.h>
#include <rtems/score/stack.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup ClassicTasks Tasks
 *
 *  @ingroup RTEMSAPIClassic
 *
 *  This encapsulates the functionality of the Classic API Task Manager.
 *  This functionality includes task services such as creation, deletion,
 *  delays, suspend/resume, and manipulation of execution mode and priority.
 */
/**@{*/

/**
 *  Constant to be used as the ID of current task
 */
#define RTEMS_SELF                OBJECTS_ID_OF_SELF

/**
 *  This constant is passed to the rtems_task_wake_after directive as the
 *  interval when a task wishes to yield the CPU.
 */
#define RTEMS_YIELD_PROCESSOR WATCHDOG_NO_TIMEOUT

/**
 * @brief Minimum stack size which every thread must exceed.
 *
 * It is the minimum stack size recommended for use on this processor. This
 * value is selected by the RTEMS developers conservatively to minimize the
 * risk of blown stacks for most user applications. Using this constant when
 * specifying the task stack size, indicates that the stack size will be at
 * least RTEMS_MINIMUM_STACK_SIZE bytes in size. If the user configured minimum
 * stack size is larger than the recommended minimum, then it will be used.
 */
#define RTEMS_MINIMUM_STACK_SIZE  STACK_MINIMUM_SIZE

/**
 * @brief Specifies that the task should be created with the configured minimum
 * stack size.
 *
 * Using this constant when specifying the task stack size indicates that this
 * task is to be created with a stack size of the minimum stack size that was
 * configured by the application. If not explicitly configured by the
 * application, the default configured minimum stack size is the processor
 * dependent value RTEMS_MINIMUM_STACK_SIZE. Since this uses the configured
 * minimum stack size value, you may get a stack size that is smaller or larger
 * than the recommended minimum. This can be used to provide large stacks for
 * all tasks on complex applications or small stacks on applications that are
 * trying to conserve memory.
 */
#define RTEMS_CONFIGURED_MINIMUM_STACK_SIZE  0

/**
 *  Define the type for an RTEMS API task priority.
 */
typedef uint32_t rtems_task_priority;

/**
 *  This is the constant used with the rtems_task_set_priority
 *  directive to indicate that the caller wants to obtain its
 *  current priority rather than set it as the name of the
 *  directive indicates.
 */
#define RTEMS_NO_PRIORITY           RTEMS_CURRENT_PRIORITY

/**
 *  This constant is the least valid value for a Classic API
 *  task priority.
 */
#define RTEMS_MINIMUM_PRIORITY      1

rtems_task_priority _RTEMS_Maximum_priority( void );

/**
 *  This run-time constant is the maximum valid value for a Classic API
 *  task priority.
 *
 *  @note This is actually the priority of the IDLE thread so
 *        using this priority will result in having a task
 *        which never executes.  This could be useful if you
 *        want to ensure that a task does not executes during
 *        certain operations such as a system mode change.
 */
#define RTEMS_MAXIMUM_PRIORITY      _RTEMS_Maximum_priority()

/**
 *  The following constant is passed to rtems_task_set_priority when the
 *  caller wants to obtain the current priority.
 */
#define RTEMS_CURRENT_PRIORITY      0

struct _Thread_Control;

/**
 *  External API name for Thread_Control
 */
typedef struct _Thread_Control rtems_tcb;

/**
 *  The following defines the "return type" of an RTEMS task.
 */
typedef void rtems_task;

/**
 *  The following defines the argument to an RTEMS task.
 */
typedef CPU_Uint32ptr rtems_task_argument;

/**
 *  The following defines the type for the entry point of an RTEMS task.
 */
typedef rtems_task ( *rtems_task_entry )(
                      rtems_task_argument
                   );

/**
 *  The following records define the Initialization Tasks Table.
 *  Each entry contains the information required by RTEMS to
 *  create and start a user task automatically at executive
 *  initialization time.
 */
typedef struct {
  /** This is the Initialization Task's name. */
  rtems_name            name;
  /** This is the Initialization Task's stack size. */
  size_t                stack_size;
  /** This is the Initialization Task's priority. */
  rtems_task_priority   initial_priority;
  /** This is the Initialization Task's attributes. */
  rtems_attribute       attribute_set;
  /** This is the Initialization Task's entry point. */
  rtems_task_entry      entry_point;
  /** This is the Initialization Task's initial mode. */
  rtems_mode            mode_set;
  /** This is the Initialization Task's argument. */
  rtems_task_argument	argument;
} rtems_initialization_tasks_table;

/**
 * @brief RTEMS Task Create
 *
 * This routine implements the rtems_task_create directive. The task
 * will have the name name. The attribute_set can be used to indicate
 * that the task will be globally accessible or utilize floating point.
 * The task's stack will be stack_size bytes. The task will begin
 * execution with initial_priority and initial_modes. It returns the
 * id of the created task in ID.
 *
 * @param[in] name is the user defined thread name
 * @param[in] initial_priority is the thread priority
 * @param[in] stack_size is the stack size in bytes
 * @param[in] initial_modes is the initial thread mode
 * @param[in] attribute_set is the thread attributes
 * @param[in] id is the pointer to thread id
 *
 * @retval RTEMS_SUCCESSFUL if successful or error code if unsuccessful
 *             	and *id thread id filled in
 */
rtems_status_code rtems_task_create(
  rtems_name           name,
  rtems_task_priority  initial_priority,
  size_t               stack_size,
  rtems_mode           initial_modes,
  rtems_attribute      attribute_set,
  rtems_id            *id
);

/**
 * @brief This constant defines the recommended alignment of a task storage
 *   area in bytes.
 *
 * Use it with RTEMS_ALIGNED() to define the alignment of a statically
 * allocated task storage area.
 */
#define RTEMS_TASK_STORAGE_ALIGNMENT CPU_HEAP_ALIGNMENT

/**
 * @brief Returns the recommended task storage area size for the specified size
 *   and task attributes.
 *
 * @param _size is the size dedicated to the task stack and thread-local
 *   storage in bytes.
 *
 * @param _attributes is the attribute set of the task using the storage area.
 *
 * @return The recommended task storage area size calculated from the input
 *   parameters is returned.
 */
#if CPU_ALL_TASKS_ARE_FP == TRUE
  #define RTEMS_TASK_STORAGE_SIZE( _size, _attributes ) \
    ( ( _size ) + CONTEXT_FP_SIZE )
#else
  #define RTEMS_TASK_STORAGE_SIZE( _size, _attributes ) \
    ( ( _size ) + \
      ( ( ( _attributes ) & RTEMS_FLOATING_POINT ) != 0 ? \
        CONTEXT_FP_SIZE : 0 ) )
#endif

/**
 * @brief This structure defines the configuration of a task constructed by
 *   rtems_task_construct().
 */
typedef struct {
  /**
   * @brief This member defines the name of the task.
   */
  rtems_name name;

  /**
   * @brief This member defines the initial priority of the task.
   */
  rtems_task_priority initial_priority;

  /**
   * @brief This member shall point to the task storage area begin.
   *
   * The task storage area will contain the task stack, the thread-local storage,
   * and the floating-point context on architectures with a separate
   * floating-point context.
   *
   * The task storage area begin address and size should be aligned by
   * #RTEMS_TASK_STORAGE_ALIGNMENT.  To avoid memory waste, use RTEMS_ALIGNED()
   * and #RTEMS_TASK_STORAGE_ALIGNMENT to enforce the recommended alignment of a
   * statically allocated task storage area.
   */
  void *storage_area;

  /**
   * @brief This member defines size of the task storage area in bytes.
   *
   * Use the RTEMS_TASK_STORAGE_SIZE() macro to determine the recommended task
   * storage area size.
   */
  size_t storage_size;

  /**
   * @brief This member defines the maximum thread-local storage size supported
   *   by the task storage area.
   *
   * Use RTEMS_ALIGN_UP() and #RTEMS_TASK_STORAGE_ALIGNMENT to adjust the size to
   * meet the minimum alignment requirement of a thread-local storage area used
   * to construct a task.
   *
   * If the value is less than the actual thread-local storage size, then the
   * task construction by rtems_task_construct() fails.
   *
   * If the is less than the task storage area size, then the task construction
   * by rtems_task_construct() fails.
   *
   * The actual thread-local storage size is determined when the application
   * executable is linked.  The ``rtems-exeinfo`` command line tool included in
   * the RTEMS Tools can be used to obtain the thread-local storage size and
   * alignment of an application executable.
   *
   * The application may configure the maximum thread-local storage size for all
   * threads explicitly through the #CONFIGURE_MAXIMUM_THREAD_LOCAL_STORAGE_SIZE
   * configuration option.
   */
  size_t maximum_thread_local_storage_size;

  /**
   * @brief This member defines the optional handler to free the task storage
   *   area.
   *
   * It is called on exactly two mutually exclusive occasions.  Firstly, when the
   * task construction aborts due to a failed task create extension, or secondly,
   * when the task is deleted.  It is called from task context under protection
   * of the object allocator lock.  It is allowed to call free() in this handler.
   * If handler is NULL, then no action will be performed.
   */
  void ( *storage_free )( void * );

  /**
   * @brief This member defines the initial modes of the task.
   */
  rtems_mode initial_modes;

  /**
   * @brief This member defines the attributes of the task.
   */
  rtems_attribute attributes;
} rtems_task_config;

/**
 * @brief Constructs a task from the specified the task configuration.
 *
 * In contrast to tasks created by rtems_task_create(), the tasks constructed
 * by this directive use a user-provided task storage area.  The task storage
 * area contains the task stack, the thread-local storage, and the
 * floating-point context on architectures with a separate floating-point
 * context.
 *
 * This directive is intended for applications which do not want to use the
 * RTEMS Workspace and instead statically allocate all operating system
 * resources.  It is not recommended to use rtems_task_create() and
 * rtems_task_construct() together in an application.  It is also not
 * recommended to use rtems_task_construct() for drivers or general purpose
 * libraries.  The reason for these recommendations is that the task
 * configuration needs settings which can be only given with a through
 * knowledge of the application resources.
 *
 * An application based solely on static allocation can avoid any runtime
 * memory allocators.  This can simplify the application architecture as well
 * as any analysis that may be required.
 *
 * The stack space estimate done by <rtems/confdefs.h> assumes that all tasks
 * are created by rtems_task_create().  The estimate can be adjusted to take
 * user-provided task storage areas into account through the
 * #CONFIGURE_MINIMUM_TASKS_WITH_USER_PROVIDED_STORAGE application
 * configuration option.
 *
 * The #CONFIGURE_MAXIMUM_TASKS should include tasks constructed by
 * rtems_task_construct().
 *
 * @param config is the task configuration.
 *
 * @param[out] id is the pointer to an object identifier variable.  The
 *   identifier of the constructed task object will be stored in this variable,
 *   in case of a successful operation.
 *
 * @retval ::RTEMS_SUCCESSFUL The requested operation was successful.
 *
 * @retval ::RTEMS_INVALID_ADDRESS The id parameter was NULL.
 *
 * @retval ::RTEMS_INVALID_NAME The task name was invalid.
 *
 * @retval ::RTEMS_INVALID_PRIORITY The initial task priority was invalid.
 *
 * @retval ::RTEMS_INVALID_SIZE The thread-local storage size is greater than
 *   the maximum thread-local storage size specified in the task configuration.
 *   The thread-local storage size is determined by the thread-local variables
 *   used by the application and #CONFIGURE_MAXIMUM_THREAD_LOCAL_STORAGE_SIZE.
 *
 * @retval ::RTEMS_INVALID_SIZE The task storage area was too small to provide
 *   a task stack of the configured minimum size, see
 *   #CONFIGURE_MINIMUM_TASK_STACK_SIZE. The task storage area contains the
 *   task stack, the thread-local storage, and the floating-point context on
 *   architectures with a separate floating-point context.
 *
 * @retval ::RTEMS_TOO_MANY There was no inactive task object available to
 *   construct a task.
 *
 * @retval ::RTEMS_TOO_MANY In multiprocessing configurations, there was no
 *   inactive global object available to construct a global task.
 *
 * @retval ::RTEMS_UNSATISFIED One of the task create extensions failed during
 *   the task construction.
 *
 * @retval ::RTEMS_UNSATISFIED In SMP configurations, the non-preemption mode
 *   was not supported.
 *
 * @retval ::RTEMS_UNSATISFIED In SMP configurations, the interrupt level mode
 *   was not supported.
 */
rtems_status_code rtems_task_construct(
  const rtems_task_config *config,
  rtems_id                *id
);

/**
 * @brief RTEMS Task Name to Id
 *
 * This routine implements the rtems_task_ident directive.
 * This directive returns the task ID associated with name.
 * If more than one task is named name, then the task to
 * which the ID belongs is arbitrary. node indicates the
 * extent of the search for the ID of the task named name.
 * The search can be limited to a particular node or allowed to
 * encompass all nodes.
 *
 * @param[in] name is the user defined thread name
 * @param[in] node is(are) the node(s) to be searched
 * @param[in] id is the pointer to thread id
 *
 * @retval This method returns RTEMS_SUCCESSFUL if there was not an
 *         error. Otherwise, a status code is returned indicating the
 *         source of the error. If successful, the id will
 *         be filled in with the thread id.
 */
rtems_status_code rtems_task_ident(
  rtems_name    name,
  uint32_t      node,
  rtems_id     *id
);

/**
 * @brief RTEMS Delete Task
 *
 * This routine implements the rtems_task_delete directive. The
 * task indicated by ID is deleted. The executive halts execution
 * of the thread and frees the thread control block.
 *
 * @param[in] id is the thread id
 *
 * @retval This method returns RTEMS_SUCCESSFUL if there was not an
 *         error and id is not the requesting thread. Status code is
 *         returned indicating the source of the error. Nothing
 *         is returned if id is the requesting thread (always succeeds).
 */
rtems_status_code rtems_task_delete(
  rtems_id   id
);

RTEMS_NO_RETURN void rtems_task_exit( void );

/**
 * @brief RTEMS Task Mode
 *
 * This routine implements the rtems_task_mode directive. The current
 * values of the modes indicated by mask of the calling task are changed
 * to that indicated in mode_set. The former mode of the task is
 * returned in mode_set.
 *
 * @param[in] mode_set is the new mode
 * @param[in] mask is the mask
 * @param[in] previous_mode_set is the address of previous mode set
 *
 * @retval RTEMS_SUCCESSFUL and previous_mode_set filled in with the
 * previous mode set
 */
rtems_status_code rtems_task_mode(
  rtems_mode  mode_set,
  rtems_mode  mask,
  rtems_mode *previous_mode_set
);

/**
 * @brief RTEMS Task Restart
 *
 * This routine implements the rtems_task_restart directive. The
 * task associated with ID is restarted at its initial entry
 * point with the new argument.
 *
 * @param[in] id is the thread id
 * @param[in] argument is the thread argument
 *
 * @retval RTEMS_SUCCESSFUL if successful or error code if unsuccessful
 */
rtems_status_code rtems_task_restart(
  rtems_id            id,
  rtems_task_argument argument
);

/**
 * @brief RTEMS Suspend Task
 *
 * This routine implements the rtems_task_suspend directive. The
 * SUSPENDED state is set for task associated with ID. Note that the
 * suspended state can be in addition to other waiting states.
 *
 * @param[in] id is the thread id
 *
 * @retval This method returns RTEMS_SUCCESSFUL if there was not an
 *         error. Otherwise, a status code is returned indicating the
 *         source of the error.
 */
rtems_status_code rtems_task_suspend(
  rtems_id   id
);

/**
 * @brief RTEMS Resume Task
 *
 * This routine implements the rtems_task_resume Directive. The
 * SUSPENDED state is cleared for task associated with ID.
 *
 * @param[in] id is the thread id
 *
 * @retval This method returns RTEMS_SUCCESSFUL if there was not an
 *         error. Otherwise, a status code is returned indicating the
 *         source of the error.
 */
rtems_status_code rtems_task_resume(
  rtems_id   id
);

/**
 * @brief RTEMS Set Task Priority
 *
 * This routine implements the rtems_task_set_priority directive. The
 * current priority of the task associated with ID is set to
 * new_priority. The former priority of that task is returned
 * in old_priority.
 *
 * @param[in] id is the thread to extract
 * @param[in] new_priority is the thread to extract
 * @param[in] old_priority is the thread to extract
 *
 * @retval RTEMS_SUCCESSFUL if successful or error code if unsuccessful and
 * and *old_priority filled in with the previous previous priority
 */
rtems_status_code rtems_task_set_priority(
  rtems_id             id,
  rtems_task_priority  new_priority,
  rtems_task_priority *old_priority
);

/**
 * @brief Gets the current priority of the specified task with respect to the
 * specified scheduler instance.
 *
 * The current priority reflects temporary priority adjustments due to locking
 * protocols, the rate-monotonic period objects on some schedulers and other
 * mechanisms.
 *
 * @param[in] task_id Identifier of the task.  Use @ref RTEMS_SELF to select
 *   the executing task.
 * @param[in] scheduler_id Identifier of the scheduler instance.
 * @param[out] priority Returns the current priority of the specified task with
 *   respect to the specified scheduler instance.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_ILLEGAL_ON_REMOTE_OBJECT Directive is illegal on remote tasks.
 * @retval RTEMS_INVALID_ADDRESS The priority parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid task or scheduler identifier.
 * @retval RTEMS_NOT_DEFINED The task has no priority within the specified
 *   scheduler instance.  This error is only possible on SMP configurations.
 *
 * @see rtems_scheduler_ident().
 */
rtems_status_code rtems_task_get_priority(
  rtems_id             task_id,
  rtems_id             scheduler_id,
  rtems_task_priority *priority
);

/**
 *  @brief RTEMS Start Task
 *
 *  RTEMS Task Manager
 *
 *  This routine implements the rtems_task_start directive.  The
 *  starting execution point of the task associated with ID is
 *  set to entry_point with the initial argument.
 */
rtems_status_code rtems_task_start(
  rtems_id             id,
  rtems_task_entry     entry_point,
  rtems_task_argument  argument
);

/**
 * @brief RTEMS Task Wake When
 *
 * This routine implements the rtems_task_wake_when directive. The
 * calling task is blocked until the current time of day is
 * equal to that indicated by time_buffer.
 *
 * @param[in] time_buffer is the pointer to the time and date structure
 *
 * @retval RTEMS_SUCCESSFUL if successful or error code if unsuccessful
 */
rtems_status_code rtems_task_wake_when(
  rtems_time_of_day *time_buffer
);

/**
 * @brief RTEMS Task Wake After
 *
 * This routine implements the rtems_task_wake_after directive. The
 * calling task is blocked until the indicated number of clock
 * ticks have occurred.
 *
 * @param[in] ticks is the number of ticks to wait
 * @retval RTEMS_SUCCESSFUL
 */
rtems_status_code rtems_task_wake_after(
  rtems_interval  ticks
);

/**
 *  @brief rtems_task_is_suspended
 *
 *  This directive returns a status indicating whether or not
 *  the specified task is suspended.
 */
rtems_status_code rtems_task_is_suspended(
  rtems_id   id
);

/**
 * @brief Gets the processor affinity set of a task.
 *
 * @param[in] id Identifier of the task.  Use @ref RTEMS_SELF to select the
 * executing task.
 * @param[in] cpusetsize Size of the specified affinity set buffer in
 * bytes.  This value must be positive.
 * @param[out] cpuset The current processor affinity set of the task.  A set
 * bit in the affinity set means that the task can execute on this processor
 * and a cleared bit means the opposite.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a cpuset parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid task identifier.
 * @retval RTEMS_INVALID_NUMBER The affinity set buffer is too small for the
 * current processor affinity set of the task.
 */
rtems_status_code rtems_task_get_affinity(
  rtems_id             id,
  size_t               cpusetsize,
  cpu_set_t           *cpuset
);

/**
 * @brief Sets the processor affinity set of a task.
 *
 * This function will not change the scheduler of the task.  The intersection
 * of the processor affinity set and the set of processors owned by the
 * scheduler of the task must be non-empty.  It is not an error if the
 * processor affinity set contains processors that are not part of the set of
 * processors owned by the scheduler instance of the task.  A task will simply
 * not run under normal circumstances on these processors since the scheduler
 * ignores them.  Some locking protocols may temporarily use processors that
 * are not included in the processor affinity set of the task.  It is also not
 * an error if the processor affinity set contains processors that are not part
 * of the system.
 *
 * @param[in] id Identifier of the task.  Use @ref RTEMS_SELF to select the
 * executing task.
 * @param[in] cpusetsize Size of the specified affinity set buffer in
 * bytes.  This value must be positive.
 * @param[in] cpuset The new processor affinity set for the task.  A set bit in
 * the affinity set means that the task can execute on this processor and a
 * cleared bit means the opposite.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a cpuset parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid task identifier.
 * @retval RTEMS_INVALID_NUMBER Invalid processor affinity set.
 */
rtems_status_code rtems_task_set_affinity(
  rtems_id         id,
  size_t           cpusetsize,
  const cpu_set_t *cpuset
);

/**
 * @brief Gets the scheduler of a task.
 *
 * @param[in] task_id Identifier of the task.  Use @ref RTEMS_SELF to select
 * the executing task.
 * @param[out] scheduler_id Identifier of the scheduler instance.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a scheduler_id parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid task identifier.
 */
rtems_status_code rtems_task_get_scheduler(
  rtems_id  task_id,
  rtems_id *scheduler_id
);

/**
 * @brief Sets the scheduler instance of a task.
 *
 * Initially, the scheduler instance of a task is set to the scheduler instance
 * of the task that created it.  This directive allows to move a task from its
 * current scheduler instance to another specified by the scheduler identifier.
 *
 * @param[in] task_id Identifier of the task.  Use @ref RTEMS_SELF to select
 *   the executing task.
 * @param[in] scheduler_id Identifier of the scheduler instance.
 * @param[in] priority The task priority with respect to the new scheduler
 *   instance.  The real and initial priority of the task is set to this value.
 *   The initial priority is used by rtems_task_restart() for example.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_ILLEGAL_ON_REMOTE_OBJECT Directive is illegal on remote tasks.
 * @retval RTEMS_INVALID_ID Invalid task or scheduler identifier.
 * @retval RTEMS_INVALID_PRIORITY Invalid priority.
 * @retval RTEMS_RESOURCE_IN_USE The task owns resources which deny a scheduler
 *   change.
 *
 * @see rtems_scheduler_ident().
 */
rtems_status_code rtems_task_set_scheduler(
  rtems_id            task_id,
  rtems_id            scheduler_id,
  rtems_task_priority priority
);

/**
 *  @brief RTEMS Get Self Task Id
 *
 *  This directive returns the ID of the currently executing task.
 */
rtems_id rtems_task_self(void);

/**
 * @brief Task visitor.
 *
 * @param[in] tcb The task control block.
 * @param[in] arg The visitor argument.
 *
 * @retval true Stop the iteration.
 * @retval false Otherwise.
 *
 * @see rtems_task_iterate().
 */
typedef bool ( *rtems_task_visitor )( rtems_tcb *tcb, void *arg );

/**
 * @brief Iterates over all tasks in the system.
 *
 * This operation covers all tasks of all APIs.
 *
 * Must be called from task context.  This operation obtains and releases the
 * objects allocator lock.  The task visitor is called while owning the objects
 * allocator lock.  It is possible to perform blocking operations in the task
 * visitor, however, take care that no deadlocks via the object allocator lock
 * can occur.
 *
 * @param[in] visitor The task visitor.
 * @param[in] arg The visitor argument.
 */
void rtems_task_iterate(
  rtems_task_visitor  visitor,
  void               *arg
);

/**
 * @brief Identifies a scheduler by its name.
 *
 * The scheduler name is determined by the scheduler configuration.
 *
 * @param[in] name The scheduler name.
 * @param[out] id The scheduler identifier associated with the name.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a id parameter is @c NULL.
 * @retval RTEMS_INVALID_NAME Invalid scheduler name.
 */
rtems_status_code rtems_scheduler_ident(
  rtems_name  name,
  rtems_id   *id
);

/**
 * @brief Identifies a scheduler by a processor index.
 *
 * @param[in] cpu_index The processor index.
 * @param[out] id The scheduler identifier associated with the processor index.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a id parameter is @c NULL.
 * @retval RTEMS_INVALID_NAME Invalid processor index.
 * @retval RTEMS_INCORRECT_STATE The processor index is valid, however, this
 *   processor is not owned by a scheduler.
 */
rtems_status_code rtems_scheduler_ident_by_processor(
  uint32_t  cpu_index,
  rtems_id *id
);

/**
 * @brief Identifies a scheduler by a processor set.
 *
 * The scheduler is selected according to the highest numbered online processor
 * in the specified processor set.
 *
 * @param[in] cpusetsize Size of the specified processor set buffer in
 *   bytes.  This value must be positive.
 * @param[out] cpuset The processor set to identify the scheduler.
 * @param[out] id The scheduler identifier associated with the processor set.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a id parameter is @c NULL.
 * @retval RTEMS_INVALID_SIZE Invalid processor set size.
 * @retval RTEMS_INVALID_NAME The processor set contains no online processor.
 * @retval RTEMS_INCORRECT_STATE The processor set is valid, however, the
 *   highest numbered online processor in the specified processor set is not
 *   owned by a scheduler.
 */
rtems_status_code rtems_scheduler_ident_by_processor_set(
  size_t           cpusetsize,
  const cpu_set_t *cpuset,
  rtems_id        *id
);

/**
 * @brief Returns the index of the current processor.
 *
 * In uniprocessor configurations, a value of zero will be returned.
 *
 * In SMP configurations, an architecture specific method is used to obtain the
 * index of the current processor in the system.  The set of processor indices
 * is the range of integers starting with zero up to the processor count minus
 * one.
 *
 * Outside of sections with disabled thread dispatching the current processor
 * index may change after every instruction since the thread may migrate from
 * one processor to another.  Sections with disabled interrupts are sections
 * with thread dispatching disabled.
 *
 * @return The index of the current processor.
 */
#define rtems_scheduler_get_processor() _SMP_Get_current_processor()

/**
 * @brief Returns the processor maximum supported by the system.
 *
 * In uniprocessor configurations, a value of one will be returned.
 *
 * In SMP configurations, this function returns the minimum of the processors
 * (physically or virtually) available by the platform and the configured
 * processor maximum.  Not all processors in the range from processor index
 * zero to the last processor index (which is the processor maximum minus one)
 * may be configured to be used by a scheduler or online (online processors
 * have a scheduler assigned).
 *
 * @return The processor maximum supported by the system.
 *
 * @see rtems_scheduler_add_processor() and rtems_scheduler_remove_processor().
 */
#define rtems_scheduler_get_processor_maximum() _SMP_Get_processor_maximum()

/**
 * @brief Gets the set of processors owned by the specified scheduler instance.
 *
 * @param[in] scheduler_id Identifier of the scheduler instance.
 * @param[in] cpusetsize Size of the specified processor set buffer in
 * bytes.  This value must be positive.
 * @param[out] cpuset The processor set owned by the scheduler.  A set bit in
 * the processor set means that this processor is owned by the scheduler and a
 * cleared bit means the opposite.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a cpuset parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid scheduler instance identifier.
 * @retval RTEMS_INVALID_NUMBER The processor set buffer is too small for the
 * set of processors owned by the scheduler.
 */
rtems_status_code rtems_scheduler_get_processor_set(
  rtems_id   scheduler_id,
  size_t     cpusetsize,
  cpu_set_t *cpuset
);

/**
 * @brief Adds a processor to the set of processors owned by the specified
 * scheduler instance.
 *
 * Must be called from task context.  This operation obtains and releases the
 * objects allocator lock.
 *
 * @param[in] scheduler_id Identifier of the scheduler instance.
 * @param[in] cpu_index Index of the processor to add.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ID Invalid scheduler instance identifier.
 * @retval RTEMS_NOT_CONFIGURED The processor is not configured to be used by
 *   the application.
 * @retval RTEMS_INCORRECT_STATE The processor is configured to be used by
 *   the application, however, it is not online.
 * @retval RTEMS_RESOURCE_IN_USE The processor is already assigned to a
 *   scheduler instance.
 */
rtems_status_code rtems_scheduler_add_processor(
  rtems_id scheduler_id,
  uint32_t cpu_index
);

/**
 * @brief Removes a processor from set of processors owned by the specified
 * scheduler instance.
 *
 * Must be called from task context.  This operation obtains and releases the
 * objects allocator lock.  Removing a processor from a scheduler is a complex
 * operation that involves all tasks of the system.
 *
 * @param[in] scheduler_id Identifier of the scheduler instance.
 * @param[in] cpu_index Index of the processor to add.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ID Invalid scheduler instance identifier.
 * @retval RTEMS_INVALID_NUMBER The processor is not owned by the specified
 *   scheduler instance.
 * @retval RTEMS_RESOURCE_IN_USE The set of processors owned by the specified
 *   scheduler instance would be empty after the processor removal and there
 *   exists a non-idle task that uses this scheduler instance as its home
 *   scheduler instance.
 */
rtems_status_code rtems_scheduler_remove_processor(
  rtems_id scheduler_id,
  uint32_t cpu_index
);

/**
 * @brief Gets the maximum task priority of the specified scheduler instance.
 *
 * @param[in] scheduler_id Identifier of the scheduler instance.
 * @param[out] priority Pointer to a task priority value.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a priority parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid scheduler instance identifier.
 */
rtems_status_code rtems_scheduler_get_maximum_priority(
  rtems_id             scheduler_id,
  rtems_task_priority *priority
);

/**
 * @brief Map a task priority to the corresponding POSIX thread priority.
 *
 * @param scheduler_id Identifier of the scheduler instance.
 * @param priority The task priority to map.
 * @param[out] posix_priority Pointer to a POSIX thread priority value.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a posix_priority parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid scheduler instance identifier.
 * @retval RTEMS_INVALID_PRIORITY Invalid task priority.
 */
rtems_status_code rtems_scheduler_map_priority_to_posix(
  rtems_id             scheduler_id,
  rtems_task_priority  priority,
  int                 *posix_priority
);

/**
 * @brief Map a POSIX thread priority to the corresponding task priority.
 *
 * @param scheduler_id Identifier of the scheduler instance.
 * @param posix_priority The POSIX thread priority to map.
 * @param[out] priority Pointer to a task priority value.
 *
 * @retval RTEMS_SUCCESSFUL Successful operation.
 * @retval RTEMS_INVALID_ADDRESS The @a priority parameter is @c NULL.
 * @retval RTEMS_INVALID_ID Invalid scheduler instance identifier.
 * @retval RTEMS_INVALID_PRIORITY Invalid POSIX thread priority.
 */
rtems_status_code rtems_scheduler_map_priority_from_posix(
  rtems_id             scheduler_id,
  int                  posix_priority,
  rtems_task_priority *priority
);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
