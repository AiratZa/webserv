//
// Created by airat on 22.02.2021.
//

#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>

// readdir
#include <sys/types.h>
#include <dirent.h>

// stat
#include <sys/stat.h>

std::string head = "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\"><title>Index of /</title><style></style></head>";
std::string body_h1 = "<h1>Index of /</h1><hr>";
std::string list_of_links = "<a href=\"http://localhost/zsh/\">zsh/</a>                                               03-Dec-2020 13:25                   -\n"
                            "<a href=\"http://localhost/adjtime\">adjtime</a>                                            05-Jan-2021 08:13                  46\n";


void write_html()
{
    //    int autoindex = open("test_autoindex.html", O_RDWR | O_CREAT | O_TRUNC, 0666);
//
//    if (autoindex <= 0) {
//        std::cerr << std::strerror(errno) << std::endl;
//        throw std::exception();
//    }
//    write(autoindex, "<html>", 6);
//    write(autoindex, head.c_str(), head.length());
//
//    write(autoindex, "<body>", 6);
//    write(autoindex, body_h1.c_str(), body_h1.length());
//    write(autoindex, "<pre>", 5);
//    write(autoindex, "<a href=\"http://localhost/\">../</a>\n", 5); // go back
//    write(autoindex, list_of_links.c_str(), list_of_links.length());
//    write(autoindex, "</pre><hr>", 10);
//    write(autoindex, "</body>",7);
//
//    write(autoindex, "</html>", 7);
//    close(autoindex);
}

void ft_exit()
{
    std::cout << strerror(errno)  << std::endl;
    exit(errno);
}

int ft_strlen(char* str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}


static int count_num(unsigned long n, int base)
{
    int i;

    i = 1;
    while ((n = n / base) != 0)
        i++;
    return (i);
}

/*
 * only 10 and 16 bases
 * 16 bases in lowercase
 */
std::string ultostr_base(unsigned long n, int base)
{
    std::string s;
    int num;

    num = count_num(n, base);
    s.resize(num);
    while (num--) {
        s[num] = n % base + '0';
        if (s[num] > '9')
            s[num] += 39; // distance between '9' and 'a'
        n /= base;
    }
    return (s);
}

void dir_opers()
{

    int dir_file = open("test_dir.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);

    if (dir_file <= 0) {
        std::cerr << std::strerror(errno) << std::endl;
        throw std::exception();
    }

    std::string dir_name = "/etc/nginx/";

    DIR* dir_stream;
    dir_stream = opendir(dir_name.c_str());

    if (!dir_stream)
        ft_exit();
    struct dirent *info;

    while ((info = readdir(dir_stream))) {
        char * file_obj_name = info->d_name;

        write(dir_file, file_obj_name, ft_strlen(file_obj_name));

        struct stat info_buf;
        std::string file_full_path = dir_name + std::string(file_obj_name);
        if (stat(file_full_path.c_str(), &info_buf) == -1) {
            std::cout << strerror(errno) << std::endl;
            break;
        }

//        blksize_t     st_blksize;  /* размер блока ввода-вывода */
//        /* в файловой системе */

//        time_t        st_mtime;    /* время последней модификации */
        std::cout << file_obj_name << "  " << info_buf.st_size  << "   " << info_buf.st_mtime  << "    "<< << std::endl;
        std::string file_type =  "\t\t\t|\tFile type: ";

        switch (info_buf.st_mode & S_IFMT) {
            case S_IFBLK:  file_type += "block device\n";            break;
            case S_IFCHR:  file_type += "character device\n";        break;
            case S_IFDIR:  file_type += "directory\n";               break;
            case S_IFIFO:  file_type += "FIFO/pipe\n";               break;
            case S_IFLNK:  file_type += "symlink\n";                 break;
            case S_IFREG:  file_type += "regular file\n";            break;
            case S_IFSOCK: file_type += "socket\n";                  break;
            default:       file_type += "unknown?\n";                break;
        }
        write(dir_file, file_type.c_str(), file_type.size());

        std::string modified_seconds_to_str = ultostr_base(info_buf.st_mtime, 10);
        struct tm modified_time;
        char *null_if_error = strptime(modified_seconds_to_str.c_str(), "%s", &modified_time);
        if (!null_if_error)
            throw std::exception();

        char *time_to_str
        size_t strftime(char *s, size_t max, const char *format,
                        const struct tm *tm);

        31-Jan-2021 20:51
        write(dir_file, "\n", 1);
        std::cout << std::endl;
    }
    closedir(dir_stream);
    close(dir_file);
}

int main(void)
{
    write_html();
    dir_opers();
}