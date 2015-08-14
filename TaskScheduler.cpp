#include "TaskScheduler.h"


// Public Functions
// ============================================================================

TaskScheduler::TaskScheduler()
{
    //zero out the task array, and all the struct-type member variables
    memset(arrayOfDelayedTasks, 0, sizeof(arrayOfDelayedTasks));
    memset(&nextScheduledTask, 0, sizeof(nextScheduledTask));
    memset(&arrayOfEventTasks, 0, sizeof(arrayOfEventTasks));
    idOfNextScheduledTask = 0;
    eventFlags = 0;
    shouldExitWhenPossible = false;


    for(uint8_t i = 0; i < MAX_NUM_DELAYED_TASKS; i++)
    {
        removeScheduledTask(i);
    }
}//end constructor



taskToken_t TaskScheduler::scheduleDelayedTask(pTaskFunc newtask, void* newClientData,
                                          uint32_t delayAmount)
{
    taskToken_t arrPosition = findFreeSpotInDelayedTaskArray();
    if( arrPosition == 0 )
    {
        //no more spots in the task array, or we got passed in a null pointer
        return 0;
    }
    else if(newtask == 0)
    {
        return 0;
    }

    arrayOfDelayedTasks[arrPosition].pTask = newtask;
    arrayOfDelayedTasks[arrPosition].clientData = newClientData;
    arrayOfDelayedTasks[arrPosition].scheduledTime = millis() + delayAmount;

    //Figure out which task is scheduled next
    sync();

    return arrPosition;

}//end scheduleDelayedTask



bool TaskScheduler::removeScheduledTask( taskToken_t taskId )
{
    bool wasDeleteSuccess = false;

    if( taskId < MAX_NUM_DELAYED_TASKS )
    {
        if(arrayOfDelayedTasks[taskId].pTask != NULL)
        {
            //delete the task
            arrayOfDelayedTasks[taskId].pTask = 0;
            arrayOfDelayedTasks[taskId].clientData = 0;
            arrayOfDelayedTasks[taskId].scheduledTime = 0;

            sync();

            wasDeleteSuccess = true;
        }
    }

    return wasDeleteSuccess;
}//end removeScheduledTask




//This function reschedule a task to another time
// Also updates the task handler function and client data
void TaskScheduler::rescheduleDelayedTask (taskToken_t taskId , pTaskFunc newtask, void* newClientData, uint32_t delayAmount)
{
    arrayOfDelayedTasks[taskId].pTask = newtask;
    arrayOfDelayedTasks[taskId].clientData = newClientData;
    arrayOfDelayedTasks[taskId].scheduledTime = millis() + delayAmount;
    sync();
}//end rescheduleDelayedTask




void TaskScheduler::doEventLoop()
{
    shouldExitWhenPossible = false;
    sync();

    while(shouldExitWhenPossible == false)
    {
        uint32_t currentTime = millis();

        // Check if it is time to handle the next delayed task.
        if( currentTime >= nextScheduledTask.scheduledTime)
        {
            //Delete the task from the array.
            // Do this before calling the task, because the task   
            // may wish to add itself back on the task list.
            arrayOfDelayedTasks[idOfNextScheduledTask].pTask = 0;
            arrayOfDelayedTasks[idOfNextScheduledTask].clientData = 0;
            arrayOfDelayedTasks[idOfNextScheduledTask].scheduledTime = 0;
			
            //call the task
            (nextScheduledTask.pTask)(nextScheduledTask.clientData);

            sync();
        }
        else
        {
            //it's not yet time to do the next task,
            //do nothing
        }


        // Check if any event was triggered and needs to be handled
        if( eventFlags > 0)
        {
            // At least one event is set
            handleAllTriggeredEvents();
        }
        else
        {
            //No events have happened
            //Do nothing
        }
    }//end while

}//end doEventLoop



void TaskScheduler::exit()
{
    shouldExitWhenPossible = true;
}//end exit



eventId_t TaskScheduler::createEventTrigger(pTaskFunc eventHandler)
{
    uint8_t arrIndex = 0;

    // Find the next free spot in the array. Returns zero is no free spot is found.
    arrIndex = findFreeSpotInEventTaskArray();

    if( arrIndex != 0 )
    {
        // Free spot was found, save the event Handler function.
        arrayOfEventTasks[arrIndex].pTask = eventHandler;
    }
    else
    {
        //no free spot found. Return 0.
    }

    return arrIndex;
}//end createEventTrigger




