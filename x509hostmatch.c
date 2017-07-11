#if !defined(__STDC_VERSION__) ||  __STDC_VERSION__ < 199901L
#error "Needs C99"
#endif
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/param.h>

#define MAX_HOSTNAME_LENGTH 253
#define MAX_LABEL_LENGTHBEL_LENGTH 63

const char* star(const char* sANData, const size_t sANDataSize)
{
    ptrdiff_t starPosition = 0;
    char* safeptr = NULL;
    bool starFound = false;
    size_t labelCount = 0;
    char ptr[MAX_HOSTNAME_LENGTH] = {0};
    memcpy(ptr, sANData, MIN(MAX_HOSTNAME_LENGTH, sANDataSize));

    for (char* label = strtok_r(ptr, ".", &safeptr); label != NULL; label = strtok_r(NULL, ".", &safeptr), labelCount++) {
        char* currentStar = strchr(label, '*'); // \0-temination is guaranteed by strtok_r
        size_t labelLength = strlen(label);  // \0-temination is guaranteed by strtok_r
        if (currentStar != NULL){
            if (labelLength > 1 && (NULL != strchr(currentStar + 1, '*'))) {
                // only one star per label
                return NULL;
            }
            if (starFound) {
                // only one star per pattern
                return NULL;
            } else {
                starFound = true;
            }

            if (0 == strncasecmp(label, "xn--", MIN(labelLength, 4))) {
                // cannot match partial idna
                return NULL;
            }
            if (labelCount < 1) {
                // first label, star is ok.
                starPosition = currentStar - ptr;
            } else {
                // star not permissible in non-leftmost
                return NULL;
            }
        }
    }

    if (!starFound) {
        // no star found, nothing to report
        return NULL;
    } else {
        if (labelCount < 3) {
            // star in last two labels
            return NULL;
        }
    }

    return sANData + starPosition;
}

bool  matches(const char* sANData, const char* serverName)
{
    size_t serverNameSize = strnlen(serverName, MAX_HOSTNAME_LENGTH);
    size_t sANDataSize = strnlen(sANData, MAX_HOSTNAME_LENGTH);

    // Normalize by dropping optional trailing dots.
    if  (sANData[sANDataSize - 1] == '.') {
        sANDataSize--;
    }
    if  (serverName[serverNameSize - 1] == '.') {
        serverNameSize--;
    }

    // direct match wins
    if ((serverNameSize == sANDataSize) &&
        (0 == strncasecmp(sANData, serverName, sANDataSize))) {
        return true;
    }

    // No star found, but no direct match either => bail
    const char* starPosition = star(sANData, sANDataSize);
    if (starPosition == NULL) {
        return false;
    }

    ptrdiff_t prefixLength = starPosition - sANData;
    ptrdiff_t suffixLength = (sANData + sANDataSize - 1) - starPosition;
    ptrdiff_t matchLength = serverNameSize - (suffixLength + prefixLength);
    const char* sANDataSuffix = starPosition + 1;
    const char* serverNameSuffix = serverName + serverNameSize - suffixLength;

    // check that prefix matches.
    if (0 != strncasecmp(sANData, serverName, (size_t) prefixLength)) {
            return false;
    }
    // check that suffix matches
    if (0 != strncasecmp(sANDataSuffix, serverNameSuffix, (size_t) suffixLength)) {
        return false;
    }

    // complete star labels (*.example.com) must match at least one character
    if (prefixLength == 0 && sANDataSuffix[0] == '.' && matchLength < 1) {
        return false;
    }
    // no more than one serverName label can match the star -> cannot contain periods
    if (matchLength > 0 && (NULL != memchr(serverName + prefixLength, '.', matchLength ))) {
        return false;
    }

    return true;
}

struct entry {
    char* pattern;
    char* data;
    bool shouldMatch;
};

