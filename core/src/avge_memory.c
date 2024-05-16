#define NO_MALLOC_REDEF
#include <avge.h>
#undef NO_MALLOC_REDEF

#include <nicelog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <avge_internal.h>

#define MAX_AVGE_MALLOC 512

struct mem_block {
  size_t size;
  void *ptr;
};

struct file_line_mem_block {
  uint line;
  char *file;
  char *func;

  struct mem_block *mem_blocks;
  uint mem_block_count;
  uint allocated;

  uint total_allocated;
  uint total_freed;
  uint size;
};

static struct file_line_mem_block mem_allocs[MAX_AVGE_MALLOC];
static uint mem_alloc_count = 0;

void AVGE_mem_add(void *ptr, char *file, int line, size_t size,
                  const char *func) {
  int i;
  for (i = 0; i < mem_alloc_count; ++i) {
    if (mem_allocs[i].line == line && strcmp(mem_allocs[i].file, file) == 0)
      break;
  }
  if (i < mem_alloc_count) {
    if (mem_allocs[i].allocated == mem_allocs[i].mem_block_count) {
      mem_allocs[i].mem_block_count += 128;
      if ((mem_allocs[i].mem_blocks = realloc(
               mem_allocs[i].mem_blocks,
               sizeof(struct mem_block) * mem_allocs[i].mem_block_count)) ==
          NULL) {
        FATAL(AVGE_state.logger,
              "Failed to realloc() memory blocks on line %d in file %s", line,
              file);
        exit(AVGE_EXIT_FAILURE);
      }
    }
    mem_allocs[i].mem_blocks[mem_allocs[i].allocated].ptr = ptr;
    mem_allocs[i].mem_blocks[mem_allocs[i].allocated++].size = size;
    mem_allocs[i].total_allocated++;
    mem_allocs[i].size += size;
  } else if (i < MAX_AVGE_MALLOC) {
    mem_allocs[i].file = malloc(strlen(file) + 1);
    mem_allocs[i].func = malloc(strlen(func) + 1);
    strcpy(mem_allocs[i].file, file);
    strcpy(mem_allocs[i].func, func);
    mem_allocs[i].line = line;
    mem_allocs[i].mem_block_count = 128;
    mem_allocs[i].mem_blocks =
        malloc(sizeof(struct mem_block) * mem_allocs[i].mem_block_count);
    mem_allocs[i].mem_blocks[0].ptr = ptr;
    mem_allocs[i].mem_blocks[0].size = size;
    mem_allocs[i].size = size;
    mem_allocs[i].total_freed = 0;
    mem_allocs[i].allocated = 1;
    mem_allocs[i].total_allocated = mem_allocs[i].allocated;
    ++mem_alloc_count;
  }
}

void *AVGE_malloc(size_t size, char *file, uint line, const char *func) {
  void *ptr;
  ptr = malloc(size);
  if (!ptr) {
    FATAL(AVGE_state.logger,
          "Could not malloc() memory on line %d int file %s !", line, file);
    exit(AVGE_EXIT_FAILURE);
  }
  AVGE_mem_add(ptr, file, line, size, func);
  return ptr;
}

void _free(int i, int j) {
  mem_allocs[i].size -= mem_allocs[i].mem_blocks[j].size;
  mem_allocs[i].mem_blocks[j] =
      mem_allocs[i].mem_blocks[--mem_allocs[i].allocated];
  mem_allocs[i].total_freed++;
}

void AVGE_free(void *ptr) {
  int i, j;
  for (i = 0; i < mem_alloc_count; ++i) {
    for (j = 0; j < mem_allocs[i].allocated; ++i) {
      if (mem_allocs[i].mem_blocks[j].ptr == ptr) {
        _free(i, j);
      }
    }
  }
  free(ptr);
}

void AVGE_print_memory_debug() {
  BEGIN("********** Starting Memory Debug **********");
  int i;
  for (i = 0; i < mem_alloc_count; ++i) {
    DEBUG(AVGE_state.logger, "%d. File : %s; Line: %d; Func: %s", i + 1,
          mem_allocs[i].file, mem_allocs[i].line, mem_allocs[i].func);
    DEBUG(AVGE_state.logger,
          "    Bytes: %u; Current allocations: %d; Total allocations: %d; "
          "Total freed: %d",
          mem_allocs[i].size, mem_allocs[i].allocated,
          mem_allocs[i].total_allocated, mem_allocs[i].total_freed);
    DEBUG(AVGE_state.logger,
          "-------------------------------------------------------");
  }
  DONE(NL_OK);
}

void AVGE_free_all() {
  for (int i = 0; i < mem_alloc_count; ++i) {
    for (int j = 0; j < mem_allocs[i].allocated; ++j) {
      _free(i, j);
    }
  }
}
