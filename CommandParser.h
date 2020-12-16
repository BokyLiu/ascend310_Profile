#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <unordered_map>

inline int parseStrToInt(const std::string &str)
{
    try {
        return std::stoi(str);
    } catch (std::invalid_argument const &e) {
        printf("Bad input: std::invalid_argument thrown\n");
    } catch (std::out_of_range const &e) {
        printf("Integer overflow: std::out_of_range thrown\n");
    }
    return 0;
}

class CommandParser {
public:
    CommandParser(){}
    ~CommandParser(){}
    CommandParser(int &argc, char **argv)
    {
        parseArgs(argc, argv);
    }

    CommandParser &addOption(const std::string &option, const std::string &defaults = "")
    {
        this->opmap.emplace(option, defaults);
        return *this;
    }

    void parseArgs(int &argc, char **argv)
    {
        for (int i = 1; i < argc; ++i) {
            this->tokens.push_back(std::string(argv[i]));
        }
    }

    const std::string &cmdGetOption(const std::string &option) const
    {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        if (opmap.count(option)) {
            return opmap.at(option);
        }
        static const std::string empty_string("");
        return empty_string;
    }

    bool cmdOptionExists(const std::string &option) const
    {
        return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
    }

private:
    std::vector<std::string> tokens;
    std::unordered_map<std::string, std::string> opmap;
};

#endif /* COMMANDPARSER_H */
