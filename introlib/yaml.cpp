#include <cassert>
#include "yaml.h"

Yaml::Yaml(const std::string& filename)
: fname(filename)
, ifs(filename)
{
}

Yaml::~Yaml()
{
    ifs.close();
}

void Yaml::parse(const std::string& line)
{
    // ignore all comments
    if (line.find("# ") == 0)
        return;
    
    // looking for a sequence indicator
    std::size_t pos = line.find("- ");
    if (pos != std::string::npos)
    {
        if (pos == 0)
        {
            // emplace an empty  BRIDGE entry
            data.emplace_back();
            
            std::cerr << "[" << pos << "] " << "- " << std::endl;
        }
        else
        {
            assert(data.empty() == false);

            // emplace a new hiking-pace for this BRIDGE
            std::string value = line.substr(pos += 2);
            data.back().paces.emplace_back(std::stod(value));

            std::cerr << "[" << pos << "] " << "  - " << value << std::endl;
        }
    }

    // looking for a key-value indicator
    pos = line.find(": ");
    if (pos != std::string::npos)
    {
        const std::string key1 = "length";
        const std::string key2 = "paces";

        if (line.find(key1, 2) != std::string::npos)
        {
            assert(data.empty() == false);

            // sent the length of the bridge
            std::string value = line.substr(pos += 2);
            data.back().length = std::stoi(value);

            std::cerr << "[" << pos << "]   " << key1 << ": " << value << std::endl;
        }
        else if (line.find(key2, 2) != std::string::npos)
        {
            std::cerr << "[" << pos << "]   " << key2 << ": " << std::endl;
        }
    }
}

void Yaml::parse()
{
    assert(ifs.is_open() == true);

    std::string line;
    getline(ifs, line);
    assert(line == "---");

    while (getline(ifs, line))
        parse(line);
}

const std::vector<BRIDGE>& Yaml::get() const
{
    return data;
}

