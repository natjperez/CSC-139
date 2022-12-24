/* Stack containing race conditions. Use PThread mutex locks, fix
   the race conditions (two threads trying to change the value of
   a variable */

// Names: Jose Ordaz, Jessica Nguyen, Viktoriya Penkova, Natalia Perez, Lauren
// Prather, Ben Pennington
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 200

// Linked list node
typedef int value_t;
typedef struct Node {
  value_t data;
  struct Node *next;
  // pthread_mutex_t mutex[NUM_THREADS];
  // pthread_mutex_t mutex;
} StackNode;

// Stack function declarations
void push(value_t v, StackNode **top);
value_t pop(StackNode **top);
int is_empty(StackNode *top);
void *testStack(void *top);
void stackPrint(StackNode *top);
StackNode **top = NULL;
pthread_mutex_t mutex;

// All threads use the same stack
// All mutex functions return a value of 0 with correct operation.
// If an error occurs, these functions return a non-zero error code.
int main(void) {

  pthread_t tid[NUM_THREADS]; // thread identifier
  pthread_attr_t attr;        // set of thread attributes
  pthread_attr_init(&attr);   //  get default attributes

  // Using a loop, create 200 threads that intermix push/pop values.
  // StackNode *top = NULL; // shared

  // Create thread loop
  for (int k = 0; k < NUM_THREADS; k++) {
    pthread_create(&tid[k], &attr, testStack, top);
  }
  for (int p = 0; p < NUM_THREADS; p++) {
    // Wait for thread to exit
    pthread_join(tid[p], NULL);
  }
  return 0;
}

// Stack function definitions
// Push a value to the stack
void push(value_t v, StackNode **top) {
  StackNode *new_node = malloc(sizeof(StackNode));
  pthread_mutex_init(&mutex, NULL); // &mutex original
  pthread_mutex_lock(&mutex);       // &mutex original
  new_node->data = v;
  new_node->next = *top;
  *top = new_node;
  pthread_mutex_unlock(&mutex); // &mutex original
}

// Pop value from top of the stack
value_t pop(StackNode **top) {
  if (is_empty(*top))
    return (value_t)0;
  pthread_mutex_init(&mutex, NULL); // &mutex original
  pthread_mutex_lock(&mutex);       // &mutex original
  value_t data = (*top)->data;
  StackNode *temp = *top;
  *top = (*top)->next;
  free(temp);
  pthread_mutex_unlock(&mutex); // &mutex original
  return data;
}

// Check if Stack is empty
int is_empty(StackNode *top) {
  if (top == NULL)
    return 1;
  else
    return 0;
}

// Print the stack
void stackPrint(StackNode *top) {
  pthread_mutex_lock(&mutex);
  while (top != NULL) {
    printf("Top = %d\n", top->data);
    top = top->next;
  }
  pthread_mutex_unlock(&mutex);
}

// Intermix 3 push operations with 3 pop operations
// in a loop that executes 500 times. This function
// is used as an entry point for each thread.
void *testStack(void *top) {
  int value = 0; 
  // pthread_mutex_init(mutex, NULL); // &mutex original
  // pthread_mutex_lock(mutex); // &mutex original
  
  for (int i = 0; i < 500; i++) {
    
    StackNode* top_t = (StackNode*)top;
    push(5, &top_t);
    value = pop(&top_t);
    printf("Value = %i\n", value);
    push(0, &top_t);
    value = pop(&top_t);
    printf("Value = %i\n", value);
    push(-5, &top_t);
    value = pop(&top_t);
    printf("Value = %i\n", value);
  }
  stackPrint(top);
  // pthread_mutex_unlock(mutex); // &mutex original
  pthread_exit(0);
}
