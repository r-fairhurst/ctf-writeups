/*
 * loicense – reconstructed from the OSU CTF binary
 *
 * gcc -Wall -Wextra -O2 -fno-stack-protector -z execstack \
 *     -o loicense_rebuild loicense_rebuild.c
 *
 * Run it and feed the licence blob on stdin, exactly as the service does.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

/* The original uses the size of its .init segment (≈ 0x1150). */
#define INIT_LEN 0x1150

/* ------------------------------------------------------------------ */
/*                           licence check                            */
/* ------------------------------------------------------------------ */
static int validate(char *buf)
{
    /* 1.  Sum every byte of the mapped area into a 64‑bit register.   */
    uint64_t sum = 0;
    for (size_t i = 0; i < INIT_LEN; ++i)
        sum += (unsigned char)buf[i];

    /* 2.  Make the region RX so the child can jump into it.           */
    if (mprotect(buf, INIT_LEN, PROT_READ | PROT_EXEC) != 0) {
        perror("mprotect");
        return 0;
    }

    /* 3.  Fork; the child does the self‑checksum trick.               */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 0;
    }

    if (pid == 0) {                       /* ---------------- child -- */
        /*
         * The real binary computes an alignment helper
         *      rdx_4 = ((sum >> 31) >> 20)        // fancy but always 0
         * then calls
         *      ((buf + ((sum + rdx_4) & 0xFFF) - rdx_4)()).
         *
         * Because we restrict the payload to sum < 4096,
         * the call target is simply  buf + (sum & 0xFFF) == buf + sum.
         */
        uint64_t (*entry)(void) = (void *)(buf + (sum & 0xFFF));
        uint64_t ret = entry();

        /* Exit with low byte of ret if it matches the checksum. */
        _exit((ret == sum) ? (uint8_t)ret : 1);
    }

    /* ---------------- parent --------------------------------------- */
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 0;
    }

    /* WIFEXITED(status) is true  ⇔  (status & 0x7f) == 0 */
    if (((status & 0x7f) != 0) ||           /* child died via signal   */
        ((status >> 8) & 0xff) != (sum & 0xff)) {   /* wrong exit code */
        puts("invalid license >:( the queen'll be hearing about this!");
        return 0;
    }

    puts("Hmmm. Seems to check out.");
    puts("fake{flag_here}");
    puts("We'll be back...");
    return 1;
}

/* ------------------------------------------------------------------ */
/*                              driver                                */
/* ------------------------------------------------------------------ */
int main(void)
{
    /* Disable stdio buffering exactly like the original. */
    setbuf(stdin,  NULL);
    setbuf(stdout, NULL);

    puts("oi! you got a loicense for this connection?");

    /* mmap RW first, will flip to RX inside validate(). */
    char *buf = mmap(NULL, INIT_LEN,
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    ssize_t n = read(STDIN_FILENO, buf, INIT_LEN);
    if (n < 0) {
        perror("read");
        return EXIT_FAILURE;
    }

    /* Ignore anything beyond INIT_LEN, exactly as the service does. */
    validate(buf);
    return EXIT_SUCCESS;
}
