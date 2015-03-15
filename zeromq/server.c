

// Hello World server

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int count = 0;

int main (void)
{
  // Socket to talk to clients
  void *context = zmq_ctx_new ();
  void *responder = zmq_socket (context, ZMQ_REP);
  int rc = zmq_bind (responder, "tcp://*:5555");
  assert (rc == 0);

  while (1) {
    char buffer [20];
    zmq_recv (responder, buffer, 20, 0);
    printf ("Received Hello\n");
    sleep (1); // Do some 'work'
    sprintf(buffer, "Answer: %d\n", count);
    zmq_send (responder, buffer, 20, 0);
    count++;
  }
  return 0;
}
