// time_shm.c -  Determines the amount of time necessary to run a command from the
// command line and report the amount of elapsed time to run the specified command ./time <command [args...]>

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

int main(int argc, char**argv){ // change parameters

  int fd;
  const char *name = "SM";      // Name of the shared memory object
  struct timeval startTime;     // Get the current time
  struct timeval endTime;      // Get the ending time
  struct timeval elapsedTime; // Get the elapsed time

  // The region of shared memory should be established before the child process is forked
  struct timeval *ptr = (struct timeval *)malloc(sizeof(struct timeval));
  fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, sizeof(struct timeval));
  ptr = (struct timeval *) mmap(0, (sizeof(struct timeval)), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  pid_t pid;       // Parent is created parent_id (pid)
  pid = fork();   // Child is created
  pid_t getpid();
  
  if(pid < 0) {
    fprintf(stderr, "The fork has failed.\n");
    exit(-1);
  }

  // Child process : Write the starting time to a region of shared memory before calling execvp()
  if(pid == 0){
    printf("\nHello, I am the child\n");
    
    gettimeofday(&startTime, 0);
    
    // Write starting time to region of shared memory here
    memcpy(ptr, &startTime, sizeof(struct timeval));

    execvp(argv[1], &argv[1]);
    
    exit(0); // Terminates child
  } 

  // Parent process : Read the starting time from shared memory after the child terminates.
  else {
    printf("Hello, I am the parent\n");
    pid = wait(0); // waits for child
    printf("...Child has completed its tasks\n");

    // Read startTime from shared memory
    memcpy(&startTime, ptr, sizeof(struct timeval));
    
    // Once child terminates, parent will record the current timestap for the ending time
    gettimeofday(&endTime, 0);

    // Subtracts startTime from endTime and puts result in value pointed to by elapsedTime
    timersub(&endTime, &startTime, &elapsedTime);
    printf("\nElapsed Time: %ld.%06ld seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec);
    
    exit(0);
  }
}
