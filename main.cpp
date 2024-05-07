#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <filesystem>

enum Operations
{
    NEXT_CELL = '>',
    PREV_CELL = '<',
    INCREMENT_CELL = '+',
    DECREMENT_CELL = '-',
    DISPLAY_VALUE = '.',
    WRITE_VALUE = ',',
    JUMP_FORWARD = '[',
    JUMP_BACKWARD = ']'
};

int find_matching_closing_bracket(int start, std::string const &code)
{
    int offset = start;
    int open_bracket_count = 1;
    while (offset < code.size() && open_bracket_count > 0)
    {
        if (code[offset] == JUMP_FORWARD)
        {
            open_bracket_count++;
        }
        else if (code[offset] == JUMP_BACKWARD)
        {
            open_bracket_count--;
        }
        if (open_bracket_count <= 0)
        {
            return offset;
        }
        offset++;
    }
    return -1;
}

int find_matching_opening_bracket(int start, std::string const &code)
{
    int offset = start;
    int open_bracket_count = 1;
    while (offset >= 0 && open_bracket_count > 0)
    {
        if (code[offset] == JUMP_FORWARD)
        {
            open_bracket_count--;
        }
        else if (code[offset] == JUMP_BACKWARD)
        {
            open_bracket_count++;
        }
        if (open_bracket_count <= 0)
        {
            return offset;
        }
        offset--;
    }
    return -1;
}

/**
 * @brief
 *
 * @return char* code of the program to execute
 */
bool prepare_program(int argc, char **argv, std::string &code)
{
    std::string path;
    bool has_path = false;
    if (argc <= 1)
    {
        return false;
    }
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0)
        {
            path = argv[i + 1];
            has_path = true;
            if (!std::filesystem::exists(path))
            {
                std::cerr << "File not found" << std::endl;
                return false;
            }
            break;
        }
        else if (strcmp(argv[i], "-c") == 0)
        {
            code = argv[i + 1];
            return true;
        }
    }
    if (!has_path)
    {
        return NULL;
    }

    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    code = buffer.str();
    return true;
}
int main(int argc, char **argv)
{
    std::string code;
    if (!prepare_program(argc, argv, code))
    {
        std::cout << "No input data provided" << std::endl;
        std::cout << "Simple brainfuck interpreter: -f <filename> or -c <code> to use" << std::endl;
        return EXIT_FAILURE;
    }

    char memory[30000];
    for (int i = 0; i < 30000; i++)
    {
        memory[i] = 0;
    }

    int program_counter = 0;
    int ptr = 0;

    while (program_counter < code.size())
    {
        switch (code[program_counter])
        {
        case NEXT_CELL:
            ptr++;
            break;
        case PREV_CELL:
            ptr--;
            break;
        case INCREMENT_CELL:
            memory[ptr]++;
            break;
        case DECREMENT_CELL:
            memory[ptr]--;
            break;
        case DISPLAY_VALUE:
            std::cout << memory[ptr];
            break;
        case WRITE_VALUE:
            std::cin >> memory[ptr];
            break;
        case JUMP_FORWARD:
            if (memory[ptr] == 0)
            {
                // offset by 1 so that the unconditional increment would not interfere
                program_counter = find_matching_closing_bracket(program_counter + 1, code) - 1;
            }
            break;
        case JUMP_BACKWARD:
            if (memory[ptr] != 0)
            {
                program_counter = find_matching_opening_bracket(program_counter - 1, code) - 1;
            }
            break;
        default:
            break;
        }
        program_counter++;
    }
    std::cout << std::endl
              << "end of execution" << std::endl;
    return 0;
}
