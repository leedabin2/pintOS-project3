/* file.c: Implementation of memory backed file object (mmaped object). */

#include "vm/vm.h"
/* project 3 에서 추가 */
#include "threads/vaddr.h"
#include "threads/mmu.h"
#include "userprog/process.h"

static bool file_backed_swap_in(struct page *page, void *kva);
static bool file_backed_swap_out(struct page *page);
static void file_backed_destroy(struct page *page);

/* DO NOT MODIFY this struct */
static const struct page_operations file_ops = {
	.swap_in = file_backed_swap_in,
	.swap_out = file_backed_swap_out,
	.destroy = file_backed_destroy,
	.type = VM_FILE,
};

/* The initializer of file vm */
void vm_file_init(void)
{
}

/* Initialize the file backed page */
bool file_backed_initializer(struct page *page, enum vm_type type, void *kva)
{
	/* Set up the handler */
	page->operations = &file_ops;

	struct file_page *file_page = &page->file;

	page->file.aux = page->uninit.aux;
}

/* Swap in the page by read contents from the file. */
static bool
file_backed_swap_in(struct page *page, void *kva)
{
	struct file_page *file_page UNUSED = &page->file;
}

/* Swap out the page by writeback contents to the file. */
static bool
file_backed_swap_out(struct page *page)
{
	struct file_page *file_page UNUSED = &page->file;
}

/* Destory the file backed page. PAGE will be freed by the caller. */
static void file_backed_destroy(struct page *page)
{
	struct file_page *file_page UNUSED = &page->file;
	struct aux *aux = file_page->aux;

	// if (pml4_is_dirty(thread_current()->pml4, page->va)) // 내용이 변경된 경우
	// {

	// 	// 변경 사항을 파일에 다시 기록
	// 	file_write_at(aux->file, page->va, file_length(aux->file), aux->ofs);
	// 	pml4_set_dirty(thread_current()->pml4, page->va, 0); // 변경 사항 다시 변경해줌
	// }
	// pml4_clear_page(thread_current()->pml4, page->va);
}

/* Do the mmap */
void *
do_mmap(void *addr, size_t length, int writable,
		struct file *file, off_t offset)
{
	struct file *f = file_reopen(file);
	void *s_addr = addr;

	// 읽어야할 바이트 수가 페이지 크기(4KB)보다 클 때, 최종적으로 몇 PAGE를 읽을지 저장해 두어야함
	// 추후 매핑을 해제 할 때에는 몇 번째 페이지까지 해제해야하는지 이 정보를 통해 알수 있음
	// int page_count;
	// if (length <= PGSIZE)
	// {
	// 	page_count = 1;
	// }
	// else if (length % PGSIZE)
	// {
	// 	page_count = length / PGSIZE + 1;
	// }
	// else
	// {
	// 	page_count = length / PGSIZE;
	// }

	size_t read_bytes = file_length(file) < length ? file_length(file) : length;
	size_t zero_bytes = PGSIZE - (read_bytes % PGSIZE);

	while (read_bytes > 0 || zero_bytes > 0)
	{
		size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
		size_t page_zero_bytes = PGSIZE - page_read_bytes;

		// disk에서 aux 구조체로 데이터 정보 저장
		struct aux *aux = (struct aux *)malloc(sizeof(struct aux));

		aux->file = f;
		aux->ofs = offset;
		aux->page_read_bytes = read_bytes;
		aux->page_zero_bytes = zero_bytes;
		aux->writable = writable;

		// Page VM_FILE 타입으로 선언해서 할당
		if (!vm_alloc_page_with_initializer(VM_FILE, addr, writable, lazy_load_segment, aux))
		{
			return false;
		}
		// page 구조체 내에 mapping page count 추가 해야함
		// 추후 데이터 삭제할때 페이지 단위로 삭제하므로 얼만큼 삭제해야할지 지표가 됌

		read_bytes -= page_read_bytes;
		zero_bytes -= page_zero_bytes;
		addr += PGSIZE;
		offset += page_read_bytes;
	}
	// struct page *page = spt_find_page(&thread_current()->spt, s_addr);
	// page->map_page_cnt = page_count;
	return s_addr;
}

/* Do the munmap */
void do_munmap(void *addr)
{
	// struct supplemental_page_table *spt = &thread_current()->spt;
	// struct page *page = spt_find_page(spt, addr);
	// int count = page->map_page_cnt;
	// for (int i = 0; i < count; i++)
	// {
	// 	if (page)
	// 		destroy(page);
	// 	addr += PGSIZE;
	// 	// 위치 조정해서 다시 찾아줌
	// 	page = spt_find_page(spt, addr);
	// }
	while(true)
	{
		struct page * page = spt_find_page(&thread_current()->spt, addr);
		if (page == NULL)
			break;
		struct aux * aux = (struct aux *)page->uninit.aux;

		if (pml4_is_dirty(thread_current()->pml4, page->va))
		{
			file_write_at(aux->file, page->frame->kva, aux->page_read_bytes, aux->ofs);
			pml4_set_dirty(thread_current()->pml4, page->va, 0);
		}
		pml4_clear_page(thread_current()->pml4, page->va);
		addr += PGSIZE;
		// destroy(page);
	}
}
