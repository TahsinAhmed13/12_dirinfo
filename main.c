#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

char *mode_str(int); 

int main(int argc, char *argv[])
{
    char *path; 
    if(argc > 1)
        path = argv[1]; 
    else 
    {
        printf("Dir Name: "); 
        path = (char *) malloc(100 * sizeof(char)); 
        scanf("%s", path); 
    }
    DIR *dir = opendir(path); 
    if(!dir)
    {
        printf("errno: %d\tmessg: %s\n", errno, strerror(errno)); 
        return 0; 
    }
    printf("Scanning: %s\n", path); 
    printf("\n"); 

    int total_size = 0; 
    char name[100]; 
    char *mode; 
    char *time; 
    char *color; 
    struct stat info; 
    struct dirent *entry = readdir(dir); 
    while(entry)
    {
        strcpy(name, path); 
        strcat(name, "/");  // add an extra '/' if it doesn't exist 
        strcat(name, entry->d_name);  
        stat(name, &info); 

        mode = mode_str(info.st_mode); 
        time = ctime(&(info.st_mtim.tv_sec)); 
        *strchr(time, '\n') = 0; 
        if(entry->d_type & DT_DIR)          // directory
            color = "\033[34m"; 
        else if(info.st_mode / 64 % 8 & 1)  // executable 
            color = "\033[32m"; 
        else 
            color = ""; 
        printf("%s %6lu %s %s%s\n", 
                mode,
                info.st_size,
                time, 
                color,
                entry->d_name); 
        printf("\033[0m"); // reset ansi code

        free(mode); 
        if(entry->d_type & DT_REG)
            total_size += info.st_size; 
        entry = readdir(dir); 
    }
    printf("\n"); 
    printf("Total Size: %d\n", total_size); 

    closedir(dir); 
    if(argc == 1) free(path); 
    return 0; 
}

char *access_str(int n)
{
    char *access = (char *) malloc(4 * sizeof(char)); 
    access[0] = n & 4 ? 'r' : '-'; 
    access[1] = n & 2 ? 'w' : '-'; 
    access[2] = n & 1 ? 'x' : '-'; 
    access[3] = 0; 
    return access; 
}

char *mode_str(int n)
{
    char *mode = (char *) malloc(11 * sizeof(char)); 
    mode[0] = n & S_IFDIR ? 'd' : '-'; 
    mode[1] = 0; 
    char *other = access_str(n % 8); 
    n /= 8; 
    char *group = access_str(n % 8); 
    n /= 8; 
    char *user = access_str(n % 8); 
    mode = strcat(mode, user); 
    free(user); 
    mode = strcat(mode, group); 
    free(group); 
    mode = strcat(mode, other); 
    free(other); 
    return mode; 
}
