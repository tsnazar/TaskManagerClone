#include "RabinKarpAlg.h"

namespace
{
    constexpr int  NUM_OF_CHAR = 256;
    constexpr int  PRIME = 7;
}

int RabinKarp(const std::string& Pat, const std::string& Text)
{
    int M = Pat.size();
    int N = Text.size();
    int i, j;
    int p = 0; // hash value for pattern
    int t = 0; // hash value for txt
    int h = 1;

    // The value of h would be "pow(d, M-1)%PRIME"
    for (i = 0; i < M - 1; i++)
        h = (h * NUM_OF_CHAR) % PRIME;

    // Calculate the hash value of pattern and first
    // window of text
    for (i = 0; i < M; i++) {
        p = (NUM_OF_CHAR * p + Pat[i]) % PRIME;
        t = (NUM_OF_CHAR * t + Text[i]) % PRIME;
    }

    // Slide the pattern over text one by one
    for (i = 0; i <= N - M; i++) {

        // Check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        if (p == t) {
            /* Check for characters one by one */
            for (j = 0; j < M; j++) {
                if (Text[i + j] != Pat[j]) {
                    break;
                }
            }

            // if p == t and pat[0...M-1] = txt[i, i+1,
            // ...i+M-1]

            if (j == M)
                return i;
        }

        // Calculate hash value for next window of text:
        // Remove leading digit, add trailing digit
        if (i < N - M) {
            t = (NUM_OF_CHAR * (t - Text[i] * h) + Text[i + M]) % PRIME;

            // We might get negative value of t, converting
            // it to positive
            while (t < 0)
                t = (t + PRIME);
        }
    }

    return -1;
}