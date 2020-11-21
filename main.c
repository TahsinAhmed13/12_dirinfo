#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>
#include <dirent.h>

char *mode_str(int); 

int main()
{
    DIR *dir = opendir("."); 

    int total_size = 0; 
    char *mode; 
    char *time; 
    char *color; 
    struct stat info; 
    struct dirent *entry = readdir(dir); 
    printf("%s %s  %s  %s   %s            %s\n", 
            "Permissions", 
            "UID", 
            "GID", 
            "Size", 
            "Date Modified", 
            "Name"); 
    while(entry)
    {
        stat(entry->d_name, &info); 
        mode = mode_str(info.st_mode); 
        time = ctime(&(info.st_mtim.tv_sec)); 
        *strchr(time, '\n') = 0; 
        if(entry->d_type & DT_DIR)          // directory
            color = "\033[34m"; 
        else if(info.st_mode / 64 % 8 & 1)  // executable 
            color = "\033[32m"; 
        else 
            color = ""; 
        printf("%s  %u %u %6lu %s %s%s\n", 
                mode,
                info.st_uid,
                info.st_gid,
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
