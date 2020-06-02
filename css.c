#include <stdbool.h>            // for data type bool
#include <stdio.h>              // for puts(), fprintf(), printf(), scanf()
#include <stdlib.h>             // for malloc(), calloc(), free()
#include <string.h>             // for strcspn(), strlen()


#define NAME_SIZE 200
#define TIME_QUANTUM_DEFAULT 10


unsigned int pid = 0;
unsigned int burstTime = 0;
unsigned int arrivalTime = 0;

typedef struct ProcessS Process;
struct ProcessS {
  unsigned int pid; // PID: pid++ for every new proccess.
  char name[NAME_SIZE]; // NAME
  bool priority; // PRI: 0 for priority queue; 1 for nonpriority queue.
  bool priorityOriginal; // PRI_O: for keeping track of priority input.
  unsigned int arrivalTime; // AT: ++arrivalTime for every process run.
  int burstTime; // BT: remaining burst time.
  unsigned int burstTimeOriginal; // BT_0
  Process *next;
  Process *previous; // For moving/freeing individual process on queue.
};


void createProcess(
                   Process **priorityQueueHead
                   , Process **nonPriorityQueueHead
                   , Process **priorityQueueTail
                   , Process **nonPriorityQueueTail
                   );

void insertProcess(
                   Process *newProcess
                   , Process ***queueHead
                   , Process ***queueTail
                   );

void runNextProcess(
                    Process **priorityQueueHead
                    , Process **priorityQueueTail
                    , Process **nonPriorityQueueHead
                    , Process **nonPriorityQueueTail
                    );

void runNextProcessForReal(
                           Process ***queueHead
                           , Process ***queueTail
                           , Process ***nonPriorityQueueHead
                           , Process ***nonPriorityQueueTail
                           );

void moveProcess(
                 Process **priorityQueueHead
                 , Process **priorityQueueTail
                 , Process **nonPriorityQueueHead
                 , Process **nonPriorityQueueTail
                 );

Process *getProcess(
                    Process **priorityQueueHead
                    , Process **nonPriorityQueueHead
                    );

Process *findProcess(
                     Process **queueHead
                     , unsigned int pid
                     );

void moveProcessToQueueTail(
                            Process *process
                            , unsigned int newPriority
                            , Process ***newQueueHead
                            , Process ***newQueueTail
                            , Process ***oldQueueHead
                            , Process ***oldQueueTail
                            );

void moveProcessToNonPriorityQueueTail(
                                       Process *process
                                       , Process ****nonPriorityQueueHead
                                       , Process ****nonPriorityQueueTail
                                       );


void moveProcessToQueueTailForReal(Process *process
                                   , unsigned int newPriority
                                   , Process ****queueHead
                                   , Process ****ququeTail
                                   );

void printMenu(void);

void printProcessQueues(
                        Process *priorityQueueHead
                        , Process *nonPriorityQueueHead
                        );

void printProcesses(
                    Process *processQueueHead
                    , bool queueClass
                    );

void freeProcessQueue(
                      Process *processQueueHead
                      );

void getLine(
             char *buffer
             , int bufferSize
             );


int main (void) {

  unsigned short int option;

  Process *priorityQueueHead = NULL;
  Process **priorityQueueHeadPointer1 = &priorityQueueHead;

  Process *nonPriorityQueueHead = NULL;
  Process **nonPriorityQueueHeadPointer1 = &nonPriorityQueueHead;

  Process *priorityQueueTail = NULL;
  Process **priorityQueueTailPointer1 = &priorityQueueTail;

  Process *nonPriorityQueueTail = NULL;
  Process **nonPriorityQueueTailPointer1 = &nonPriorityQueueTail;

  do {
    printMenu();

    printf("\nEnter an option:\n");
    scanf("%hu", &option);

    switch (option) {
    case 1:
      createProcess(
                    priorityQueueHeadPointer1
                    , priorityQueueTailPointer1
                    , nonPriorityQueueHeadPointer1
                    , nonPriorityQueueTailPointer1
                    );
      break;

    case 2:
      if (priorityQueueHead == NULL && nonPriorityQueueHead == NULL) {
        puts("Error: none process to move. Try adding some process first.");
        break;
      }
      else {
        moveProcess(
                    priorityQueueHeadPointer1
                    , priorityQueueTailPointer1
                    , nonPriorityQueueHeadPointer1
                    , nonPriorityQueueTailPointer1
                    );
        break;
      }

    case 3:
      if (priorityQueueHead == NULL && nonPriorityQueueHead == NULL) {
        puts("Error: none process to run. Try adding some process first.");
        break;
      }
      else {
        runNextProcess(
                       priorityQueueHeadPointer1
                       , priorityQueueTailPointer1
                       , nonPriorityQueueHeadPointer1
                       , nonPriorityQueueTailPointer1
                       );
        break;
      }

    case 4:
      printProcessQueues(
                         priorityQueueHead
                         , nonPriorityQueueHead
                         );
      break;

    default:
      if (option != 0 )
        puts("Error: invalid option! Try again.");
    } // end switch
  } while (option != 0); // end do while

  freeProcessQueue(priorityQueueHead);
  freeProcessQueue(nonPriorityQueueHead);

  return 0;

} // end main()


