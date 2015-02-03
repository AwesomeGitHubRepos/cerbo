#ifndef INODES_H
#define INODES_H

#define MAX_INODES 7500

typedef struct inodes {
  int nnodes; /* number of nodes */
  int cursor; /* a marker for looping purposes */
  int magic; /* marker that checks for initialisation */
  void* pnodes[MAX_INODES]; /* pointers to the nodes themselves */
} inodes;

void init_inodes(inodes *p_inodes);
void insert_inodes(inodes *p_inodes, void *p_data);
void *linode(inodes *pinodes);

#endif /* INODES_H */
