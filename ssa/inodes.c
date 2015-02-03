#include <assert.h>

#include "inodes.h"

void init_inodes(inodes *p_inodes)
{
  int i;
  p_inodes->nnodes = 0;
  p_inodes->cursor = -1;
  p_inodes->magic = 0xFEEDFACE;
  for(i=0; i < MAX_INODES; i++) {p_inodes->pnodes[i] = 0 ;};
}
void insert_inodes(inodes *p_inodes, void *p_data)
{
  assert(p_inodes->magic == 0xFEEDFACE); // the data structure has been initialised, right?
  assert(p_inodes->nnodes<MAX_INODES);
  p_inodes->pnodes[p_inodes->nnodes] = p_data;
  p_inodes->nnodes++;
}

void *linode(inodes *pinodes)
{
  pinodes->cursor++;
  if(pinodes->cursor == pinodes->nnodes) {
    pinodes->cursor = -1; //reset it to the beginning
    return 0;
  }

  return pinodes->pnodes[pinodes->cursor];
}
