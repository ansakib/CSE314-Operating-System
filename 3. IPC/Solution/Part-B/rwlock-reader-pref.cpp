#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
   pthread_cond_init(&rw->read, NULL);
   pthread_cond_init(&rw->write, NULL);
   pthread_mutex_init(&rw->lock, NULL);

   rw->readers=0;
   rw->writers=0;
   rw->waiting_readers=0;
   rw->waiting_writers=0;

}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
  if( rw->writers==1){
    //printf("reader");
    rw->waiting_readers++;
    pthread_cond_wait(&rw->read,&rw->lock);
    rw->waiting_readers--;
  }
  //printf("readerg");
  rw->readers++;
  pthread_mutex_unlock(&rw->lock);
  pthread_cond_broadcast(&rw->read);
  
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.

  pthread_mutex_lock(&rw->lock);
  rw->readers--;
  if(rw->waiting_readers==0 && rw->readers==0){
    pthread_cond_signal(&rw->write);
  }

  pthread_mutex_unlock(&rw->lock);

}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);

  if(rw->writers==1 || rw->readers>0 || rw->waiting_readers>0){

      pthread_cond_wait(&rw->write,&rw->lock);   
  }
  //printf("writer");
 
  rw->writers++;
  pthread_mutex_unlock(&rw->lock);
  

}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
//printf("writerrel");
  rw->writers=0;
   if (rw->waiting_readers > 0)
        pthread_cond_signal(&rw->read);
    else
        pthread_cond_signal(&rw->write);

  pthread_mutex_unlock(&rw->lock);
}
