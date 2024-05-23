/* vm.c: Generic interface for virtual memory objects. */

/* vm.c: 가상 메모리 객체에 대한 일반적인 인터페이스입니다. */
#include "vm/vm.h"
#include "threads/malloc.h"
#include "vm/inspect.h"
#include "threads/mmu.h"

#include "threads/vaddr.h"
#include "lib/kernel/hash.h"
/* Initializes the virtual memory subsystem by invoking each subsystem's
 * intialize codes. */
#include <stdlib.h>
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
        struct page *new_page = (struct page *)malloc(sizeof(struct page));
        bool (*initializer)(struct page *, enum vm_type, void *) ;
       
        switch (VM_TYPE(type))
        {
            case VM_ANON:
                initializer = anon_initializer;
                break;
            case VM_FILE:
                initializer = file_backed_initializer;
                break;
        } 
        uninit_new(new_page, upage, init, type, aux, initializer);

        new_page->writable = writable; // 추가
        bool ok = spt_insert_page(&thread_current()->spt, new_page);

        struct page *result = spt_find_page(&thread_current()->spt, upage);
        if (result == NULL) {
            goto err;
        }
        return true;
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
    // int succ = false;
    /* TODO: Fill this function. */

    // struct hash *hash = page_hash(&page->spt_entry, NULL);
    // if (spt->spt_hash.hash == hash) 
        // spt에 페이지 구조체 삽입
    return page_insert(&spt->spt_hash, page);
    
    // return succ;
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
    // struct frame *frame = NULL;
    /* TODO: Fill this function. */
    uint64_t *kva = palloc_get_page(PAL_USER); // palloc_get_page()를 통해 물리적 메모리를 할당하고, kva를 반환함

    if (kva == NULL) 
        PANIC("todo : swap out 구현해야함.");
    
    struct frame *frame = (struct frame *)malloc(sizeof(struct frame));
    // 구조체 멤버 초기화
    frame->kva = kva; 
    frame->page = NULL;

    ASSERT(frame != NULL);
    ASSERT(frame->page == NULL);
    return frame;
}

/* Growing the stack. */
static void vm_stack_growth(void *addr UNUSED) {
    // anon 페이지를 할당해서 스택 크기 증가
    vm_alloc_page(VM_ANON|VM_MARKER_0, pg_round_down(addr), 1);
    // 페이지를 할당할 때는 주소를 PGSIZE 기준으로 내림 
}

/* Handle the fault on write_protected page */
static bool vm_handle_wp(struct page *page UNUSED) {
}

/* Return true on success */
bool vm_try_handle_fault(struct intr_frame *f UNUSED, void *addr UNUSED, bool user UNUSED, bool write UNUSED, bool not_present UNUSED) {
    struct supplemental_page_table *spt UNUSED = &thread_current()->spt;
    //write: page fault를 일으킨 명령어가 write일 경우

    if (addr == NULL || !is_user_vaddr(addr)) // 사용자 주소가 아닌 경우 
        return false;

    // struct intr_frame *user_if = pg_round_up(thread_current() + 1) - sizeof(struct intr_frame);
	// void *rsp = user_if->rsp;
    void *rsp = f->rsp; 
    if (!user) { // ex) syscall 의 커널모드에서 페이지 폴트가 나서 , user stack을 증가시켜야 할 때, 
        void *rsp = thread_current()->rsp; // syscall에서 커널모드로 전환하기 전에 저장한 user모드의 rsp를 가져옴
    }

    if (not_present) // 접근한 메모리의 physical page가 존재하지 않은 경우
    {
        if (addr >= rsp - 8 && rsp - 8 >= USER_STACK - (1<<20) && addr <= USER_STACK ) {
            vm_stack_growth(addr);
        } 
        if (addr >= rsp && rsp >= USER_STACK - (1<<20) && addr <= USER_STACK) {
            vm_stack_growth(addr); 
        }
        struct page * page = spt_find_page(spt,addr);
        if (page == NULL){ // 찐 폴트는 걍 죽음
            return false;
        }
        return vm_do_claim_page(page);
    }
    
    // 유저 프로세스가 접근하려던 주소에서 데이터를 얻을 수 없거나, 페이지가 커널 가상 메모리 영역에 존재하거나, 읽기 전용 페이지에 대해 쓰기를 시도하는 상황 
    // 프로세스를 종료시키고 프로세스의 모든 자원을 해제합니다.
    /* TODO: Validate the fault */
    /* TODO: Your code goes here */
    // 그렇지 않다면, bogus 폴트 (일단 , 지연 로딩의 경우)
    // 콘텐츠를 로드
    // bogus 폴트의 case - 지연 로딩 페이지 , 스왑 아웃 페이지, 쓰기 보호페이지 (extra)
    // 지연 로딩 페이지의 경우 - vm_alloc_page_with_initializer 함수에서 세팅해 놓은 초기화 함수를 호출
    // process.c 의 lazy_load_segment 함수 구현해야 함
    return false;
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
    // va에 page를 할당
    // page = (struct page *)malloc(sizeof(page)); // 의문 : page 메모리 영역이 없는데 spt_find_page 를 먼저 사용해서 어케 page를 할당받음 ? -.-
    // page->va = va;

    // 해당 page에 프레임을 할당 
    page = spt_find_page(&thread_current()->spt,va);
    if (page == NULL)
    {
        return false;
    }
    
    return vm_do_claim_page(page);
}

