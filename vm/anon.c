/* anon.c: Implementation of page for non-disk image (a.k.a. anonymous page). */

#include "vm/vm.h"
#include "devices/disk.h"

#include "threads/mmu.h"
#include "lib/kernel/bitmap.h"
struct bitmap * swap_table;
const size_t SECTORS_PER_PAGE =  PGSIZE / DISK_SECTOR_SIZE; // 필요한 스왑슬롯 8개 (4096/512 = 8)

/* DO NOT MODIFY BELOW LINE */
static struct disk *swap_disk;
static bool anon_swap_in (struct page *page, void *kva);
static bool anon_swap_out (struct page *page);
static void anon_destroy (struct page *page);

/* DO NOT MODIFY this struct */
static const struct page_operations anon_ops = {
	.swap_in = anon_swap_in,
	.swap_out = anon_swap_out,
	.destroy = anon_destroy,
	.type = VM_ANON,
};

/* Initialize the data for anonymous pages */
void
vm_anon_init (void) {
	/* TODO: Set up the swap_disk. */
	// swap_disk = NULL;
	/*
	 * swap_size : 스왑 디스크 안에서 만들 수 있는 스왑 슬롯의 갯수 
	 * swap_table : 스왑 영역을 관리하는 스왑 테이블
	 * 각각의 비트는 스왑 슬롯 각각과 매칭됨
	 */
	swap_disk = disk_get(1,1); 
	size_t swap_size = disk_size(swap_disk) / 8;
	swap_table = bitmap_create(swap_size);
}

/* Initialize the file mapping */
bool
anon_initializer (struct page *page, enum vm_type type, void *kva) {
	/* Set up the handler */
	struct uninit_page *uninit = &page->uninit;
	memset(uninit, 0, sizeof(struct uninit_page));

	page->operations = &anon_ops;

	struct anon_page *anon_page = &page->anon;
	// 해당 페이지는 이제 물리 메모리 위에 있으므로 index -1로 설정
	anon_page->swap_idx = BITMAP_ERROR;
	return true;
}

/* Swap in the page by read contents from the swap disk. */
static bool
anon_swap_in (struct page *page, void *kva) {
	struct anon_page *anon_page = &page->anon;

	int page_no = anon_page->swap_idx;

    if(bitmap_test(swap_table, page_no) ==  false)
		return false;

	for (int i = 0; i < SECTORS_PER_PAGE; ++i)
	{
		disk_read(swap_disk, page_no * SECTORS_PER_PAGE + i, kva + DISK_SECTOR_SIZE * i);
	}
	bitmap_set(swap_table, page_no, false);

	return true;
}

/* Swap out the page by writing contents to the swap disk. */
static bool
anon_swap_out (struct page *page) {
	struct anon_page *anon_page = &page->anon;

	// 1. 비트맵을 순회하며 false 값을 가진 비트를 찾는다 = 페이지를 할당 받을 수 있는 스왑 슬롯 찾는다
	int page_no = bitmap_scan(swap_table, 0, 1, false);

	if (page_no == BITMAP_ERROR)
	{
		return false;
	}

	/* 한 페이지를 디스크에 써 주기 위해 8개의 섹터에 저장해야 한다.
	   이 때 디스크에 각 섹터의 크기 DISK_SECTOR_SIZE만큼 써 준다. */
	for (int i = 0; i < (PGSIZE/DISK_SECTOR_SIZE); ++i)
	{
		disk_write(swap_disk, page_no * SECTORS_PER_PAGE + i, page->va + DISK_SECTOR_SIZE * i);
	}
	bitmap_set(swap_table, page_no, true);
	pml4_clear_page(thread_current()->pml4, page->va);
	anon_page->swap_idx = page_no;

	return true;
}

/* Destroy the anonymous page. PAGE will be freed by the caller. */
static void
anon_destroy (struct page *page) {
	struct anon_page *anon_page = &page->anon;
	
	// if (page->frame->kva)
	// {
	// 	free(page->frame);
	// }
}
