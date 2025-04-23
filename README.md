⚙️ What I Built
Multi-Tasking in 4 “C” Functions: Four non-returning task handlers (T1–T4), each with its own 1 KB private stack.

Dual Stack-Pointer Magic: Switched seamlessly between MSP for the scheduler and PSP for user tasks via inline assembly.

Context-Switching Mastery: Leveraged SysTick and PendSV exceptions to save/restore R4–R11, PC, LR, XPSR, and PSP in compact, hand-crafted stack frames.

Dummy Stack-Frame Initialization: Pre-populated each task’s stack with “fake” SF1/SF2 frames so any new task can start running out of the box.

🚥 How It Works
Scheduler Trigger: SysTick fires every 1 ms (16 MHz / 1000 Hz – 1 reload) as our “heartbeat.”

Save Context: In the handler, we STMDB R4–R11 to the current task’s private stack, update its PSP, and record it globally.

Pick Next Task: Simple round-robin index (0 → 1 → 2 → 3 → 0 …).

Restore Context: Load SF2 from the next task’s stack via LDMIA, update PSP, and exit back to thread mode—automagically resuming where it left off!

💪 Skills Showcased
ARM Cortex-Mx architecture: MSP vs. PSP, exception entry/exit, special regs (EXC_RETURN magic).

Bare-metal C & Inline Assembly: Writing naked functions, using MSR/MRS, BL vs. BX nuances.

Memory & Stack Management: Careful SRAM partitioning, full-descending stacks, precise stack-frame layout.

Debug & Fault Handling: Enabled MemManage/Bus/Usage faults for rock-solid reliability.
