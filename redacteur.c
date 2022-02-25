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

int taille(char *chaine);

int main(int argc, char *argv[]) {
    key_t cle;
    int msgid, type;
    MSGRQ *monMessage = NULL;

    if(argc != 3) {
        perror("argc");
        return EXIT_FAILURE;
    }

    if(!atoi(argv[1])) {
        perror("argv[1]");
        return EXIT_FAILURE;
    }

    type = atoi(argv[1]);

    if(taille(argv[2]) > 20) {
        perror("argv[2]");
        return EXIT_FAILURE;
    }

    cle = ftok("/usr/include/err.h", 0);
    msgid = msgget(cle, IPC_CREAT | IPC_EXCL | 0600);
    if(msgid == -1) {
        msgid = msgget(cle, IPC_EXCL);
        if(msgid == -1) {
            perror("msgget");
            return EXIT_FAILURE;
        }
    }

    monMessage = malloc(sizeof(*monMessage));
    if(monMessage == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    monMessage->type = type;
    strcpy(monMessage->message, argv[2]);

    if(msgsnd(msgid, monMessage, (sizeof(MSGRQ) - sizeof(int)), 0) == -1) {
        perror("msgsnd");
        return EXIT_FAILURE;
    }

    printf("Message envoyee !");

    return 0;
}

int taille(char *chaine) {
    int i, sum = 0;
    for(i = 0; chaine[i] != '\0'; i++) {
        sum++;
    }
    return sum;
}
