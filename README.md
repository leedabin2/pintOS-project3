# SW Jungle - PintOS Project
---

## 💻 Project Introduntion

**Project 1 : THREADS**        (2024.04.25 ~ 2024.05.01)
  - ✅ Alarm clock
  - ✅ Priority Scheduling
  - ❎ (Option) Advanced Scheduler
    
**Project 2 : USER PROGRAMS**  (2024.05.02 ~ 2024.05.13)
  - ✅ Argument Passing
  - ✅ User Memory Access
  - ✅ System Calls
  - ✅ Process Termination Message
  - ✅ Deny Write on Executables
  - ❎ (Option) Extend File Descriptor
    
**Project 3 : VIRTUAL MEMORY** (2024.05.14 ~ 2024.05.27)
  - ✅ Memory Management
  - ✅ Anonymous Page
  - ✅ Stack Growth
  - ✅ Memory Mapped Files
  - ✅ Swap In/Out
  - ❎ (Option) Copy-on-write
    
---

## 👥 Project Member
  - Dabin&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;https://github.com/leedabin2
    
  - Jinyong&nbsp;&nbsp;&nbsp;https://github.com/Bambamsong
    
  - Geonu&nbsp;&nbsp;&nbsp;&nbsp;https://github.com/GEONU-MOON
    
---

<details>
<summary>Test Case Result</summary>

~~~
pass tests/userprog/args-none
pass tests/userprog/args-single
pass tests/userprog/args-multiple
pass tests/userprog/args-many
pass tests/userprog/args-dbl-space
pass tests/userprog/halt
pass tests/userprog/exit
pass tests/userprog/create-normal
pass tests/userprog/create-empty
pass tests/userprog/create-null
pass tests/userprog/create-bad-ptr
pass tests/userprog/create-long
pass tests/userprog/create-exists
pass tests/userprog/create-bound
pass tests/userprog/open-normal
pass tests/userprog/open-missing
pass tests/userprog/open-boundary
pass tests/userprog/open-empty
pass tests/userprog/open-null
pass tests/userprog/open-bad-ptr
pass tests/userprog/open-twice
pass tests/userprog/close-normal
pass tests/userprog/close-twice
pass tests/userprog/close-bad-fd
pass tests/userprog/read-normal
pass tests/userprog/read-bad-ptr
pass tests/userprog/read-boundary
pass tests/userprog/read-zero
pass tests/userprog/read-stdout
pass tests/userprog/read-bad-fd
pass tests/userprog/write-normal
pass tests/userprog/write-bad-ptr
pass tests/userprog/write-boundary
pass tests/userprog/write-zero
pass tests/userprog/write-stdin
pass tests/userprog/write-bad-fd
pass tests/userprog/fork-once
pass tests/userprog/fork-multiple
pass tests/userprog/fork-recursive
pass tests/userprog/fork-read
pass tests/userprog/fork-close
pass tests/userprog/fork-boundary
pass tests/userprog/exec-once
pass tests/userprog/exec-arg
pass tests/userprog/exec-boundary
pass tests/userprog/exec-missing
pass tests/userprog/exec-bad-ptr
pass tests/userprog/exec-read
pass tests/userprog/wait-simple
pass tests/userprog/wait-twice
pass tests/userprog/wait-killed
pass tests/userprog/wait-bad-pid
pass tests/userprog/multi-recurse
pass tests/userprog/multi-child-fd
pass tests/userprog/rox-simple
pass tests/userprog/rox-child
pass tests/userprog/rox-multichild
pass tests/userprog/bad-read
pass tests/userprog/bad-write
pass tests/userprog/bad-read2
pass tests/userprog/bad-write2
pass tests/userprog/bad-jump
pass tests/userprog/bad-jump2
pass tests/vm/pt-grow-stack
pass tests/vm/pt-grow-bad
pass tests/vm/pt-big-stk-obj
pass tests/vm/pt-bad-addr
pass tests/vm/pt-bad-read
pass tests/vm/pt-write-code
pass tests/vm/pt-write-code2
pass tests/vm/pt-grow-stk-sc
pass tests/vm/page-linear
pass tests/vm/page-parallel
pass tests/vm/page-merge-seq
pass tests/vm/page-merge-par
pass tests/vm/page-merge-stk
pass tests/vm/page-merge-mm
pass tests/vm/page-shuffle
pass tests/vm/mmap-read
pass tests/vm/mmap-close
pass tests/vm/mmap-unmap
pass tests/vm/mmap-overlap
pass tests/vm/mmap-twice
pass tests/vm/mmap-write
pass tests/vm/mmap-ro
pass tests/vm/mmap-exit
pass tests/vm/mmap-shuffle
pass tests/vm/mmap-bad-fd
pass tests/vm/mmap-clean
pass tests/vm/mmap-inherit
pass tests/vm/mmap-misalign
pass tests/vm/mmap-null
pass tests/vm/mmap-over-code
pass tests/vm/mmap-over-data
pass tests/vm/mmap-over-stk
pass tests/vm/mmap-remove
pass tests/vm/mmap-zero
pass tests/vm/mmap-bad-fd2
pass tests/vm/mmap-bad-fd3
pass tests/vm/mmap-zero-len
pass tests/vm/mmap-off
pass tests/vm/mmap-bad-off
pass tests/vm/mmap-kernel
pass tests/vm/lazy-file
pass tests/vm/lazy-anon
pass tests/vm/swap-file
pass tests/vm/swap-anon
pass tests/vm/swap-iter
pass tests/vm/swap-fork
pass tests/filesys/base/lg-create
pass tests/filesys/base/lg-full
pass tests/filesys/base/lg-random
pass tests/filesys/base/lg-seq-block
pass tests/filesys/base/lg-seq-random
pass tests/filesys/base/sm-create
pass tests/filesys/base/sm-full
pass tests/filesys/base/sm-random
pass tests/filesys/base/sm-seq-block
pass tests/filesys/base/sm-seq-random
pass tests/filesys/base/syn-read
pass tests/filesys/base/syn-remove
pass tests/filesys/base/syn-write
pass tests/threads/alarm-single
pass tests/threads/alarm-multiple
pass tests/threads/alarm-simultaneous
pass tests/threads/alarm-priority
pass tests/threads/alarm-zero
pass tests/threads/alarm-negative
pass tests/threads/priority-change
pass tests/threads/priority-donate-one
pass tests/threads/priority-donate-multiple
pass tests/threads/priority-donate-multiple2
pass tests/threads/priority-donate-nest
pass tests/threads/priority-donate-sema
pass tests/threads/priority-donate-lower
pass tests/threads/priority-fifo
pass tests/threads/priority-preempt
pass tests/threads/priority-sema
pass tests/threads/priority-condvar
pass tests/threads/priority-donate-chain
FAIL tests/vm/cow/cow-simple
1 of 141 tests failed.
~~~

</details>

---

### Books that may be helpful
  - “Operating Systems: Three Easy Pieces”

    한글 번역판 링크 : https://github.com/remzi-arpacidusseau/ostep-translations/tree/master/korean

  - "Computer System : A Programmer's Perspective"

---
Brand new pintos for Operating Systems and Lab (CS330), KAIST, by Youngjin Kwon.

The manual is available at https://casys-kaist.github.io/pintos-kaist/.
