#include <alloca.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#include <libtecla.h>

#define BUFLEN 4096
int cline = 0; // current line within block
char *mem;
int fd;
char *line;
GetLine *gl;

void accept() {
  line=gl_get_line(gl, "", NULL, -1);
  strncpy(mem + 64*cline, line, strlen(line) -1);
}

void bye() { close(fd); exit(0); }

void down() { cline++ ;}

void print_block() {
  int i, j;
  for(i=0; i<16; i++){
    if( i == cline) {printf(">");} else {printf(" ");};
    printf("%02d ", i);
    for(j=0; j<64; j++) { putchar(mem[i*64+j]);};
    puts("");
  }
}

typedef void (*wfunc) ();

struct WORD { char *cmd; wfunc func ;};

struct WORD words[] = {
  {"accept", accept},
  {"bye", bye},
  {"d", down},
  {"pb", print_block},
  {"q", bye},
  {NULL, NULL}
};

void process_token(char *token) {
  struct WORD *w = words;

  //puts(token);
  while(w->cmd != NULL) {
    
    //puts(w->cmd);
    if(strcmp(w->cmd, token) == 0) {
      //puts("Found");
      w->func();
      return;
    }
    w++;
  }
    //if(strcmp(token, "bye") == 0) {bye();}
    //else if(strcmp(token, "accept") == 0) {accept(); }
    puts("ERR: didn't understand token");
}

void process_line(char *line)
{
  char buf[BUFLEN];
  char delim[] = " \t\r\n";
  char *token;
  strcpy(buf, line);
  token = strtok(buf, delim);
  while(token != NULL)
    {
      process_token(token);
      token = strtok(NULL, delim);
    }
}


void repl() {

  gl = new_GetLine(BUFLEN, 2048);
  assert(gl);
  while ((line=gl_get_line(gl, "", NULL, -1)) != NULL) {process_line(line);}
  gl = del_GetLine(gl);
}


int main() {
  fd = open("blocks", O_RDWR);
  mem = (char *) alloca(1024*64);
  mem = (char *) mmap(NULL, 1024*64, PROT_EXEC | PROT_READ | PROT_WRITE,
             MAP_SHARED, fd, 0);
  //strcpy(mem, "DEADBEEF");
  repl();
  bye();
  // close(fd);
  return EXIT_SUCCESS;
}
