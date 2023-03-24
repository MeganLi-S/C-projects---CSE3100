#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void * thread_main(void * p_arg)
{
    // TODO
    thread_arg_t* my_data = (thread_arg_t*) p_arg;

    if(my_data->id == 0){
        for (unsigned int i = 1; i < (my_data->m->nrows); i+=2)
        for (unsigned int j = 0; j < (my_data->m->ncols); j++)
            my_data->t->data[i][j] = my_data->m->data[i][j] + my_data->n->data[i][j];}
    else if(my_data->id == 1){
        for (unsigned int i = 0; i < (my_data->m->nrows); i+=2)
        for (unsigned int j = 0; j < (my_data->m->ncols); j++)
            my_data->t->data[i][j] = my_data->m->data[i][j] + my_data->n->data[i][j];
    }
   /* TMatrix * m1 = newMatrix((my_data->m->nrows)/2, (my_data->m->ncols)/2);
    for (int i = 0; i < (my_data->m->nrows)){
        for (int j = 0; i < (my_data->m->ncols)){
            m1[i][j] = my_data->m[i][j];
        }
    }*/
    
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    pthread_t threads[NUM_THREADS];
    thread_arg_t thread_data_array[NUM_THREADS];
    int rc, i;
	for( i=0; i<NUM_THREADS; i++ ) {
        thread_data_array[i].m = m;
        thread_data_array[i].n = n;
        thread_data_array[i].t = t;
        thread_data_array[i].id = i;
		
        rc = pthread_create(&threads[i], NULL, thread_main, &thread_data_array[i]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    for( i=0; i<NUM_THREADS; i++ ) {
        rc = pthread_join( threads[i], NULL );
        if( rc ){
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        

    }
    

    // TODO



    return t;
}
