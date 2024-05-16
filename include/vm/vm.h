#ifndef VM_VM_H
#define VM_VM_H
#include <stdbool.h>
#include "threads/palloc.h"
#include "lib/kernel/hash.h"

enum vm_type {
	/* 페이지가 초기화되지 않은 상태 */
	/* page not initialized */
	VM_UNINIT = 0,
	/* 파일과 관련이 없는 페이지, 즉 익명 페이지 */
	/* page not related to the file, aka anonymous page */
	VM_ANON = 1,
	/* 파일과 관련된 페이지 */
	/* page that realated to the file */
	VM_FILE = 2,
	/* 페이지 캐시를 보유하는 페이지, 프로젝트 4용 */
	/* page that hold the page cache, for project 4 */
	VM_PAGE_CACHE = 3,

	/* 상태 정보를 저장하는 보조 비트 플래그 */
	/* Bit flags to store state */

	/* 정보를 저장하는 보조 비트 플래그. 필요한 만큼 더 추가할 수 있습니다.
	 * 값이 int에 맞게 들어가는 한 더 추가할 수 있습니다. */
	/* Auxillary bit flag marker for store information. You can add more
	 * markers, until the value is fit in the int. */
	VM_MARKER_0 = (1 << 3),
	VM_MARKER_1 = (1 << 4),

	/* 이 값보다 큰 값은 사용하지 마십시오. */
	/* DO NOT EXCEED THIS VALUE. */
	VM_MARKER_END = (1 << 31),
};

#include "vm/uninit.h"
#include "vm/anon.h"
#include "vm/file.h"
#ifdef EFILESYS
#include "filesys/page_cache.h"
#endif

struct page_operations;
struct thread;

#define VM_TYPE(type) ((type) & 7)

/* "페이지"의 표현입니다.
 * 이것은 "부모 클래스"의 역할을 하며, "자식 클래스"인 uninit_page, file_page, anon_page, 그리고 페이지 캐시(project4)를 갖습니다.
 * 이 구조체의 미리 정의된 멤버를 제거하거나 수정하지 마십시오. */
/* The representation of "page".
 * This is kind of "parent class", which has four "child class"es, which are
 * uninit_page, file_page, anon_page, and page cache (project4).
 * DO NOT REMOVE/MODIFY PREDEFINED MEMBER OF THIS STRUCTURE. */
struct page {
	const struct page_operations *operations;
	void *va;              /* Address in terms of user space *//* 사용자 공간에서의 주소 */
	struct frame *frame;   /* Back reference for frame *//* 프레임에 대한 역참조 */

	/* 여러분의 구현 */
	/* Your implementation */
	struct hash_elem spt_entry;

	/* 각 유형의 데이터가 union에 바인딩됩니다.
	 * 각 함수는 현재 union을 자동으로 감지합니다. */
	/* Per-type data are binded into the union.
	 * Each function automatically detects the current union */
	union {
		struct uninit_page uninit;
		struct anon_page anon;
		struct file_page file;
#ifdef EFILESYS
		struct page_cache page_cache;
#endif
	};
};

/* "프레임"의 표현입니다. */
/* The representation of "frame" */
struct frame {
	void *kva;
	struct page *page;
};

/* 페이지 작업에 대한 함수 테이블입니다.
 * 이것은 C에서 "인터페이스"를 구현하는 한 가지 방법입니다.
 * "메소드"의 테이블을 구조체의 멤버로 넣고,
 * 필요할 때마다 호출합니다. */
/* The function table for page operations.
 * This is one way of implementing "interface" in C.
 * Put the table of "method" into the struct's member, and
 * call it whenever you needed. */
struct page_operations {
	bool (*swap_in) (struct page *, void *);
	bool (*swap_out) (struct page *);
	void (*destroy) (struct page *);
	enum vm_type type;
};

#define swap_in(page, v) (page)->operations->swap_in ((page), v)
#define swap_out(page) (page)->operations->swap_out (page)
#define destroy(page) \
	if ((page)->operations->destroy) (page)->operations->destroy (page)

/* 현재 프로세스의 메모리 공간의 표현입니다.
 * 이 구조체에 대해 특정 디자인을 강요하고 싶지 않습니다.
 * 이 구조체의 모든 디자인은 여러분의 몫입니다. */
/* Representation of current process's memory space.
 * We don't want to force you to obey any specific design for this struct.
 * All designs up to you for this. */
struct supplemental_page_table {
	struct hash spt_hash;
};

#include "threads/thread.h"
void supplemental_page_table_init (struct supplemental_page_table *spt);
bool supplemental_page_table_copy (struct supplemental_page_table *dst,
		struct supplemental_page_table *src);
void supplemental_page_table_kill (struct supplemental_page_table *spt);
struct page *spt_find_page (struct supplemental_page_table *spt,
		void *va);
bool spt_insert_page (struct supplemental_page_table *spt, struct page *page);
void spt_remove_page (struct supplemental_page_table *spt, struct page *page);

void vm_init (void); 
bool vm_try_handle_fault (struct intr_frame *f, void *addr, bool user,
		bool write, bool not_present);

#define vm_alloc_page(type, upage, writable) \
	vm_alloc_page_with_initializer ((type), (upage), (writable), NULL, NULL)
bool vm_alloc_page_with_initializer (enum vm_type type, void *upage,
		bool writable, vm_initializer *init, void *aux);
void vm_dealloc_page (struct page *page);
bool vm_claim_page (void *va);
enum vm_type page_get_type (struct page *page);

#endif  /* VM_VM_H */