void TaskScheduler::triggerEvent(eventId_t eventId, void* clientData)
{
    // Make sure this event has a handler
    if(arrayOfEventTasks[eventId].pTask != NULL)
    {
        //set the flag for this event to indicate it has happened and should be handled
        eventFlags = eventFlags | (1 << eventId);

        //copy over the client data
        arrayOfEventTasks[eventId].clientData = clientData;

        //Serial.println("Event flag set for the trigger event"); delay(20);
    }
    else
    {
        //Serial.println("pTask is null! Event not set"); delay(20);
        // Trying to trigger an event that does not exist.
        // Do nothing.
    }
}//end triggerEvent



void TaskScheduler::deleteEventTrigger(eventId_t eventId)
{
    //clear out the handler from the even task array
    arrayOfEventTasks[eventId].pTask = NULL;

    //Clear the task flag in case this event is trigger and waiting to be handled
    eventFlags &= ~(1 << eventId);
}//end deleteEventTrigger





// Private Functions
// ============================================================================

taskToken_t TaskScheduler::findFreeSpotInDelayedTaskArray()
{
    taskToken_t positionToReturn = 0;
    // Position 0 is unused. It indicates an error.

    for( uint8_t i = 1; i < MAX_NUM_DELAYED_TASKS; i++)
    {
        if(arrayOfDelayedTasks[i].pTask == NULL)
        {
            //found an empty position
            positionToReturn = i;
            break;
        }
        else
        {
            //Serial.print("Position "); //debug
            //Serial.print(i); //debug
            //Serial.print(" is occupied because pTask = "); //debug
            //Serial.println((uint16_t)(arrayOfDelayedTasks[i].pTask)); //debug
            //this array position is occupied
        }
    }
    return positionToReturn;
}//end findFreeSpotInDelayedTaskArray




taskToken_t TaskScheduler::findFreeSpotInEventTaskArray()
{
    taskToken_t positionToReturn = 0;
    // Position 0 is unused. It indicates an error.
    for( uint8_t i = 1; i < MAX_NUM_EVENT_TASKS; i++)
    {
        if(arrayOfEventTasks[i].pTask == NULL)
        {
            //found an empty position
            positionToReturn = i;
            break;
        }
        else
        {
            //this array position is occupied
        }
    }
    return positionToReturn;
}//findFreeSpotInEventTaskArray



void TaskScheduler::handleAllTriggeredEvents()
{
    uint32_t mask = 0;
    //todo: should be MAX_NUM_EVENT_TASKS + 1
    for( uint8_t i = 1; i < MAX_NUM_EVENT_TASKS; i++)
    {
        mask = (1 << i);
        // If this flag is set
        if( (eventFlags & mask) > 0)
        {
            //clear the flag
            eventFlags &= ~(mask);

            //call the handler function
            (arrayOfEventTasks[i].pTask)(arrayOfEventTasks[i].clientData);
        }
        else
        {
           // This flag is not set. Go on to the next one.
        }
    }
}//end handleAllTriggeredEvents




void TaskScheduler::sync()
{
    //find the task with the earliest scheduled time
    uint32_t earliestTime = -1; // the largest value

    // Position 0 is unused. It indicates an error.
    for( taskToken_t i = 1; i < MAX_NUM_DELAYED_TASKS; i++)
    {
        if( (arrayOfDelayedTasks[i].pTask != NULL) && //this array position is occupied
            (arrayOfDelayedTasks[i].scheduledTime < earliestTime) ) //and the time is earlier
        {
            earliestTime = arrayOfDelayedTasks[i].scheduledTime;
            idOfNextScheduledTask = i;
            nextScheduledTask.pTask = arrayOfDelayedTasks[i].pTask;
            nextScheduledTask.clientData = arrayOfDelayedTasks[i].clientData;
            nextScheduledTask.scheduledTime = arrayOfDelayedTasks[i].scheduledTime;
        }
    }

    //TODO: handle the case when no delayed tasks are scheduled
}//end sync
