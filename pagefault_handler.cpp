#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "swapfile.h"
#include "process.h"
#include "global.h"

pfn_t get_free_frame(void);


/*******************************************************************************
 * Page fault handler. When the CPU encounters an invalid address mapping in a
 * process' page table, it invokes the CPU via this handler. The OS then
 * allocates a physical frame for the reuested page (either by using a free
 * frame or evicting one), changes the process' page table to reflect the
 * mapping and then restarts the interrupted process.
 *
 * @param vpn The virtual page number requested.
 * @param write If the CPU is writing to the page, this is 1. Otherwise, it's 0.
 * @return The physical frame the OS has mapped to the virtual page.
 */

pfn_t pagefault_handler(vpn_t request_vpn, int write) {
    pfn_t victim_pfn;
    vpn_t victim_vpn;
    pcb_t *victim_pcb;

    /* Sanity Check */
    assert(ptbr != NULL); /* do NOT modify */

    /* Add 1 to the Page Fault count */
    count_pagefaults++; /* do NOT modify */

    /* Find a free or victim frame */
    victim_pfn = get_free_frame();
    assert(victim_pfn < CPU_NUM_FRAMES); /* make sure the victim_pfn is valid */

    /* Use the Frame Table (reverse lookup table) to find the victim. */
    victim_vpn = frametable[victim_pfn].vpn;
    victim_pcb = frametable[victim_pfn].pcb; /* NULL if the frame was free */

    printf("****PAGE FAULT has occurred at VPN %u. Evicting (PFN %u VPN %u) as victim.\n",
           request_vpn, victim_pfn, victim_vpn); /* do NOT modify this output statement */

    //if the victim page is occupied
    if (victim_pcb != NULL)
    {
        //invalidate the page's entry in the victim's page table.
        victim_pcb->pagetable[victim_vpn].valid = 0;
        //if dirty, save page to disk
        if(victim_pcb->pagetable[victim_vpn].dirty)
            save_page_to_disk(victim_pfn, victim_vpn, victim_pcb->pid);
    }

    load_page_from_disk(victim_pfn, request_vpn, current->pid);

    //update current process page table
    ptbr[request_vpn].pfn = victim_pfn;
    ptbr[request_vpn].valid = 1;
    ptbr[request_vpn].dirty = 0;
    ptbr[request_vpn].used = 0;

    //update the frame table so that it knows the new page stored in the frame
    frametable[victim_pfn].vpn = request_vpn;
    frametable[victim_pfn].pcb = current;

    return victim_pfn;
}


/*******************************************************************************
 * Finds a free physical frame. If none are available, uses a clock sweep
 * algorithm to find a used frame for eviction.
 *
 * @return The physical frame number of a free (or evictable) frame.
 */

pfn_t get_free_frame(void) {
    unsigned int i;

    /* See if there are any free frames */
    /* do NOT modify this for loop */
    for (i = 0; i < CPU_NUM_FRAMES; i++)
        if (frametable[i].pcb == NULL)
            return i;

    /* TODO: Part 4 //12 lines
     * IMPLEMENT A CLOCK SWEEP ALGORITHM HERE
     * Note: Think of what kinds of frames can you return before you decide
     * to evict one of the pages using the clock sweep and return that frame
     */
    int pointer = 0;


    /* If all else fails, return a random frame */
    return rand() % CPU_NUM_FRAMES;
}

