#ifndef TASKSCHEDULER_HEADERGUARD
#define TASKSCHEDULER_HEADERGUARD

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif

#include <inttypes.h>

typedef uint8_t taskToken_t;
typedef void(*pTaskFunc)(void*);
typedef uint8_t eventId_t;

//#define MAX_NUM_DELAYED_TASKS 31
//#define MAX_NUM_EVENT_TASKS 31
#define MAX_NUM_DELAYED_TASKS 8
#define MAX_NUM_EVENT_TASKS 2
// We allocate less slots for tasks and events because the attiny85 has less ram.

class TaskScheduler
{
public:

    TaskScheduler();
    taskToken_t scheduleDelayedTask(pTaskFunc newtask, void* newClientData, uint32_t delayAmount);
    bool removeScheduledTask( taskToken_t taskId );
    void rescheduleDelayedTask (taskToken_t taskId , pTaskFunc newtask, void* newClientData, uint32_t delayAmount);
    void doEventLoop();
    void exit();

    eventId_t createEventTrigger(pTaskFunc eventHandler);
        // Creates a 'trigger' for an event, which - if it occurs - will be handled (from the event loop) using "eventHandlerProc".
        // (Returns 0 iff no such trigger can be created (e.g., because of implementation limits on the number of triggers).)

    void triggerEvent(eventId_t eventTriggerId, void* clientData = NULL);
        // Causes the (previously-registered) handler function for the specified event to be handled (from the event loop).
        // The handler function is called with "clientData" as parameter.
        // Note: This function (unlike other library functions) may be called from an external thread - to signal an external event.

    void deleteEventTrigger(eventId_t eventId);
        // Deletes an existing event trigger. If the event being deleted is triggered and awaiting to be handled it will not be
        // handled.



private:

    // Private functions
    taskToken_t findFreeSpotInDelayedTaskArray();
    taskToken_t findFreeSpotInEventTaskArray();
    void handleAllTriggeredEvents();
    void sync();


    // Private types
    struct delayedTaskEntry_t
    {
        pTaskFunc pTask;
        void* clientData;
        uint32_t scheduledTime;
    };

    struct eventTaskEntry_t
    {
        pTaskFunc pTask;
        void* clientData;
    };


    // Private variables
    delayedTaskEntry_t arrayOfDelayedTasks[MAX_NUM_DELAYED_TASKS];
    delayedTaskEntry_t nextScheduledTask;
    taskToken_t idOfNextScheduledTask;
    bool shouldExitWhenPossible;   // set to true when a task Scheduler exit was requested

    eventTaskEntry_t arrayOfEventTasks[MAX_NUM_EVENT_TASKS];
    uint32_t eventFlags; // A set flag means the even happened and is pending to be handled

};//end class TaskScheduler



#endif //TASKSCHEDULER_HEADERGUARD
