/* anon.c: Implementation of page for non-disk image (a.k.a. anonymous page). */

#include "devices/disk.h"
#include "threads/vaddr.h"
#include "vm/vm.h"
#include "lib/kernel/bitmap.h"
#include "threads/mmu.h"

/* DO NOT MODIFY BELOW LINE */
static struct disk *swap_disk;
static bool anon_swap_in(struct page *page, void *kva);
static bool anon_swap_out(struct page *page);
static void anon_destroy(struct page *page);

struct bitmap *swap_table;

/* DO NOT MODIFY this struct */
static const struct page_operations anon_ops = {
    .swap_in = anon_swap_in,
    .swap_out = anon_swap_out,
    .destroy = anon_destroy,
    .type = VM_ANON,
};

/* Initialize the data for anonymous pages */
void vm_anon_init(void) {
    /* TODO: Set up the swap_disk. */
    // swap_disk = NULL;
    swap_disk = disk_get(1, 1);  // 1:1 - 스왑디스크
    // // 스왑 영역도 PGSIZE(4096바이트) 단위로 관리
    // 스왑 테이블이 필요 - bit_map으로 관리, 사용가능한 slot공간 찾을 수 있도록 설정
    size_t swap_disk_size = disk_size(swap_disk) / 8; // swap disk에 들어갈 수 있는 페이지 개수 
    swap_table = bitmap_create(swap_disk_size);            // swap disk 크기만큼 동적 할당
}

/* Initialize the file mapping */
bool anon_initializer(struct page *page, enum vm_type type, void *kva) {
    /* Set up the handler */
    page->operations = &anon_ops;

    struct anon_page *anon_page = &page->anon;
		anon_page->swap_idx = -1;
}

/* Swap in the page by read contents from the swap disk. */
static bool anon_swap_in(struct page *page, void *kva) {
    struct anon_page *anon_page = &page->anon;
		
		if (bitmap_test(swap_table, anon_page->swap_idx) == false) // anon_page에 저장한 slot 정보를 통해 swap_disk에 내용가져오기
			return false; 
		
		for (int i = 0; i < 8; ++i)
		{
			disk_read(swap_disk, anon_page->swap_idx * 8 + i, kva + DISK_SECTOR_SIZE * i); // 디스크 swap_disk 에서 섹터 SEC_NO를 읽어 BUFFER에 저장
		}

		bitmap_set(swap_table,anon_page->swap_idx,false);
		
		return true;

}

/* Swap out the page by writing contents to the swap disk. */
static bool anon_swap_out(struct page *page) {
    struct anon_page *anon_page = &page->anon;
		// swap_disk, swap_table 에서 사용가능한 slot공간 찾기 - 추후 사용 slot의 bit -> true 으로 바꿔줘야 함
		int slot_no = bitmap_scan(swap_table,0,1,false);  // 단일 페이지 

		if (slot_no == BITMAP_ERROR)
			return false;
		
		for (int i = 0; i < 8; ++i) 
		{	
			// page->frame->kva의 데이터를 slot에 write // 어차피 매핑된 상태에서의 데이터를 쓰는 거니까 page->va 도 상관없을 듯
			// 디스크의 섹터 SEC_NO에 BUFFER에 저장된 DISK_SECTOR_SIZE 바이트의 데이터를 쓰는 역할
			disk_write(swap_disk, slot_no * 8 + i, page->va + DISK_SECTOR_SIZE * i);

		}

		// 추후 사용 slot의 bit -> true 
		bitmap_set(swap_table, slot_no, true);
		// present bit 0으로 설정 - 물리 메모리와 매핑 해제
		pml4_clear_page(thread_current()->pml4, page->va);
		
		// page->anonpage에 사용한 slot의 정보(데이터의 위치)를 저장
		anon_page->swap_idx = slot_no;
		// 만약 디스크에 슬롯이 없다면 커널 패닉
		return true;
}

/* Destroy the anonymous page. PAGE will be freed by the caller. */
static void anon_destroy(struct page *page) {
    struct anon_page *anon_page = &page->anon;
		// anon의 자원들을 free , 페이지 구조체를 free할 필요없음
    // if (page->frame->kva)
    // {
    // 	free(page->frame);
    // }
}
