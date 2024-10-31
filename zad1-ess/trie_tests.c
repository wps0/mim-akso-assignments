#include <stdlib.h>
#include <stdio.h>

#include "trie.h"
#include "groups.h"


int main() {
    const int BUF_SIZE = 256;
    char *buf = malloc(sizeof(char)*BUF_SIZE);

    scanf("%255s", buf);

    trie_t *t = trie_make(buf);

    while (1) {
        char cmd;
        scanf(" %c", &cmd);
        if (cmd == 'e')
        {
            break;
        }

        scanf("%255s", buf);
        if (cmd == 'a')
            trie_add_string(t, buf);
        else if (cmd == 'r')
            trie_remove_string(t, buf);
    
        trie_print(t);
    }
    trie_free(t);
    free(buf);
}