int main(int argc, char** argv)
{
    struct entry harness[] = {
        // OpenSSL v3nametest
        { "example.com",                "example.com",                    true,  },
        { "www.example.com",            "www.example.com",                true,  },
        { "example.com",                "www.example.com",                false, },
        { "www.example.com",            "example.com",                    false, },
        { "*.example.com",              "a.example.com",                  true,  },
        { "*.example.com",              "b.example.com",                  true,  },
        { "*.example.com",              "www.example.com",                true,  },
        { "*.example.com",              "xn--rger-koa.example.com",       true,  },
        { "*.www.example.com",          "test.www.example.com",           true,  },
        /* { "*.www.example.com",          ".www.example.com",               true,  }, */
        { "*www.example.com",           "www.example.com",                true,  },
        /* { "test.www.example.com",       ".www.example.com",               true,  }, */
        { "*.xn--rger-koa.example.com", "www.xn--rger-koa.example.com",   true,  },
        { "*.xn--bar.com",              "xn--foo.xn--bar.com",            true,  },
        { "*.good--example.com",        "www.good--example.com",          true,  },
        /* { "*.www.example.com",          ".www.example.com",               true,  }, */
        /* { "test.www.example.com",       ".www.example.com",               true,  }, */
        { "t*.example.com",             "test.example.com",               true,  }, // microsoft does not like but what do
        { "*t.example.com",             "test.example.com",               true,  },
        // from https://en.wikipedia.org/wiki/Wildcard_certificate
        { "*.example.com",              "www.example.com",                true,  },
        { "*.example.com",              "example.com",                    false, },
        { "*.example.com",              "zzz.www.example.com",            false, },
        { "f*.example.com",             "frog.example.com",               true,  },
        { "f*.example.com",             "frog.super.example.com",         false, },
        { "baz*.example.net",           "baz1.example.net",               true,  },
        { "*baz.example.net",           "foobaz.example.net",             true,  },
        { "b*z.example.net",            "buzz.example.net",               true,  },
        { "sub1.*.example.com",         "sub1.a.example.com",             false, },
        { "*.*.example.com",            "a.a.example.com",                false, },
        { "*.com",                      "a.com",                          false, },
        { "*",                          "example.com",                    false, },
        { "xn--caf-dma*.com",           "xn--caf-dmaa.com",               false, },
        { "xn--caf*-dma.com",           "xn--cafa-dma.com",               false, },
        { "xn--caf-dma*.example.com",   "xn--caf-dmaa.example.com",       false, },
        { "xn--caf*-dma.example.com",   "xn--cafa-dma.example.com",       false, },
        { "w*.xn--caf-dma.com",         "www.xn--caf-dma.com",            true,  },
        // RFC 6125 6.4.3
        { "bar.*.example.net",          "bar.a.example.net",              false, }, // 6.4.3.1
        { "*.example.com",              "foo.example.com",                true,  }, // 6.4.3.2
        { "*.example.com",              "bar.foo.example.com",            false, }, // 6.4.3.2
        { "*.example.com",              "example.com",                    false, }, // 6.4.3.2
        { "baz*.example.net",           "baz1.example.net",               true,  }, // 6.4.3.3
        { "*baz.example.net",           "foobaz.example.net",             true,  }, // 6.4.3.3
        { "b*z.example.net",            "buzz.example.net",               true,  }, // 6.4.3.3
        { "xn--caf*-dpa.example.net",   "xn--cafa-dpa.example.net",       false, }, // 6.4.3.3
        { "xn--caf*-dpa.example.net",   "xn--cafaa-dsa.example.net",      false, }, // 6.4.3.3
        { "xn--*caf-epa.example.net",   "xn--acaf-epa.example.net",       false, }, // 6.4.3.3
        { "xn--c*f-dma.example.net",    "xn--cxf-dma.example.net",        false, }, // 6.4.3.3
        // Microsoft https://support.microsoft.com/en-us/help/258858/accepted-wildcards-used-by-server-certificates-for-server-authenticati
        { "www.example.com",            "www.example.com",                true,  },
        { "*.example.com",              "www.example.com",                true,  },
        { "w*.example.com",             "www.example.com",                true,  },
        { "ww*.example.com",            "www.example.com",                true,  },
        { "Www.Example.com",            "www.examPle.cOm",                true,  },
        /* We chose to ignore these 4 because they are a-ok for RFC 6125 */
        /* { "*www.example.com",            "awww.example.com",              false, }, */
        /* { "*w.example.com",              "aw.example.com",                false, }, */
        /* { "w*w.example.com",             "waw.example.com",               false, }, */
        /* { "*ww.example.com",             "www.example.com",               false, }, */
        { "www.e*ample.com",             "www.example.com",               false, },
        { "www.*ample.com",              "www.example.com",               false, },
        { "www.ex*.com",                 "www.example.com",               false, },
        { "www.*.com",                   "www.example.com",               false, },
        { "*.com",                       "example.com",                   false, },
        { "www.example.com",             "example.com",                   false, },
        { "*.abc.com",                   "www.example.abc.com",           false, },
        { "*.*",                         "example.com",                   false, },
        { "*",                           "example",                       false, },
        { "a*.d*.example.com",           "abc.def.example.com",           false, },
        { "*.*.com.au",                  "www.example.com.au",            false, },
        { "www.*.com.au",                "www.example.com.au",            false, },
        // misc
        { "*.example.com",               "..example.com",                 false,  },
        { "*.example.com",               ".example.com",                  false,  },
        { "**.example.com",              "ab.example.com",                false,  },
        { "*x*.example.com",             "axb.example.com",               false,  },
        // Normalization: . and .
        { "example.com.",                "example.com.",                  true,  },
        { "www.example.com.",            "www.example.com.",              true,  },
        { "*.example.com.",              "a.example.com.",                true,  },
        { "*.example.com.",              "b.example.com.",                true,  },
        { "*.example.com.",              "www.example.com.",              true,  },
        { "*.example.com.",              "xn--rger-koa.example.com.",     true,  },
        { "*.www.example.com.",          "test.www.example.com.",         true,  },
        { "*www.example.com.",           "www.example.com.",              true,  },
        { "*.xn--rger-koa.example.com.", "www.xn--rger-koa.example.com.", true,  },
        { "*.xn--bar.com.",              "xn--foo.xn--bar.com.",          true,  },
        { "*.good--example.com.",        "www.good--example.com.",        true,  },
        { "t*.example.com.",             "test.example.com.",             true,  },
        { "*t.example.com.",             "test.example.com.",             true,  },
        { "*.example.com.",              "www.example.com.",              true,  },
        { "f*.example.com.",             "frog.example.com.",             true,  },
        { "baz*.example.net.",           "baz1.example.net.",             true,  },
        { "*baz.example.net.",           "foobaz.example.net.",           true,  },
        { "b*z.example.net.",            "buzz.example.net.",             true,  },
        { "w*.xn--caf-dma.com.",         "www.xn--caf-dma.com.",          true,  },
        { "*.example.com.",              "foo.example.com.",              true,  },
        { "baz*.example.net.",           "baz1.example.net.",             true,  },
        { "*baz.example.net.",           "foobaz.example.net.",           true,  },
        { "b*z.example.net.",            "buzz.example.net.",             true,  },
        { "www.example.com.",            "www.example.com.",              true,  },
        { "*.example.com.",              "www.example.com.",              true,  },
        { "w*.example.com.",             "www.example.com.",              true,  },
        { "ww*.example.com.",            "www.example.com.",              true,  },
        { "Www.Example.com.",            "www.examPle.cOm.",              true,  },
        // Normalization: . and no .
        { "example.com.",                "example.com",                   true,  },
        { "www.example.com.",            "www.example.com",               true,  },
        { "*.example.com.",              "a.example.com",                 true,  },
        { "*.example.com.",              "b.example.com",                 true,  },
        { "*.example.com.",              "www.example.com",               true,  },
        { "*.example.com.",              "xn--rger-koa.example.com",      true,  },
        { "*.www.example.com.",          "test.www.example.com",          true,  },
        { "*www.example.com.",           "www.example.com",               true,  },
        { "*.xn--rger-koa.example.com.", "www.xn--rger-koa.example.com",  true,  },
        { "*.xn--bar.com.",              "xn--foo.xn--bar.com",           true,  },
        { "*.good--example.com.",        "www.good--example.com",         true,  },
        { "t*.example.com.",             "test.example.com",              true,  },
        { "*t.example.com.",             "test.example.com",              true,  },
        { "*.example.com.",              "www.example.com",               true,  },
        { "f*.example.com.",             "frog.example.com",              true,  },
        { "baz*.example.net.",           "baz1.example.net",              true,  },
        { "*baz.example.net.",           "foobaz.example.net",            true,  },
        { "b*z.example.net.",            "buzz.example.net",              true,  },
        { "w*.xn--caf-dma.com.",         "www.xn--caf-dma.com",           true,  },
        { "*.example.com.",              "foo.example.com",               true,  },
        { "baz*.example.net.",           "baz1.example.net",              true,  },
        { "*baz.example.net.",           "foobaz.example.net",            true,  },
        { "b*z.example.net.",            "buzz.example.net",              true,  },
        { "www.example.com.",            "www.example.com",               true,  },
        { "*.example.com.",              "www.example.com",               true,  },
        { "w*.example.com.",             "www.example.com",               true,  },
        { "ww*.example.com.",            "www.example.com",               true,  },
        { "Www.Example.com.",            "www.examPle.cOm",               true,  },
        // Normalization: no . and .
        { "example.com",                 "example.com.",                  true,  },
        { "www.example.com",             "www.example.com.",              true,  },
        { "*.example.com",               "a.example.com.",                true,  },
        { "*.example.com",               "b.example.com.",                true,  },
        { "*.example.com",               "www.example.com.",              true,  },
        { "*.example.com",               "xn--rger-koa.example.com.",     true,  },
        { "*.www.example.com",           "test.www.example.com.",         true,  },
        { "*www.example.com",            "www.example.com.",              true,  },
        { "*.xn--rger-koa.example.com",  "www.xn--rger-koa.example.com.", true,  },
        { "*.xn--bar.com",               "xn--foo.xn--bar.com.",          true,  },
        { "*.good--example.com",         "www.good--example.com.",        true,  },
        { "t*.example.com",              "test.example.com.",             true,  },
        { "*t.example.com",              "test.example.com.",             true,  },
        { "*.example.com",               "www.example.com.",              true,  },
        { "f*.example.com",              "frog.example.com.",             true,  },
        { "baz*.example.net",            "baz1.example.net.",             true,  },
        { "*baz.example.net",            "foobaz.example.net.",           true,  },
        { "b*z.example.net",             "buzz.example.net.",             true,  },
        { "w*.xn--caf-dma.com",          "www.xn--caf-dma.com.",          true,  },
        { "*.example.com",               "foo.example.com.",              true,  },
        { "baz*.example.net",            "baz1.example.net.",             true,  },
        { "*baz.example.net",            "foobaz.example.net.",           true,  },
        { "b*z.example.net",             "buzz.example.net.",             true,  },
        { "www.example.com",             "www.example.com.",              true,  },
        { "*.example.com",               "www.example.com.",              true,  },
        { "w*.example.com",              "www.example.com.",              true,  },
        { "ww*.example.com",             "www.example.com.",              true,  },
        { "Www.Example.com",             "www.examPle.cOm.",              true,  },
        { NULL,                          NULL,                            false, },
    };

    size_t count = 0;
    size_t failure  = 0;
    for (struct entry* entry = harness; entry->pattern != NULL; entry++, count++) {
        bool assertionHolds = entry->shouldMatch == matches(entry->pattern, entry->data);
        if (!assertionHolds) {
            failure++;
        }
        printf(" [ %s ]\t%27s should%4s match %29s\n",
               assertionHolds ? "\033[32mOK\033[0m" : "\033[31mNO\033[0m",
               entry->pattern,
               entry->shouldMatch ? "" : " not",
               entry->data);
    }
    printf(" %3zu tests: %3zu ok, %3zu failed\n", count, count - failure, failure);
    return 0;
}
