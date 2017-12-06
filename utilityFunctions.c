//
// Created by Jamie on 06/12/2017.
//

/**
 * Counter function that takes bits in a counter, pointer to the counter and base of counting and then returns 1 if the counter has finished and if not returns 0 and increments the coutner by 1
 *
 * @param bits      Bits in the counter
 * @param p         Pointer to counter
 * @param base      Base of counting
 * @return          If the counter has finished
 */
int nBitCounterAddOne(int bits, int *p, int base) {
    int finished = 1;
    for (int i = 0; i < bits; i++) {
        if (p[i] < base - 1) {
            finished = 0;
            break;
        }
    }

    if (finished) {
        return finished;
    }

    p[0] = p[0] + 1;

    for (int i = 0; i < bits; i++) {
        if (p[i] >= base) {
            if (i < bits - 1) {
                if (p[i + 1] < base - 1) {
                    p[i + 1] = p[i + 1] + 1;
                    p[i] = 0;
                }
            }
        }
    }

    return 0;
}