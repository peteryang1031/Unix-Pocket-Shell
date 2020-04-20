#include "Header.h"
#include"ls.h"
void dirwalk(char *dir, void (*fcn)(char *))
{
        char name[1024];
        struct dirent *dp;
        DIR *dfd;
        if ((dfd = opendir(dir)) == NULL)
        {
                perror("opendir");
                exit(1);
        }
        while ((dp = readdir(dfd)) != NULL)
        {
                if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                {
                        continue;
                }
                sprintf(name, "%s/%s", dir, dp->d_name);
                (*fcn)(name);
        }
        closedir(dfd);
}

void fsize(char *name)
{
        struct stat stbuf;
        if (stat(name, &stbuf) == -1)
        {
                perror("stat");
                exit(1);
        }
        if ((stbuf.st_mode & __S_IFDIR) == __S_IFDIR)
        {
                dirwalk(name, fsize);
        }
        printf("%s\n", name);
}
void ls_subprint(char c)
{
        char name[60];
        char path[60];
        if (getcwd(path, sizeof(path)) == NULL)
        {
                perror("getcwd");
                exit(1);
        }
        struct dirent *dp;
        DIR *dfd;
        if ((dfd = opendir(path)) == NULL)
        {
                perror("opendir");
                exit(1);
        }
        if (c == 'l')
        {
                int mode_array[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
                int sum = 0;
                struct stat stbuf;
                while ((dp = readdir(dfd)) != NULL)
                {
                        if (stat(dp->d_name, &stbuf) < 0)
                        {
                                perror("Stat");
                                exit(1);
                        }
                        if ((stbuf.st_mode & __S_IFREG) == __S_IFREG)
                        {
                                putchar('-');
                        }
                        else
                        {
                                putchar('d');
                        }
                        for (int i = 0; i < 9; i++) //文件权限
                        {
                                if ((stbuf.st_mode & mode_array[i]) == mode_array[i])
                                {
                                        if (i % 3 == 0)
                                        {
                                                putchar('r');
                                        }
                                        if (i % 3 == 1)
                                        {
                                                putchar('w');
                                        }
                                        if (i % 3 == 2)
                                        {
                                                putchar('x');
                                        }
                                }
                                else
                                {
                                        putchar('-');
                                }
                        }
                        putchar(' ');
                        printf("%d ", stbuf.st_nlink); //文件硬链接数或子目录数
                        struct passwd *getpwuid(), *pw_str;
                        if ((pw_str = getpwuid(stbuf.st_uid)) == NULL)
                        {
                                perror("getpwuid");
                                exit(1);
                        }
                        printf("%-8s ", pw_str->pw_name); //文件拥有者用户名
                        struct group *getgrgid(), *gr_str;
                        if ((gr_str = getgrgid(stbuf.st_gid)) == NULL)
                        {
                                perror("getgrgid");
                                exit(1);
                        }
                        printf("%-8s ", gr_str->gr_name); //文件拥有者所在组名
                        printf("%5d ", stbuf.st_size);    //文件大小(byte)
                        struct tm *t = localtime(&(stbuf.st_ctime));
                        printf("%2d月  %2d %02d:%02d ", 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min); //文件状态最后修改时间(ctime)
                        sum += stbuf.st_size;
                        printf("%s", dp->d_name);
                        putchar('\n');
                }
                printf("总用量 %d\n", sum / 1024); //目录总大小(KB)
        }
        else if (c == 'R')
        {
                fsize(".");
        }
        else
        {
                while ((dp = readdir(dfd)) != NULL)
                {
                        if ((c == 0 || c == 'A') && (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0))
                        {
                                continue;
                        }
                        if (c == 0 && dp->d_name[0] == '.')
                        {
                                continue;
                        }
                        printf("%s ", dp->d_name);
                }
        }
        closedir(dfd);
        printf("\n");
}
void ls_command(char *shell)
{
        int paracnt = 1;
        char *para[5];
        for (int u = 0; u < 5; u++)
        {
                para[u] = (char *)malloc(10 * sizeof(char));
        }
        para[0] = "ls";
        char *pos = NULL;
        while (((pos = strchr(shell, '-')) != NULL))
        {
                para[paracnt][0] = '-';
                int temp = 1;
                while (isalpha(*(pos + temp)) || *(pos + temp) == '-')
                {
                        para[paracnt][temp] = *(pos + temp);
                        temp++;
                }
                para[paracnt][temp] = '\0';
                shell = pos + temp;
                paracnt++;
        }
        para[paracnt] = NULL;
        if (paracnt == 1)
        { //case1:ls无参数(不显示隐藏文件)
                ls_subprint(0);
        }
        else
        {
                for (int i = 1; i < paracnt; i++)
                {
                        if (strcmp(para[i], "-a") == 0)
                        {
                                ls_subprint('a');
                                break;
                        }
                        if (strcmp(para[i], "-A") == 0)
                        {
                                ls_subprint('A');
                        }
                        if (strcmp(para[i], "-l") == 0)
                        {
                                ls_subprint('l');
                        }
                        if (strcmp(para[i], "-R") == 0)
                        {
                                ls_subprint('R');
                        }
                }
        }
        //execvp("ls", para);
        //perror("ls command");
        exit(1);
}