#include "../src/var.c"

#include <stdlib.h>
#include <stdio.h>

char *names[] = {
    "var1",
    "int",
    "var2",
    "test",
    "overload"
};

int main(int argc, char **argv) {
    vtResult res = 0;
    nameid_t id = 0;
    struct namedt dt = {
        .nameLCount = 0,
        .namePCount = 4,
        .pRef = 0
    };
    
    dt.pRef = malloc(sizeof(struct nameref) * dt.namePCount);


    // PUSH ALL NAMES
    printf("Start Push\n");
    for(int i = 0; i < 4; ++i) {
        res = namedt_Push(&dt, names[i], NULL);
        if(res != VT_RESULT_SUCCESS) {
            printf("Error pushing %s : %s\n", names[i], VtStrError(res));
            return 1;
        }
    }
    res = namedt_Push(&dt, names[4], NULL);
    if(res != VT_RESULT_ERR_NO_SPACE) {
        printf("Error pushed over the limit\n");
        return 1;
    }


    for(int i = 0; i < 4; ++i) {
        printf("%s\n", dt.pRef[i].n);
    }


    // FIND THE NAMES
    printf("Start Find\n");
    for(int i = 0; i < 4; ++i) {
        res = namedt_Find(&dt, names[i], &id);
        if(res != VT_RESULT_SUCCESS) {
            printf("Error failed to find name: %s\n", names[i]);
            return 1;
        }
        printf("name \"%s\" was at : %d\n", names[i], id);
    }
    
    res = namedt_Find(&dt, names[4], &id);
    if(res != VT_RESULT_FAILED) {
        printf("Error found invalid name.\n");
        return 1;
    }


    // DELETE THE NAMES
    printf("Start Pull\n");
    for(int i = 3; i >= 0; --i) {
        res = namedt_Find(&dt, names[i], &id);
        if(res != VT_RESULT_SUCCESS) {
            printf("Error failed to find name in delete: %s\n", names[i]);
            return 1;
        }
        res = namedt_Pull(&dt, id);
        if(res != VT_RESULT_SUCCESS) {
            printf("Error failed to pull name \"%s\" at %d\n", names[i], id);
        }
    }
    
}

