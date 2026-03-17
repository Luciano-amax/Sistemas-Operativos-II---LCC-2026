/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2007-2009 Universidad de Las Palmas de Gran Canaria.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "thread_test_simple.hh"
#include "system.hh"
#include "strings.h"
#include "semaphore.hh"

#include <stdio.h>
#include <string.h>

// Ejercicio 14: generar cinco hilos
#define NUM_THREADS 5

// Ejercicio 15: usar un semáforo con contador inicial 3
#define SEMAPHORE_TEST
Semaphore sem15("semEj15", 3);

/// Loop 10 times, yielding the CPU to another ready thread each iteration.
///
/// * `name` points to a string with a thread name, just for debugging
///   purposes.

bool threadDone[NUM_THREADS];

static char threadNames[NUM_THREADS][16];

static inline bool
AllDone()
{
    for (int i = 0; i < NUM_THREADS; i++) {
        if (!threadDone[i]) {
            return false;
        }
    }
    return true;
}

void
SimpleThread(void *id_)
{
    // If the lines dealing with interrupts are commented, the code will
    // behave incorrectly, because printf execution may cause race
    // conditions.

    int id = *(int *) id_;

    for (unsigned num = 0; num < 10; num++) {
#ifdef SEMAPHORE_TEST
        sem15.P();
        DEBUG('s', "Thread `%s` acquired semaphore\n", currentThread->GetName());
#endif
        printf("*** Thread `%s` is running: iteration %u\n",
               currentThread->GetName(), num);
#ifdef SEMAPHORE_TEST
        DEBUG('s', "Thread `%s` releasing semaphore\n", currentThread->GetName());
        sem15.V();
#else
        currentThread->Yield();
#endif
    }
    threadDone[id] = true;
    printf("!!! Thread `%s` has finished SimpleThread\n", currentThread->GetName());
}

/// Set up a ping-pong between several threads.
///
/// Do it by launching one thread which calls `SimpleThread`, and finally
/// calling `SimpleThread` on the current thread.
void
ThreadTestSimple()
{
    Thread *threads[NUM_THREADS - 1];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        snprintf(threadNames[i], sizeof threadNames[i], "worker%d", i + 1);
    }

    for (int i = 0; i < NUM_THREADS - 1; i++) {
        threads[i] = new Thread(threadNames[i + 1]);
        threads[i]->Fork(SimpleThread, &ids[i + 1]);
    }

    //the "main" thread also executes the same function
    SimpleThread(&ids[0]);

    //Wait for the 2nd thread to finish if needed
    while (!AllDone()) {
        currentThread->Yield();
    }
    printf("Test finished\n");
}
