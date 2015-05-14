#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define dabort() \
{  \
    printf("Aborting at line %d in source file %s\n",__LINE__,__FILE__); \
    abort(); \
}

/*
** This set of queue functions was originally thread-aware.  I
** redesigned the code to make this set of queue routines
** thread-ignorant (just a generic, boring yet very fast set of queue
** routines).  Why the change?  Because it makes more sense to have
** the thread support as an optional add-on.  Consider a situation
** where you want to add 5 nodes to the queue.  With the
** thread-enabled version, each call to queue_put() would
** automatically lock and unlock the queue mutex 5 times -- that's a
** lot of unnecessary overhead.  However, by moving the thread stuff
** out of the queue routines, the caller can lock the mutex once at
** the beginning, then insert 5 items, and then unlock at the end.
** Moving the lock/unlock code out of the queue functions allows for
** optimizations that aren't possible otherwise.  It also makes this
** code useful for non-threaded applications.
**
** We can easily thread-enable this data structure by using the
** data_control type defined in control.c and control.h.  */
typedef struct node
{
    struct node *next;
} node;

typedef struct queue
{
    node *head, *tail;
} queue;

void queue_init(queue *myroot) 
{
	myroot->head=NULL;
	myroot->tail=NULL;
}

void queue_put(queue *myroot,node *mynode) 
{
	mynode->next=NULL;
	if (myroot->tail!=NULL)
		myroot->tail->next=mynode;
	myroot->tail=mynode;
	if (myroot->head==NULL)
	    myroot->head=mynode;
}

node *queue_get(queue *myroot) 
{
	//get from root
	node *mynode;
	mynode=myroot->head;
	if (myroot->head!=NULL)
		myroot->head=myroot->head->next;
	return mynode;
}


/*
** These routines provide an easy way to make any type of
** data-structure thread-aware.  Simply associate a data_control
** structure with the data structure (by creating a new struct, for example).
** Then, simply lock and unlock the mutex, or 
** wait/signal/broadcast on the condition variable in the data_control
** structure as needed.
**
** data_control structs contain an int called "active".  This int is
** intended to be used for a specific kind of multithreaded design,
** where each thread checks the state of "active" every time it locks
** the mutex.  If active is 0, the thread knows that instead of doing
** its normal routine, it should stop itself.  If active is 1, it
** should continue as normal.  So, by setting active to 0, a
** controlling thread can easily inform a thread work crew to shut
** down instead of processing new jobs.  Use the control_activate()
** and control_deactivate() functions, which will also broadcast on
** the data_control struct's condition variable, so that all threads
** stuck in pthread_cond_wait() will wake up, have an opportunity to
** notice the change, and then terminate.
*/
typedef struct data_control
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int active;
} data_control;

int control_init(data_control *mycontrol) 
{
	int mystatus;
	if (pthread_mutex_init(&(mycontrol->mutex),NULL))
		return 1;
	if (pthread_cond_init(&(mycontrol->cond),NULL))
	    return 1;
	mycontrol->active=0;
	return 0;
}

int control_destroy(data_control *mycontrol) 
{
	int mystatus;
	if (pthread_cond_destroy(&(mycontrol->cond)))
		return 1;
	if (pthread_mutex_destroy(&(mycontrol->mutex)))
	    return 1;
	mycontrol->active=0;
	return 0;
}

int control_activate(data_control *mycontrol) 
{
	int mystatus;
	if (pthread_mutex_lock(&(mycontrol->mutex)))
		return 0;
	mycontrol->active=1;
	pthread_mutex_unlock(&(mycontrol->mutex));
	pthread_cond_broadcast(&(mycontrol->cond));
	return 1;
}

int control_deactivate(data_control *mycontrol) 
{
	int mystatus;
	if (pthread_mutex_lock(&(mycontrol->mutex)))
		return 0;
	mycontrol->active=0;
	pthread_mutex_unlock(&(mycontrol->mutex));
	pthread_cond_broadcast(&(mycontrol->cond));
	return 1;
}

/* the work_queue holds tasks for the various worker threads to complete. */
struct work_queue 
{
	data_control control;
	queue work;
} wq;

/* 
 * I added a job number to the work node.  Normally, the work node
 * would contain additional data that needed to be processed. 
 */
typedef struct work_node 
{
	struct node *next;
	int jobnum;
} wnode;

/* the cleanup queue holds stopped threads.  Before a thread
 * terminates, it adds itself to this list.  Since the main thread is
 * waiting for changes in this list, it will then wake up and clean up
 * the newly terminated thread. 
 */
