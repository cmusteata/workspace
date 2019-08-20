/*******************************************************************************
 *                         Author: Constantin Musteata                         *
 *                          costin.musteata@gmail.com                          *
 ******************************************************************************/

#ifndef __YAML_H__
#define __YAML_H__

#include <iostream>
#include <fstream>
#include <vector>

// struct BRIDGE
//
// This data-structure is used
// internally by the Yaml parser

struct BRIDGE final
{
    int length = 0;
    std::vector<double> paces;
};

// class Yaml
//
// This class is not intended to parse any generic .yaml file
// The sole purpose of this class is to parse the intro.yaml
//
// The following assumptions are made in order to parse:
// 1) The intro.yaml file is a valid yaml formatted file
// 2) All comments start at index 0
// 3) The sequence indicator is "- "
// 4) The keyvalue indicator is ": "
// 5) All sequences in a block are aligned relative to the same column
// 6) The underlying data structure of 'Yaml' is a sequence of BRIDGES
//    BRIDGE entries in the intro.yaml file start at index 0
// 7) Each BRIDGE has two key-value pairs starting at index 2
//    length: scalar-value
//    paces: list of scalar-values

class Yaml final
{
    std::string fname;
    std::ifstream ifs;
    std::vector<BRIDGE> data;

    void parse(const std::string& line);

public:
    Yaml(const std::string& filename);
    ~Yaml();

    void parse();
    const std::vector<BRIDGE>& get() const;
};

#endif //__YAML_H__