/* Define and insert process on choosen queue */
void createProcess(
                   Process **priorityQueueHead
                   , Process **priorityQueueTail
                   , Process **nonPriorityQueueHead
                   , Process **nonPriorityQueueTail
                   ) {

  Process ***priorityQueueHeadPointer2 = &priorityQueueHead;
  Process ***nonPriorityQueueHeadPointer2 = &nonPriorityQueueHead;
  Process ***priorityQueueTailPointer2 = &priorityQueueTail;
  Process ***nonPriorityQueueTailPointer2 = &nonPriorityQueueTail;

  Process *newProcess = NULL;

  unsigned short int tempPriority; // To scanf() bool.

  /* Allocate new process */
  newProcess = (Process *) malloc(sizeof (Process));
  if (newProcess == NULL) {
    fprintf(stderr, "malloc() failed allocating memory\n");
    exit(EXIT_FAILURE);
  }

  /* Fill:
   * pid, name, burstTime, burstTimeOriginal,
   * arrivalTime, next, previous.
   */
  newProcess->pid = pid++;

  puts("Enter process' name:");
  getLine(newProcess->name, NAME_SIZE);

  puts("Enter process' time burst:");
  scanf("%u", &newProcess->burstTime);
  newProcess->burstTimeOriginal = newProcess->burstTime;

  newProcess->arrivalTime = arrivalTime;

  newProcess->next = NULL;
  newProcess->previous = NULL;

  /* Fill: priority */
  do {
    puts("Enter [0] for priority process, "
         "or [1] for nonpriority process:");
    scanf("%hu", &tempPriority);

    if (tempPriority != 0 && tempPriority != 1)
      puts("Error: priority must be 0 or 1. Try again.");
  } while (tempPriority != 0 && tempPriority != 1);
  newProcess->priority = tempPriority;
  newProcess->priorityOriginal = tempPriority;

  /* Insert priority process into priority queue */
  if (newProcess->priority == 0) {
    insertProcess(
                  newProcess
                  , priorityQueueHeadPointer2
                  , priorityQueueTailPointer2
                  );
  }
  /* Insert nonpriority process into nonpriority queue */
  else {
    insertProcess(
                  newProcess
                  , nonPriorityQueueHeadPointer2
                  , nonPriorityQueueTailPointer2
                  );
  }

} // end createProcess()


void insertProcess(
                   Process *newProcess
                   , Process ***queueHead
                   , Process ***queueTail
                   ) {

  if (**queueHead == NULL)
    **queueHead = newProcess;
  else {
    newProcess->previous = **queueTail;
    (**queueTail)->next = newProcess;
  }

  /* Save queue's tail */
  (**queueTail) = newProcess;

} // end insertProcess()


void runNextProcess(
                    Process **priorityQueueHead
                    , Process **priorityQueueTail
                    , Process **nonPriorityQueueHead
                    , Process **nonPriorityQueueTail
                    ) {

  Process ***priorityQueueHeadPointer2 = &priorityQueueHead;
  Process ***priorityQueueTailPointer2 = &priorityQueueTail;
  Process ***nonPriorityQueueHeadPointer2 = &nonPriorityQueueHead;
  Process ***nonPriorityQueueTailPointer2 = &nonPriorityQueueTail;

  /* Remove process from queue when it's finished */
  /* IF priority queue IS NOT empty, THEN run process at its head */
  if ((*priorityQueueHead) != NULL) {
    runNextProcessForReal(
                          priorityQueueHeadPointer2
                          , priorityQueueTailPointer2
                          , nonPriorityQueueHeadPointer2
                          , nonPriorityQueueTailPointer2
                          );
  } // end if
  /* ELSE IF nonpriority queue IS NOT empty, THEN run process at its head */
  else if ((*nonPriorityQueueHead) != NULL ) {
    runNextProcessForReal(
                          nonPriorityQueueHeadPointer2
                          , nonPriorityQueueTailPointer2
                          , nonPriorityQueueHeadPointer2
                          , nonPriorityQueueTailPointer2
                          );
  } // end else if

} // end runNextProcess()


