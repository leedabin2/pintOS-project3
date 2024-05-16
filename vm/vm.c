/* vm.c: Generic interface for virtual memory objects. */

/* vm.c: 가상 메모리 객체에 대한 일반적인 인터페이스입니다. */
#include "vm/vm.h"
#include "threads/malloc.h"
#include "vm/inspect.h"

#include "threads/vaddr.h"
#include "lib/kernel/hash.h"
/* Initializes the virtual memory subsystem by invoking each subsystem's
 * intialize codes. */

/* 가상 메모리 서브시스템을 각 서브시스템의 초기화 코드를 호출함으로써 초기화합니다. */

void vm_init(void) {
    vm_anon_init();
    vm_file_init();
#ifdef EFILESYS /* For project 4 */
    pagecache_init();
#endif
    register_inspect_intr();
    /* DO NOT MODIFY UPPER LINES. */
    /* TODO: Your code goes here. */
}

/* Get the type of the page. This function is useful if you want to know the
 * type of the page after it will be initialized.
 * This function is fully implemented now. */

/* 페이지의 타입을 가져옵니다. 이 함수는 페이지가 초기화된 후 그 타입을 알고 싶을 때 유용합니다.
 * 이 함수는 현재 완전히 구현되어 있습니다. */
enum vm_type page_get_type(struct page *page) {
    int ty = VM_TYPE(page->operations->type);
    switch (ty) {
        case VM_UNINIT:
            return VM_TYPE(page->uninit.type);
        default:
            return ty;
    }
}

/* Helpers */
static struct frame *vm_get_victim(void);
static bool vm_do_claim_page(struct page *page);
static struct frame *vm_evict_frame(void);

/* Create the pending page object with initializer. If you want to create a
 * page, do not create it directly and make it through this function or
 * `vm_alloc_page`. */

/* 초기화자와 함께 보류 중인 페이지 객체를 생성합니다.
 * 페이지를 생성하고자 한다면, 직접 생성하지 말고 이 함수나 vm_alloc_page를 통해 생성하세요. */
bool vm_alloc_page_with_initializer(enum vm_type type, void *upage, bool writable, vm_initializer *init, void *aux) {
    ASSERT(VM_TYPE(type) != VM_UNINIT)

    struct supplemental_page_table *spt = &thread_current()->spt;

    /* Check wheter the upage is already occupied or not. */
    if (spt_find_page(spt, upage) == NULL) {
        /* TODO: Create the page, fetch the initialier according to the VM type,
         * TODO: and then create "uninit" page struct by calling uninit_new. You
         * TODO: should modify the field after calling the uninit_new. */

        /* TODO: Insert the page into the spt. */
    }
err:
    return false;
}

/* Find VA from spt and return page. On error, return NULL. */
/* spt에서 VA를 찾아 페이지를 반환합니다. 오류가 발생하면 NULL을 반환합니다. */
/* 목표 : va 를 가지고 page 찾기 
    spt는 해시구조라 hasg_elem 을 가지고 page 찾아야함 */
struct page *spt_find_page(struct supplemental_page_table *spt UNUSED, void *va UNUSED) {
    // struct page *page = NULL;
    /* TODO: Fill this function. */
    
    // dummy page 를 만들어서 hash_elem 을 추출한다.
    struct page *page = (struct page *)malloc(sizeof(struct page));
    struct hash_elem *e;

    page->va = pg_round_down(va); 
    e = hash_find(&spt->spt_hash, &page->spt_entry); // 찾고자하는 va에 해당하는 페이지의 hash_elem 추출
    free(page); // dummy page - free 하기 

    if (e != NULL)
    {
        return hash_entry(e,struct page, spt_entry); // 실제 페이지 리턴
    }
    return NULL;
}

/* Insert PAGE into spt with validation. */
/* 유효성 검사와 함께 spt에 PAGE를 삽입합니다. */
bool spt_insert_page(struct supplemental_page_table *spt UNUSED, struct page *page UNUSED) {
    int succ = false;
    /* TODO: Fill this function. */

    return succ;
}

void spt_remove_page(struct supplemental_page_table *spt, struct page *page) {
    vm_dealloc_page(page);
    return true;
}

/* Get the struct frame, that will be evicted. */

/* 추방될 struct frame을 가져옵니다. */
static struct frame *vm_get_victim(void) {
    struct frame *victim = NULL;
    /* TODO: The policy for eviction is up to you. */

    return victim;
}

/* Evict one page and return the corresponding frame.
 * Return NULL on error.*/

/* 한 페이지를 쫓아내고 해당하는 프레임을 반환합니다.
 * 오류가 발생하면 NULL을 반환합니다. */
