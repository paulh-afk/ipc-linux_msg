#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

typedef struct MSGRQ MSGRQ;
struct MSGRQ {
    int type;
    char message[20];
};

int main(int argc, char* argv[]) {
    key_t cle;
    int msgid, type;
    MSGRQ *monMessage = NULL;

    if(argc != 2) {
        perror("argc");
        return EXIT_FAILURE;
    }

    if(!atoi(argv[1])) {
        perror("argv[1]");
        return EXIT_FAILURE;
    }

    cle = ftok("/usr/include/err.h", 0);
    type = atoi(argv[1]);

    msgid = msgget(cle, IPC_EXCL | 0600);
    if(msgid == -1) {
        perror("MSG Queue");
        return EXIT_FAILURE;
    }

    monMessage = malloc(sizeof(*monMessage));

    if(monMessage == NULL) {
        return EXIT_FAILURE;
    }

    if(msgrcv(msgid, monMessage, (sizeof(MSGRQ) - sizeof(int)), 0, IPC_NOWAIT) == -1) {
        perror("Aucun message");
        return EXIT_FAILURE;
    }

    printf("Type: %d\nContenu: %s\n", monMessage->type, monMessage->message);

    return 0;
}