/* Run the process, but now for real
 * (I'm scared of the quadruple indirection down there btw)
 */
void runNextProcessForReal(
                           Process ***queueHead
                           , Process ***queueTail
                           , Process ***nonPriorityQueueHead
                           , Process ***nonPriorityQueueTail
                           ) {

  Process ****nonPriorityQueueHeadPointer3 = &nonPriorityQueueHead;
  Process ****nonPriorityQueueTailPointer3 = &nonPriorityQueueTail;

  /* Get process at queue's head */
  Process *process = **queueHead;

  process->burstTime -= TIME_QUANTUM_DEFAULT;

  /* IF process HAS NO burstTime left */
  if (process->burstTime <= 0) {
    /* iF it IS the only process in the queue */
    if (
        process->previous == NULL
        && process->next == NULL
        ) {
      /* REMOVE queue's head and tail reference to process. */
      **queueHead = NULL;
      **queueTail = NULL;

      /* FREE process. */
      free(process);
    } // end if
    /* ELSE there IS 2+ processes in the queue */
    else {
      /* MOVE 2nd process TO queue's head,
       * AND REMOVE its reference TO 1st process
       */
      **queueHead = process->next;
      process->next->previous = NULL;

      /* FREE process */
      free(process);
    } // end else
  } // end if
  /* ELSE process HAS burstTime left */
  else {
    /* IF it IS the only process in the queue */
    if (
        process->previous == NULL
        && process->next == NULL
        ) {
      /* REMOVE queue's head and tail reference to process. */
      **queueHead = NULL;
      **queueTail = NULL;

      moveProcessToNonPriorityQueueTail(
                                        process
                                        , nonPriorityQueueHeadPointer3
                                        , nonPriorityQueueTailPointer3
                                        );
    } // end if
      /* ELSE there IS 2+ processes in the queue */
    else {
      /* REMOVE process from queue's head */
      **queueHead = process->next;
      process->next->previous = NULL;

      moveProcessToNonPriorityQueueTail(
                                        process
                                        , nonPriorityQueueHeadPointer3
                                        , nonPriorityQueueTailPointer3
                                        );
    } // end else
  } // end else

  ++arrivalTime;

} // runNextProcessForReal end


void moveProcessToNonPriorityQueueTail(
                                       Process *process
                                       , Process ****nonPriorityQueueHead
                                       , Process ****nonPriorityQueueTail
                                       ) {

  /* Remove previous and next links (old queue) from process. */
  process->previous = NULL;
  process->next = NULL;

  /* Change ex-priority process' priority to nonpriority. */
  if (process->priority == 0)
    process->priority = 1;

  /* IF there IS NO process in the nonpriority queue */
  if (***nonPriorityQueueHead == NULL && ***nonPriorityQueueTail == NULL) {
    /* Insert process AT nonpriority queue's head,
     * referencing nonpriority queue's head and tail TO process.
     */
    ***nonPriorityQueueHead = process;
    ***nonPriorityQueueTail = process;
  }
  /* ELSE there IS process in the nonpriority queue */
  else {
    /* MOVE process TO nonpriority queue's tail. */
    process->previous = (***nonPriorityQueueTail);
    process->next = NULL;

    (***nonPriorityQueueTail)->next = process;
    ***nonPriorityQueueTail = process;
  }

} // end moveProcessToNonPriorityQueueTail()