static struct frame *vm_evict_frame(void) {
    struct frame *victim UNUSED = vm_get_victim();
    /* TODO: swap out the victim and return the evicted frame. */

    return NULL;
}

/* palloc() and get frame. If there is no available page, evict the page
 * and return it. This always return valid address. That is, if the user pool
 * memory is full, this function evicts the frame to get the available memory
 * space.*/

/* palloc()을 호출하여 프레임을 획득합니다.
 * 사용 가능한 페이지가 없는 경우, 페이지를 쫓아내고 반환합니다.
 * 이 함수는 항상 유효한 주소를 반환합니다. 즉, 사용자 풀 메모리가 가득 찬 경우,
 * 이 함수는 프레임을 쫓아내어 사용 가능한 메모리 공간을 확보합니다. */
static struct frame *vm_get_frame(void) {
    struct frame *frame = NULL;
    /* TODO: Fill this function. */

    ASSERT(frame != NULL);
    ASSERT(frame->page == NULL);
    return frame;
}

/* Growing the stack. */
static void vm_stack_growth(void *addr UNUSED) {
}

/* Handle the fault on write_protected page */
static bool vm_handle_wp(struct page *page UNUSED) {
}

/* Return true on success */
bool vm_try_handle_fault(struct intr_frame *f UNUSED, void *addr UNUSED, bool user UNUSED, bool write UNUSED, bool not_present UNUSED) {
    struct supplemental_page_table *spt UNUSED = &thread_current()->spt;
    struct page *page = NULL;

    // 유저 프로세스가 접근하려던 주소에서 데이터를 얻을 수 없거나, 페이지가 커널 가상 메모리 영역에 존재하거나, 읽기 전용 페이지에 대해 쓰기를 시도하는 상황 
    // 프로세스를 종료시키고 프로세스의 모든 자원을 해제합니다.
    /* TODO: Validate the fault */
    /* TODO: Your code goes here */

    return vm_do_claim_page(page);
}

/* Free the page.
 * DO NOT MODIFY THIS FUNCTION. */
void vm_dealloc_page(struct page *page) {
    destroy(page);
    free(page);
}

/* Claim the page that allocate on VA. */
bool vm_claim_page(void *va UNUSED) {
    struct page *page = NULL;
    /* TODO: Fill this function */

    return vm_do_claim_page(page);
}

/* Claim the PAGE and set up the mmu. */
static bool vm_do_claim_page(struct page *page) {
    struct frame *frame = vm_get_frame();

    /* Set links */
    frame->page = page;
    page->frame = frame;

    /* TODO: Insert page table entry to map page's VA to frame's PA. */

    return swap_in(page, frame->kva);
}

/* Initialize new supplemental page table */
void supplemental_page_table_init(struct supplemental_page_table *spt UNUSED) {
    
    hash_init(&spt->spt_hash, page_hash, page_less, NULL);
}

/* Copy supplemental page table from src to dst */
bool supplemental_page_table_copy(struct supplemental_page_table *dst UNUSED, struct supplemental_page_table *src UNUSED) {
}

/* Free the resource hold by the supplemental page table */
void supplemental_page_table_kill(struct supplemental_page_table *spt UNUSED) {
    /* TODO: Destroy all the supplemental_page_table hold by thread and
     * TODO: writeback all the modified contents to the storage. */
    /* TODO: 스레드가 보유한 모든 추가 페이지 테이블을 파괴하고, */
    /* TODO: 수정된 모든 내용을 저장소에 다시 쓰세요. */
}

/* hash_elem을 사용하여 page->va 정보를 불러와 해쉬값 반환 */
unsigned page_hash(struct hash_elem *p_, void *aux UNUSED){

    struct page *page = hash_entry(p_, struct page, spt_entry);

    return hash_bytes(&page->va, sizeof(page->va));
}

/* */
bool page_insert(struct hash *h, struct page *page) {
    if (!hash_insert(h,&page->spt_entry))
    {
        return true;
    }
    return false;  
}

bool page_delete(struct hash *h, struct page *page) {
    if (!hash_delete(h,&page->spt_entry))
    {
        return true;
    }
    return false;  
}

/* 보조 데이터 AUX가 주어진 두 해시 요소 a와 b의 값을 비교 */
bool page_less(struct hash_elem *a, struct hash_elem *b, void *aux) {
    struct page *page_a = hash_entry(a, struct page, spt_entry);
    struct page *page_b = hash_entry(b, struct page, spt_entry);

    if (page_a->va < page_b->va)
    {
        return true;
    }
    return false;    
}







