#include <stddef.h>
#include <stdint.h>

// The "practical" entrypoint for your program
extern int main();

// The linker script must define this value
extern uint32_t __stack_top;

// The start of memory must contain this interrupt vector table. The processor
// will use it to figure out where to start execution
// Ref: https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/exception-model/vector-table
struct isr_vectors {
    // Pointer to the stack
    void* stack;
    // This is were code starts execution
    void* reset;
    // The rest are interrupt handlers
    void* nmi;
    void* hard_fault;
    void* mem_mgmt_fault;
    void* bus_fault;
    void* usage_fault;
    void* reserved1;
    void* reserved2;
    void* reserved3;
    void* reserved4;
    void* sv_call;
    void* reserved_debug;
    void* pend_sv;
    void* systick;
};

// Do-nothing interrupt handler
void dummyHandler() {
    return;
}

// Interrupt handler that freezes the CPU
void freeze() {
    while (1) {}
}

// This is where execution starts. For our simple program, we just jump right
// into main()
// In a more complicated program, you might have to do some system setup, or
// initialize globals (e.g., copying initial values from rom, calling
// constructors, etc)
void resetHandler() {
    main();
}

// This table needs to live in a specific place in memory.
// __attribute__ ((section)) marks this struct for special handling in the
// linker.
// Then your linker script, you can specify the memory location for the
// .isr_vector section.
__attribute__ ((section(".isr_vector")))
const struct isr_vectors interrupt_vector_table = {
    .stack = (void*)(&__stack_top),
    .reset = resetHandler,
    .nmi = freeze,
    .hard_fault = freeze,
    .mem_mgmt_fault = freeze,
    .bus_fault = freeze,
    .usage_fault = freeze,
    .reserved1 = NULL,
    .reserved2 = NULL,
    .reserved3 = NULL,
    .reserved4 = NULL,
    .sv_call = freeze,
    .reserved_debug = NULL,
    .pend_sv = NULL,
    .systick = dummyHandler,
};