void moveProcess(
                 Process **priorityQueueHead
                 , Process **priorityQueueTail
                 , Process **nonPriorityQueueHead
                 , Process **nonPriorityQueueTail
                 ) {

  Process ***priorityQueueHeadPointer2 = &priorityQueueHead;
  Process ***priorityQueueTailPointer2 = &priorityQueueTail;
  Process ***nonPriorityQueueHeadPointer2 = &nonPriorityQueueHead;
  Process ***nonPriorityQueueTailPointer2 = &nonPriorityQueueTail;

  Process *process = NULL;
  unsigned short int newPriority;

  process = getProcess(priorityQueueHead, nonPriorityQueueHead);

  /* Input: newPriority */
  do {
    puts("Enter [0] to move process to priority queue's tail"
         ", or [1] to move process to nonpriority queue's tail.");
    scanf("%hu", &newPriority);

    if (newPriority != 0 && newPriority != 1)
      puts("Error: invalid option. Try again!");
  } while (newPriority != 0 && newPriority != 1);

  /* Move process to priority queue's tail. */
  if (newPriority == 0) {
    /* From priority queue to priority queue's tail. */
    if (process->priority == 0) {
      moveProcessToQueueTail(
                             process
                             , newPriority
                             , priorityQueueHeadPointer2
                             , priorityQueueTailPointer2
                             , priorityQueueHeadPointer2
                             , priorityQueueTailPointer2
                             );
    }
    else if (process->priority == 1) {
      /* From nonpriority queue to priority queue's tail. */
      moveProcessToQueueTail(
                             process
                             , newPriority
                             , priorityQueueHeadPointer2
                             , priorityQueueTailPointer2
                             , nonPriorityQueueHeadPointer2
                             , nonPriorityQueueTailPointer2
                             );
    }
  }
  /* Move process to nonpriority queue's tail. */
  else if (newPriority == 1) {
    /* From nonpriority queue to nonpriority queue's tail. */
    if (process->priority == 1) {
      moveProcessToQueueTail(
                             process
                             , newPriority
                             , nonPriorityQueueHeadPointer2
                             , nonPriorityQueueTailPointer2
                             , nonPriorityQueueHeadPointer2
                             , nonPriorityQueueTailPointer2
                             );
    }
    /* From priority queue to nonpriority queue's tail. */
    else if (process->priority == 0){
      moveProcessToQueueTail(
                             process
                             , newPriority
                             , nonPriorityQueueHeadPointer2
                             , nonPriorityQueueTailPointer2
                             , priorityQueueHeadPointer2
                             , priorityQueueTailPointer2
                             );
    }
  }

} // end moveProcess()


Process *getProcess(
                    Process **priorityQueueHead
                    , Process **nonPriorityQueueHead
                    ) {

  Process *process = NULL;
  unsigned int pid;

  do {
    puts("Enter PID:");
    scanf("%u", &pid);

    if ((process = findProcess(priorityQueueHead, pid)) != NULL)
      return process;
    else {
      if ((process = findProcess(nonPriorityQueueHead, pid)) != NULL)
        return process;
      else
        printf("Error: PID %u not found. Try again!\n", pid);
    }
  } while (process == NULL);

  return process;

} // end getProcess()


Process *findProcess(
                     Process **queueHead
                     , unsigned int pid
                     ) {

  Process *queueHeadTemp = *queueHead;

  while(queueHeadTemp != NULL) {
    if ((queueHeadTemp)->pid == pid) {
      pid = true; // ?
      return queueHeadTemp;
    }
    else {
      queueHeadTemp = (queueHeadTemp)->next;
    }
  }

  return NULL;

} // end findProcess()


void moveProcessToQueueTail(
                            Process *process
                            , unsigned int newPriority
                            , Process ***newQueueHead
                            , Process ***newQueueTail
                            , Process ***oldQueueHead
                            , Process ***oldQueueTail
                            ) {

  Process ****newQueueHeadPointer3 = &newQueueHead;
  Process ****newQueueTailPointer3 = &newQueueTail;

  /*  IF process is in the queue's head */
  if (process->previous == NULL) {

    /* IF process has no next process (IS the only process in the queue) */
    if (process->next == NULL) {
      /* REMOVE queue's head and tail reference to process. */
      **oldQueueHead = NULL;
      **oldQueueTail = NULL;

      moveProcessToQueueTailForReal(
                                    process
                                    , newPriority
                                    , newQueueHeadPointer3
                                    , newQueueTailPointer3
                                    );
    }

    /* IF process has next process */
    else if (process->next != NULL) {
      /* REMOVE process from queue's head */
      **oldQueueHead = process->next;
      process->next->previous = NULL;

      moveProcessToQueueTailForReal(
                                    process
                                    , newPriority
                                    , newQueueHeadPointer3
                                    , newQueueTailPointer3
                                    );
    }

  } // end if

  /* ELSE process IS NOT in the queue's head */
  else {

    /* IF process IS in the queue's tail */
    if (process->next == NULL) {
      /* REMOVE process from queue's tail */
      **oldQueueTail = process->previous;
      process->previous->next = NULL;

      moveProcessToQueueTailForReal(
                                    process
                                    , newPriority
                                    , newQueueHeadPointer3
                                    , newQueueTailPointer3
                                    );
    }

    /* ELSE process IS between 2 processes */
    else {
      process->previous->next = process->next;
      process->next->previous = process->previous;

      moveProcessToQueueTailForReal(
                                    process
                                    , newPriority
                                    , newQueueHeadPointer3
                                    , newQueueTailPointer3
                                    );
    }

  } // end  else

} // end moveProcessToQueueTail()


