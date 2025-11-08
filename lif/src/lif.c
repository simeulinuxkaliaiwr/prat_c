#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

char* tipo_arquivo(int d_type) {
    if (d_type == DT_REG) return "ARQ";
    if (d_type == DT_DIR) return "DIR";
    if (d_type == DT_LNK) return "LNK";
    return "???";
}

void mostra_dir(char* caminho, int nivel, int rec) {
    DIR *dir = opendir(caminho);
    if (!dir) {
        perror("Erro abrindo diretorio");
        return;
    }

    struct dirent *entry;
    char temp[1024];
    
    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) 
            continue;

        for (int i = 0; i < nivel * 2; i++) printf(" ");
        printf("|-%s %s\n", tipo_arquivo(entry->d_type), entry->d_name);

        if (rec && entry->d_type == DT_DIR) {
            snprintf(temp, sizeof(temp), "%s/%s", caminho, entry->d_name);
            mostra_dir(temp, nivel + 1, rec);
        }
    }
    
    closedir(dir);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <pasta> [recursivo]\n", argv[0]);
        return 1;
    }

    int rec = 0;
    if (argc > 2 && (!strcmp(argv[2], "-r") || !strcmp(argv[2], "-R"))) {
        rec = 1;
    }

    mostra_dir(argv[1], 0, rec);
    return 0;
}