struct cleanup_queue 
{
	data_control control;
	queue cleanup;
} cq;

/* I added a thread number (for debugging/instructional purposes) and
 * a thread id to the cleanup node.  The cleanup node gets passed to
 * the new thread on startup, and just before the thread stops, it
 * attaches the cleanup node to the cleanup queue.  The main thread
 * monitors the cleanup queue and is the one that performs the
 * necessary cleanup. 
 */
typedef struct cleanup_node 
{
	struct node *next;
	int threadnum;
	pthread_t tid;
} cnode;

void *threadfunc(void *myarg) 
{
	wnode *mywork;
	cnode *mynode;

	mynode=(cnode *) myarg;

	pthread_mutex_lock(&wq.control.mutex);
	while (wq.control.active) 
	{
		while (wq.work.head==NULL && wq.control.active) 
		{
			pthread_cond_wait(&wq.control.cond, &wq.control.mutex);
		}
		if (!wq.control.active) 
			break;

		//we got something!
		mywork=(wnode *)queue_get(&wq.work);
		pthread_mutex_unlock(&wq.control.mutex);

		//perform processing...
		printf("Thread number %d processing job %d\n",mynode->threadnum,mywork->jobnum);
		free(mywork);
		pthread_mutex_lock(&wq.control.mutex);
	}
	pthread_mutex_unlock(&wq.control.mutex);
	
	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *) mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	printf("thread %d shutting down...\n",mynode->threadnum);
	return NULL;
}

#define NUM_WORKERS 4
int numthreads;

void join_threads(void) 
{
	cnode *curnode;
	printf("joining threads...\n");
	while (numthreads) 
	{
		pthread_mutex_lock(&cq.control.mutex);

	   /* below, we sleep until there really is a new cleanup node.  
		* This takes care of any false wakeups... even if we break out of
		* pthread_cond_wait(), we don't make any assumptions that the
	    * condition we were waiting for is true.  
		*/
		while (cq.cleanup.head==NULL) 
		{
			pthread_cond_wait(&cq.control.cond,&cq.control.mutex);
		}

		/* at this point, we hold the mutex and there is an item in the
		   list that we need to process.  First, we remove the node from
		   the queue.  Then, we call pthread_join() on the tid stored in
		   the node.  When pthread_join() returns, we have cleaned up
		   after a thread.  Only then do we free() the node, decrement the
		   number of additional threads we need to wait for and repeat the
		   entire process, if necessary 
		 */

		curnode = (cnode *) queue_get(&cq.cleanup);
		pthread_mutex_unlock(&cq.control.mutex);
		pthread_join(curnode->tid,NULL);
		printf("joined with thread %d\n",curnode->threadnum);
		free(curnode);
		numthreads--;
	}
}

int create_threads(void) 
{
	int x;
	cnode *curnode;
	for (x = 0; x < NUM_WORKERS; x++) 
	{
		curnode = (cnode*)malloc(sizeof(cnode));
		if (!curnode)
			return 1;
		curnode->threadnum=x;
		if (pthread_create(&curnode->tid, NULL, threadfunc, (void *) curnode))
			return 1;
		printf("created thread %d\n",x);
		numthreads++;
	}
	return 0;
}

void initialize_structs(void) 
{
	numthreads=0;
	if (control_init(&wq.control))
		dabort();
	queue_init(&wq.work);

	if (control_init(&cq.control)) 
	{
		control_destroy(&cq.control);
		dabort();
	}
	queue_init(&cq.cleanup);

	control_activate(&wq.control);
}

void cleanup_structs(void) 
{
	control_destroy(&cq.control);
	control_destroy(&wq.control);
}

int main(void) 
{
	int x;
	wnode *mywork;
	initialize_structs();
		/* CREATION */
	if (create_threads()) 
	{					
		printf("Error starting threads... cleaning up.\n");
		join_threads();
		dabort();
	}

	pthread_mutex_lock(&wq.control.mutex);
	for (x = 0; x < 16000; x++) 
	{
		mywork = (wnode*)malloc(sizeof(wnode));
		if(!mywork)
		{
			printf("ouch! can not malloc! \n");
			break;
		}
  		mywork->jobnum = x;
      	queue_put(&wq.work,(node *) mywork);
	 }
	 pthread_mutex_unlock(&wq.control.mutex);

	 pthread_cond_broadcast(&wq.control.cond);

	 printf("sleeping...\n");
	 sleep(2);

	 printf("deactivating work queue...\n");
	 control_deactivate(&wq.control);

	 /* CLEANUP  */
     join_threads();
     cleanup_structs();

}