void moveProcessToQueueTailForReal(
                                   Process *process
                                   , unsigned int newPriority
                                   , Process ****queueHead
                                   , Process ****queueTail
                                   ) {

  /* Remove previous and next links (old queue) from process. */
  process->previous = NULL;
  process->next = NULL;

  /* Change process priority to choosen priority. */
  process->priority = newPriority;

  /* IF there IS NO process in the queue */
  if (***queueHead == NULL && ***queueTail == NULL) {
    /* Insert process AT queue's head,
     * referencing queue's head and tail TO process.
     */
    ***queueHead = process;
    ***queueTail = process;
  }
  /* ELSE there IS process in the queue */
  else {
    /* MOVE process TO queue's tail. */
    process->previous = (***queueTail);
    process->next = NULL;

    (***queueTail)->next = process;
    ***queueTail = process;
  }

} // end moveProcessToQueueTailForReal()


void printMenu(void) {
  puts("+-------------------------------+");
  puts("|              MENU             |");
  puts("+--------+----------------------+");
  puts("| Option | Description          |");
  puts("+--------+----------------------+");
  puts("|      1 | Create new process   |");
  puts("+--------+----------------------+");
  puts("|      2 | Move prcess          |");
  puts("+--------+----------------------+");
  puts("|      3 | Run next process     |");
  puts("+--------+----------------------+");
  puts("|      4 | Print process queues |");
  puts("+--------+----------------------+");
  puts("|      0 | Quit                 |");
  puts("+--------+----------------------+");
}


void printProcessQueues(Process *priorityQueueHead,
                        Process *nonPriorityQueueHead) {

  printProcesses(priorityQueueHead, 0);
  printProcesses(nonPriorityQueueHead, 1);

} // end printProcessQueues()


void printProcesses(Process *processQueueHead, bool queueClass) {

  if (queueClass == 0)
    printf("\n\t\t\t\t\tPRIORITY PROCESS QUEUE\n");
  else
    printf("\n\t\t\t\t\tNONPRIORITY PROCESS QUEUE\n");

  printf("PID\t\t" "PRI\t\t" "PRI_O\t\t" "AT\t\t"
         "BT\t\t" "BT_O\t\t" "NAME\n");

  while (processQueueHead != NULL) {
    printf("%i\t\t", processQueueHead->pid);
    printf("%i\t\t", processQueueHead->priority);
    printf("%i\t\t", processQueueHead->priorityOriginal);
    printf("%i\t\t", processQueueHead->arrivalTime);
    printf("%i\t\t", processQueueHead->burstTime);
    printf("%i\t\t", processQueueHead->burstTimeOriginal);
    printf("%s\t\t", processQueueHead->name);
    puts("");
    processQueueHead = processQueueHead->next;
  }
  puts("");

}


void freeProcessQueue(Process *processQueueHead) {
  Process *currentProcess;

  while(processQueueHead != NULL) {
    currentProcess = processQueueHead;
    processQueueHead = processQueueHead->next;
    free(currentProcess);
  }

} // end freeProcessQueue()


/* https://stackoverflow.com/a/9458609 */
void getLine(char *buffer, int bufferSize) {

  int ch, len;

  /* https://www.go4expert.com/articles/solution-using-scanf-fgets-c-t27148 */
  getchar();

  fgets(buffer, bufferSize, stdin);

  /* Remove unwanted characters from the buffer. */
  buffer[strcspn(buffer, "\r\n")] = '\0';

  len = strlen(buffer);

  /* Clean input buffer if needed. */
  if (len == bufferSize - 1)
    while ((ch = getchar()) != '\n' && ch != EOF);

  /* return len; */

} // end getLine()
