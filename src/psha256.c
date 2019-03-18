/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
// parallel file hashing

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define HASH_CHUNKS 1024
#define HASH_CHUNK_SIZE 1048576
#define WORKER_CHUNK_SIZE ((HASH_CHUNKS * HASH_CHUNK_SIZE))

#define CHECK_CRYPTO(C)                                                                        \
    do {                                                                                       \
        if ((C) != 1) {                                                                        \
            fprintf(stderr, "libcrypto error: %s\n", ERR_error_string(ERR_get_error(), NULL)); \
            exit(1);                                                                           \
        }                                                                                      \
    } while (0)

typedef struct {
    unsigned chunk_num;
    unsigned char digest[SHA256_DIGEST_LENGTH];
} digest_val;

typedef struct {
    unsigned size;
    unsigned end;
    digest_val *elems;
} digest_heap;

void realloc_heap(digest_heap *dheap, unsigned size);
void insert_heap_element(unsigned cnum, unsigned char *data, digest_heap *dheap);
void heap_swap(unsigned idx1, unsigned idx2, digest_heap *dheap);
void heap_pop(digest_heap *dheap);

int main(int argc, char **argv) {
    char *check_hash = NULL;
    int npar = 0;
    int opt_ret;
    bool found_err = false;
    while ((opt_ret = getopt(argc, argv, "c:p:")) >= 0) {
        switch (opt_ret) {
            case 'p':
                npar = atoi(optarg);
                break;

            case 'c':
                check_hash = optarg;
                break;

            default:
                found_err = true;
        }
        if (found_err) {
            break;
        }
    }
    if (found_err || (optind + 1 != argc)) {
        printf("Usage: %s [-c <check_hash>] [-p <npar>] <filename>\n", argv[0]);
        exit(1);
    }
    int fd;
    // stat the file
    struct stat fdstat;
    if (stat(argv[optind], &fdstat) < 0) {
        fprintf(stderr, "Could not stat %s: %s\n", argv[optind], strerror(errno));
        exit(1);
    }
    // open the file
    if ((fd = open(argv[optind], O_RDONLY)) < 0) {
        fprintf(stderr, "Could not open %s: %s\n", argv[optind], strerror(errno));
        exit(1);
    }
    // and mmap it
    char *fbytes = NULL;
    if ((fbytes = (char *)mmap(NULL, fdstat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "Could not mmap %s: %s\n", argv[optind], strerror(errno));
        exit(1);
    }
    // now that it looks like we've got a job to do, load libcrypto errors
    ERR_load_crypto_strings();

    // number of chunks
    unsigned nchunks = fdstat.st_size / WORKER_CHUNK_SIZE;
    unsigned lastlen = fdstat.st_size % WORKER_CHUNK_SIZE;
    if (npar < 2 || nchunks < 2) {
        npar = 0;
    }

    digest_heap dheap = {0, 0, NULL};
    realloc_heap(&dheap, npar);
    int *cpids = (int *)calloc(npar, sizeof(cpids[0]));
    int *pipes = (int *)calloc(npar, sizeof(pipes[0]));
    if (cpids == NULL || pipes == NULL) {
        perror("Allocating child pids, pipes, or heap");
        exit(1);
    }
    int pnums[2] = {
        0,
    };
    int num_children = 0;
    bool launching = true;
    bool finishing = false;
    bool finished = false;
    int last_chunk_seen = -1;
    SHA256_CTX master_ctx;
    CHECK_CRYPTO(SHA256_Init(&master_ctx));
    unsigned chunk_num = 0;

    while (npar > 0 || chunk_num < nchunks) {
        int child_num = -1;

        if (npar > 0) {
            if (launching & !finishing) {
                child_num = num_children++;
                if (num_children == npar) {
                    launching = false;
                }
            } else {
                int cstat;
                pid_t cpid = wait(&cstat);
                if (WIFEXITED(cstat) && (WEXITSTATUS(cstat) != 0)) {
                    fprintf(stderr, "Child exited abnormally.\n");
                    kill(-2, SIGTERM);
                    exit(1);
                }

                if (cpid < 0) {
                    // wait will return error if we have no children
                    if (errno == ECHILD) {
                        // all done
                        goto master_hash;
                    } else {
                        // oops, real error
                        perror("Wait error");
                        kill(-2, SIGTERM);
                        exit(1);
                    }
                }

                // find child pid
                for (child_num = 0; child_num < npar; child_num++) {
                    if (cpids[child_num] == cpid) {
                        break;
                    }
                }

                // did we actually find a child?
                if (child_num == npar) {
                    fprintf(stderr, "Unexpected child pid death %d\n", cpid);
                    kill(-2, SIGTERM);
                    exit(1);
                }

                // read data from child
                unsigned cnum;
                unsigned char data[SHA256_DIGEST_LENGTH];
                if ((read(pipes[child_num], &cnum, sizeof(cnum)) < 0) ||
                    (read(pipes[child_num], data, SHA256_DIGEST_LENGTH) < 0)) {
                    perror("Read from child failed");
                    kill(-2, SIGTERM);
                    exit(1);
                } else {
                    close(pipes[child_num]);
                    insert_heap_element(cnum, data, &dheap);
                }

                if (finishing && (--num_children == 0)) {
                    // no more children
                    finished = true;
                }
            }

            if (child_num == -1) {
                fprintf(stderr, "Somehow didn't find a child num. Giving up.\n");
                kill(-2, SIGTERM);
                exit(1);
            }

        master_hash:
            while ((dheap.end > 0) && ((last_chunk_seen + 1) == ((int)dheap.elems[0].chunk_num))) {
                last_chunk_seen++;
                CHECK_CRYPTO(SHA256_Update(&master_ctx, dheap.elems[0].digest, SHA256_DIGEST_LENGTH));
                heap_pop(&dheap);
            }

            if (finished) {
                break;
            }

            if (finishing) {
                continue;
            }

            if (pipe(pnums) < 0) {
                perror("Failed to open child pipe");
                kill(-2, SIGTERM);
                exit(1);
            }
            pipes[child_num] = pnums[0];
        }

        // parent forks
        if (npar > 0 && (cpids[child_num] = fork()) != 0) {
            close(pnums[1]);
            if (++chunk_num >= nchunks) {
                finishing = true;
            }
        } else {
            for (int i = 0; i < num_children; i++) {
                close(pipes[i]);
            }
            // child
            SHA256_CTX sctx;
            CHECK_CRYPTO(SHA256_Init(&sctx));

            for (unsigned i = 0; i < HASH_CHUNKS; i++) {
                CHECK_CRYPTO(SHA256_Update(
                    &sctx, fbytes + chunk_num * WORKER_CHUNK_SIZE + i * HASH_CHUNK_SIZE, HASH_CHUNK_SIZE));
            }
            unsigned char out[SHA256_DIGEST_LENGTH];
            CHECK_CRYPTO(SHA256_Final(out, &sctx));

            if (npar == 0) {
                CHECK_CRYPTO(SHA256_Update(&master_ctx, out, SHA256_DIGEST_LENGTH));
                ++chunk_num;
            } else if ((write(pnums[1], &chunk_num, sizeof(chunk_num)) < 0) ||
                       (write(pnums[1], out, SHA256_DIGEST_LENGTH) < 0)) {
                perror("Child write failed");
                exit(1);
            } else {
                exit(0);
            }
        }
    }

    // sanity check
    if (dheap.end != 0) {
        fprintf(stderr, "Heap is not empty after children finished. Giving up.\n");
        exit(1);
    }

    if (cpids != NULL) {
        free(cpids);
    }
    if (pipes != NULL) {
        free(pipes);
    }
    if (dheap.elems != NULL) {
        free(dheap.elems);
    }

    if (lastlen != 0) {
        unsigned remaining = fdstat.st_size - nchunks * WORKER_CHUNK_SIZE;
        unsigned nparts = remaining / HASH_CHUNK_SIZE;
        unsigned left = remaining % HASH_CHUNK_SIZE;
        for (unsigned i = 0; i < nparts || (i == nparts && left > 0); i++) {
            CHECK_CRYPTO(SHA256_Update(&master_ctx,
                                       fbytes + nchunks * WORKER_CHUNK_SIZE + i * HASH_CHUNK_SIZE,
                                       i == nparts ? left : HASH_CHUNK_SIZE));
        }
    }

    unsigned char out[SHA256_DIGEST_LENGTH];
    CHECK_CRYPTO(SHA256_Final(out, &master_ctx));
    char out_string[2 * SHA256_DIGEST_LENGTH + 1];
    for (unsigned i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(out_string + 2 * i, "%02hhx", out[i]);
    }

    if (check_hash != NULL) {
        bool check_failed = strncmp(out_string, check_hash, 2 * SHA256_DIGEST_LENGTH + 1) != 0;
        printf("%s: %s\n", argv[optind], check_failed ? "FAILED" : "OK");
        return check_failed ? 1 : 0;
    } else {
        printf("%s  %s\n", out_string, argv[optind]);
        return 0;
    }
}

void realloc_heap(digest_heap *dheap, unsigned size) {
    dheap->elems = (digest_val *)realloc(dheap->elems, size * sizeof(dheap->elems[0]));
    if (dheap->elems == NULL) {
        perror("Allocating value heap");
        exit(1);
    }
    dheap->size = size;
}

#define heap_lt(idx1, idx2) (dheap->elems[idx1].chunk_num < dheap->elems[idx2].chunk_num)
void insert_heap_element(unsigned cnum, unsigned char *data, digest_heap *dheap) {
    // make sure we have enough space
    if (dheap->end >= dheap->size) {
        realloc_heap(dheap, dheap->size * 2);
    }

    // construct the element
    int idx = dheap->end;
    dheap->end += 1;
    dheap->elems[idx].chunk_num = cnum;
    memcpy(dheap->elems[idx].digest, data, SHA256_DIGEST_LENGTH);

    // heapify
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap_lt(idx, parent)) {
            heap_swap(idx, parent, dheap);
            idx = parent;
        } else {
            break;
        }
    }
}

void heap_pop(digest_heap *dheap) {
    // invariant: this function is called only when dheap->end > 0, so safe not to check
    dheap->end -= 1;
    if (dheap->end > 0) {
        // move last element into head position
        memcpy(dheap->elems, dheap->elems + dheap->end, sizeof(dheap->elems[0]));
    }

    // heapify;
    unsigned idx = 0;
    while (idx < dheap->end) {
        unsigned rchild = 2 * (idx + 1);
        unsigned lchild = rchild - 1;
        unsigned nidx;
        if (lchild >= dheap->end) {
            break;
        } else if (rchild >= dheap->end) {
            nidx = lchild;
        } else {
            nidx = heap_lt(rchild, lchild) ? rchild : lchild;
        }
        if (heap_lt(nidx, idx)) {
            heap_swap(nidx, idx, dheap);
            idx = nidx;
        } else {
            break;
        }
    }
}
#undef heap_lt

void heap_swap(unsigned idx1, unsigned idx2, digest_heap *dheap) {
    digest_val temp;
    memcpy(&temp, dheap->elems + idx1, sizeof(temp));
    memcpy(dheap->elems + idx1, dheap->elems + idx2, sizeof(temp));
    memcpy(dheap->elems + idx2, &temp, sizeof(temp));
}
