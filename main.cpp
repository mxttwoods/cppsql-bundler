#include <iostream>
#include <filesystem> // C++17 feature for directory handling
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem; // alias for convenience

struct SqlFile
{
    int order;
    std::string filename;

    // Constructor
    SqlFile(int ord, std::string fname) : order(ord), filename(fname) {}
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <input_directory> <output_file>\n";
        return 1;
    }

    std::string input_dir = argv[1];
    std::string output_file = argv[2];
    std::vector<SqlFile> sql_files;

    // Read directory and collect SQL files
    try
    {
        for (const auto &entry : fs::directory_iterator(input_dir))
        {
            if (entry.path().extension() == ".sql")
            {
                std::string filename = entry.path().filename().string();

                // Parse the order number from filename
                size_t underscore_pos = filename.find('_');
                if (underscore_pos != std::string::npos)
                {
                    try
                    {
                        int order = std::stoi(filename.substr(0, underscore_pos));
                        sql_files.emplace_back(order, entry.path().string());
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Error parsing order from filename: " << filename << "\n";
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Directory error: " << e.what() << "\n";
        return 1;
    }

    // Sort files by order
    std::sort(sql_files.begin(), sql_files.end(),
              [](const SqlFile &a, const SqlFile &b)
              {
                  return a.order < b.order;
              });

    // Write combined output
    std::ofstream output(output_file);
    if (!output.is_open())
    {
        std::cerr << "Could not open output file\n";
        return 1;
    }

    for (const auto &sql_file : sql_files)
    {
        std::ifstream input(sql_file.filename);
        if (input.is_open())
        {
            output << "-- File: " << sql_file.filename << "\n\n";
            output << input.rdbuf();
            output << "\n\n";
            input.close();
        }
        else
        {
            std::cerr << "Could not open file: " << sql_file.filename << "\n";
        }
    }

    output.close();
    return 0;
}