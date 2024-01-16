#include"loader_text.h"
#include<fstream>
std::string* loader_text::load(const std::string& filepath)
{
    if(filepath.empty())
    {
        return nullptr;
    }
    std::ifstream file;
    file.open(filepath, std::ios::in);
    if(!file.is_open())
    {
        return nullptr;
    }
    std::string line;
    std::string* text=new std::string();
    while (std::getline(file, line))
    {
        file>>line;
        *text+=line;
    }
    return text;
}