#include "div.h"

uint64_t div64(uint64_t dividend, uint32_t divisor, uint32_t* remainderOut) {
    uint64_t quotient;
    
    x86_div64_32(dividend, divisor, quotient, remainderOut);

    return quotient;
}
