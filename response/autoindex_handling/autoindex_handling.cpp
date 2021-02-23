//
// Created by airat on 22.02.2021.
//

#include <cstdlib>
#include <algorithm>
#include "autoindex_handling.hpp"

std::string replaceQuoteToCode(const std::string& str) {
    std::string copy = str;
    std::string::iterator it = copy.begin();
    while (it != copy.end()) {
        if ((*it) == '"') {
            copy.replace(it, it+1, "%22");
        }
        ++it;
    }
    return copy;
}


bool isUtf_8(char c)
{
    return (c & 0xC0) == 0x80;
}

// https://stackoverflow.com/questions/31652407/how-to-get-the-accurate-length-of-a-stdstring
std::size_t getCharsLen(const std::string& str)
{
    return (str.length() - count_if(str.begin(), str.end(), isUtf_8));
}

void write_html()
{
    int autoindex = open("test_autoindex.html", O_RDWR | O_CREAT | O_TRUNC, 0666);

    if (autoindex <= 0) {
        std::cerr << std::strerror(errno) << std::endl;
        throw std::exception();
    }

    std::string root = "";
    std::string uri = "/";

    std::string host_n_port = "http://localhost";
    std::string url = host_n_port + uri;

    std::string response_body = "<html>";

    response_body += "<head>\n"
                     "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
                     "<title>Index of ";
    response_body += uri;
    response_body +="</title>\n"
                     "</head>\n";

    response_body += "<body>\n";
    response_body += "<h1>Index of ";
    response_body += uri;
    response_body += "</h1><hr>\n";
    response_body += "<pre>\n";


    response_body += "<a href=\"";
    response_body += "http://localhost/"; // current page root
    response_body += "\">../</a>\n";



    std::list<std::map<std::string, std::list<std::string> > > info = dir_opers(root, uri);
    std::list<std::map<std::string, std::list<std::string> > >::iterator it = info.begin();

    while (it != info.end()) {
        std::map<std::string, std::list<std::string> >::iterator it_map = (*it).begin();
        while (it_map != (*it).end()) {
            response_body += "<a href=\"";

            std::string correct = replaceQuoteToCode((*it_map).first);
            response_body += (url + correct);
            response_body += "\">";

            std::string file_name = (*it_map).first;
            std::size_t fname_len = getCharsLen(file_name);
            if (fname_len > 50) {
                std::size_t i = 47;
                std::string file_name_tmp = file_name.substr(0, i);
                while (getCharsLen(file_name_tmp) != 47)
                {
                    i++;
                    file_name_tmp = file_name.substr(0, i);
                }
                if (i > 47)
                    i++;
                file_name = file_name.substr(0,i) + "..>";
            }
            fname_len = getCharsLen(file_name);
            response_body += file_name;
            response_body += "</a>";

            std::size_t count_of_spaces = (50 - fname_len) + 1;
            response_body += std::string(count_of_spaces, ' ');

            response_body += (*it_map).second.front(); // file modified datetime

            count_of_spaces = 20 - (*it_map).second.back().size();
            response_body += std::string(count_of_spaces, ' ');

            response_body += (*it_map).second.back(); // file size in bytes
            response_body += "\n";
            ++it_map;
        }
        ++it;
    }

    response_body += "</pre><hr>\n";
    response_body += "</body>\n";
    response_body += "</html>\n";

    write(autoindex, response_body.c_str(), response_body.size());
    close(autoindex);
}

void ft_exit()
{
    std::cout << strerror(errno)  << std::endl;
    exit(errno);
}


std::list<std::map<std::string, std::list<std::string> > > dir_opers(const std::string& root,
                                                                     const std::string& uri)
{
    std::string dir_name = root + uri;

    DIR* dir_stream;
    dir_stream = opendir(dir_name.c_str());

    if (!dir_stream)
        ft_exit();
    struct dirent *info;

    std::map<std::string, std::list<std::string> > files;
    std::map<std::string, std::list<std::string> > dirs;

    std::map<std::string, std::list<std::string> >* tmp;
    while ((info = readdir(dir_stream))) {
        std::string file_obj_name = std::string(info->d_name);

        if (file_obj_name[0] == '.')
            continue;
        struct stat info_buf;
        std::string file_full_path = dir_name + file_obj_name;

        if (stat(file_full_path.c_str(), &info_buf) == -1) {
            std::cout << strerror(errno) << std::endl;
            exit(errno);
        }

        int file_type = info_buf.st_mode & S_IFMT;

        if (file_type == S_IFREG)
            tmp = &files;
        else if (file_type == S_IFDIR)
            tmp = &dirs;

        std::list<std::string> date_modified_and_size_in_bytes;


        // get modify time info
        long corrected_to_GMT = info_buf.st_mtime + WebServ::getCorrectionMinutesToGMT()*60;
        std::string modified_seconds_to_str = libft::ultostr_base(corrected_to_GMT, 10);
        struct tm modified_time;
        char *null_if_error = strptime(modified_seconds_to_str.c_str(), "%s", &modified_time);
        if (!null_if_error)
            throw std::exception();
        char time_to_str[18]; // example: 31-Jan-2021 20:51
        strftime(time_to_str, 18, "%d-%b-%Y %H:%M", &modified_time);

        date_modified_and_size_in_bytes.push_back(std::string(time_to_str));

        // get size of file in bytes
        if (file_type == S_IFDIR)
            date_modified_and_size_in_bytes.push_back("-");
        else
            date_modified_and_size_in_bytes.push_back(libft::ultostr_base(info_buf.st_size, 10));

        if (file_type == S_IFDIR)
            file_obj_name += "/";
        (*tmp)[file_obj_name] = date_modified_and_size_in_bytes;

    }
    closedir(dir_stream);

    std::list<std::map<std::string, std::list<std::string> > > all_info;
    all_info.push_back(dirs);
    all_info.push_back(files);

    return all_info;
}
