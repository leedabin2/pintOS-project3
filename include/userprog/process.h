#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_create_initd (const char *file_name);
tid_t process_fork (const char *name, struct intr_frame *if_);
int process_exec (void *f_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (struct thread *next);

/* project3 에서 헤더파일 참조해야하므로 추가하자! */
bool lazy_load_segment(struct page *page, void *aux);

struct aux {
	struct file *file ;
	off_t ofs; 
	bool writable;
	uint32_t page_zero_bytes;
	uint32_t page_read_bytes;
};

#endif /* userprog/process.h */
