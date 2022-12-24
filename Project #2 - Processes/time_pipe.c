// Names: Jose Ordaz, Jessica Nguyen, Viktoriya Penkova, Natalia Perez, Lauren Prather, Ben Pennington
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>

int main(int argc, char**argv) {
  int fd[2]; // fd[0] = read, fd[1] = write

  struct timeval startTime;     // Get the current time
  struct timeval endTime;      // Get the ending time
  struct timeval elapsedTime;  // Get the elapsed time
  
  pid_t pid;                   // Parent is created parent_id (pid)

  /* Establishing the pipe */
  if(pipe(fd) == -1) {
    fprintf(stderr, "The pipe has failed.\n");
    exit(-1);
  }

  pid = fork();   // Child is created

  /* Fork creation failure */
  if(pid < 0) {
    fprintf(stderr, "The fork has failed.\n");
    exit(-1);
  }

  /* Parent process : Read from the pipe following the termination of the child */
  if(pid > 0) {
    printf("Hello, I am the parent\n");
    wait(0);
    printf("...Child has completed its tasks\n");

    /* Close the unused end of pipe (write)
       fd[0] = read, fd[1] = write */
    close(fd[1]);
    
    /* Allow the parent to read from the pipe */
    read(fd[0], &startTime, (sizeof(struct timeval)));
    close(fd[0]); // Close the read end of the pipe
  
    /* Calculate the elapsed time */
    gettimeofday(&endTime, 0);
    timersub(&endTime, &startTime, &elapsedTime);
    printf("\nElapsed Time: %ld.%06ld seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec);
    exit(0);
  }

  /* Child process : Write the starting time to the pipe */
  else {

    printf("\nHello, I am the child\n");
    // Close unused end of pipe (read)
    close(fd[0]);
    
    gettimeofday(&startTime, 0);
    
    /* Allow the child to write to the pipe */
    write(fd[1], &startTime, (sizeof(struct timeval)));
    close(fd[1]); // Close the write end of the pipe
    
    execvp(argv[1], &argv[1]);
    exit(0);
  }
  return 0;
}