/* Claim the PAGE and set up the mmu. */
static bool vm_do_claim_page(struct page *page) {
    struct frame *frame = vm_get_frame();
    struct thread *curr = thread_current();
    /* Set links */
    frame->page = page;
    page->frame = frame;

    /* TODO: Insert page table entry to map page's VA to frame's PA. */
    // 가상주소와 물리주소를 매핑한 정보를 진짜 페이지 테이블인 pml4에 추가
    // 성공하면 true, 실패하면 false 
    if (frame->page != NULL) {
        if (!pml4_set_page(curr->pml4,page->va,frame->kva,page->writable))
            return false;
    }
 
    return swap_in(page, frame->kva); // 물리 메모리에 페이지를 올리는 과정 (데이터는 안 올라감 ? )
}

/* Initialize new supplemental page table */
void supplemental_page_table_init(struct supplemental_page_table *spt UNUSED) {
    
    hash_init(&spt->spt_hash, page_hash, page_less, NULL);
}

/* Copy supplemental page table from src to dst */
bool supplemental_page_table_copy(struct supplemental_page_table *dst UNUSED, struct supplemental_page_table *src UNUSED) {
    // src 부터 dst 까지 보조 페이지 테이블 복사
    // src 의 보조 페이지 테이블을 반복하면서, 목적지 보조 테이블의 엔트리의 정확한 복사본을 만들기

    struct hash_iterator i; 

	hash_first(&i, &src->spt_hash);
	while (hash_next(&i))
	{
        struct page *page = hash_entry(hash_cur(&i), struct page, spt_entry); // 해당 페이지들을 가져와서, 목적지 보조 테이블의 엔트리에 복사본 삽입 ? 

        // 매핑됐냐 안 됐냐로 판단
        // 부모가 매핑이 안 됐으면, 즉 uninit이면 그 페이지를 그대로 spt에 복사해준다.
        // 부모가 매핑 됐으면 즉, uninit 이 아니면 페이지를 할당해주고, 즉시 매핑
        enum vm_type type = page_get_type(page);
        if (page->operations->type == VM_TYPE(VM_UNINIT)) // 부모가 매핑이 안 됐으면, 즉 uninit이면 그 페이지를 그대로 spt에 복사해준다.
         {  
            bool ok = vm_alloc_page_with_initializer(type, page->va,page->writable, page->uninit.init, page->uninit.aux); // 페이지 생성후 보조 페이지 테이블에 넣기까지 성공
            if (!ok)
                return false;
            
        } else 
        {   
            bool ok = vm_alloc_page(type, page->va, page->writable);
            if (!ok)
                return false;
            struct page *child_page = spt_find_page(dst, page->va);
            vm_do_claim_page(child_page);
            memcpy(child_page->frame->kva, page->frame->kva, PGSIZE);
        }
	}
    return true;
}

/* Free the resource hold by the supplemental page table */
void supplemental_page_table_kill(struct supplemental_page_table *spt UNUSED) {
    /* TODO: Destroy all the supplemental_page_table hold by thread and
     * TODO: writeback all the modified contents to the storage. */
    /* TODO: 스레드가 보유한 모든 추가 페이지 테이블을 파괴하고, */
    /* TODO: 수정된 모든 내용을 저장소에 다시 쓰세요. */

    // 보조 페이지에 의해 유지되던 모든 자원 free 
    // process_exit 할 때 호출 , 페이지 엔트리 반복하면서 페이지에 destroy 
    
    hash_clear(spt, clear_action_func);

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







