#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "pagetable.h"
#include "swapfile.h"
#include "process.h"
#include "global.h"


/*******************************************************************************
 * GLOBALS (you will use this in the translate() function)
 */

/*
 * Current page table register, used by the processor to find the running
 * processes page table. The scheduler will update this when it dispatches
 * a new process, so you may assume it always points to the page table for
 * the /current/ process. A page table is an array of pte_t.
 */

pte_t* ptbr; /* Page Table Base Register on the CPU */

/*******************************************************************************
 * Number of entries in a page table
 */

const uint16_t CPU_NUM_PTE = ((1<<16) / page_size); /* do NOT modify */

/*******************************************************************************
 * Number of frames in memory
 */

const uint16_t CPU_NUM_FRAMES = (mem_size / page_size); /* do NOT modify */


/*******************************************************************************
 * Get the page number from a virtual address.
 * @param addr The virtual address.
 * @return     The virtual page number (VPN) for this addres.
 */

vpn_t get_vpn(vaddr_t virt_addr) {
    int offset_bits = log2(page_size);
    return virt_addr >> offset_bits;
}


/*******************************************************************************
 * Get the offset for a particular address.
 * @param addr The virtual address.
 * @return     The offset into a page for the given virtual address.
 */

offset_t get_offset(vaddr_t virt_addr) {
    return virt_addr % page_size;
}


/*******************************************************************************
 * Create a physical address from a PFN and an offset.
 * @param The physical frame number (PFN).
 * @param The offset into the page.
 * @return The physical address.
 */

paddr_t create_paddr(pfn_t pfn, offset_t offset) {
    int offset_bits = log2(page_size);
    return (pfn << offset_bits) + offset;
}


/*******************************************************************************
 * Looks up an address in the current page table and returns the translated
 * physical address. If the entry for the given page is not valid, traps to
 * the OS (simulated by calling the pagefault_handler function).
 *
 * @param virt_addr The virtual address to translate.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical address we are accessing.
 */

paddr_t translate(vaddr_t virt_addr, int write) {
    vpn_t vpn;
    offset_t offset;
    pfn_t pfn;
    paddr_t phys_addr;

    /* Sanity Check */
    assert(ptbr != NULL); /* do NOT modify */

    vpn = get_vpn(virt_addr);
    offset = get_offset(virt_addr);
    printf("Virtual Address %5.5hu (VPN: %5.5hu OFFSET: %5.5hu)\n",
           virt_addr, vpn, offset); /* do NOT modify this output statement */

    if(!ptbr[vpn].valid)
        pfn = pagefault_handler(vpn, write);
    else
    {
        pfn = ptbr[vpn].pfn;
        ptbr[vpn].used = 1;
    }

    phys_addr = create_paddr(pfn, offset);
    printf("Physical Address %5.5hu (PFN: %5.5hu OFFSET: %5.5hu)\n",
           phys_addr, pfn, offset); /* do NOT modify this output statement */
    return phys_addr;
}
