//
// Created by Philip on 4/2/2023.
//

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <optional>
namespace Doge {

    /**
     * A file to be read
     */
    class InFile {
    private:
        std::filesystem::path location;
    public:

        /**
         * Search for file recursive in directory
         * @param directory Directory to search
         * @param filename Name of file to search for with extension
         * @return Found file if found
         */
        static std::optional<InFile> searchForFile(const  std::filesystem::path& directory, const std::string& filename){
            if(!is_directory(directory) && directory.filename() == filename){
                    return {InFile(directory.string())};
            }
            for (const std::filesystem::path & child : std::filesystem::directory_iterator(directory)){
                std::optional<InFile> search = searchForFile(child, filename);
                if(search) return search;
            }
            return {};
        }

        /**
         * Create a new input file
         * @param filepath Location of input file
         * Does not read file yet
         * @throws runtime_error File could not be found
         */
        InFile(const std::filesystem::path& filepath) : location(filepath) {
            if(!exists(location)){
                throw std::runtime_error("Could not find " + filepath.string());
            }
        }

        /**
         * Get the location of the file
         * @return The filepath
         */
        std::string getLocation() const {
            return location.string();
        }

        /**
         * Get the file extension of the file
         * @return Extension including the '.'
         */
        std::string getExtension() const {
            return location.extension().string();
        }

        /**
        * Read a file as a bunch of bytes
        * @return A bunch of bytes
        * @throws runtime_error Could not open file
        */
        std::vector<char> readFile() const {
            //Open file at end
            std::ifstream file(location, std::ios::ate | std::ios::binary);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file");
            }
            //Get size using end of file
            size_t file_size = (size_t) file.tellg();
            std::vector<char> buffer(file_size);
            //read
            file.seekg(0);
            file.read(buffer.data(), file_size);
            file.close();
            return buffer;
        }

        //todo defer loading of files until resources are needed
        //todo auto pick loader based on file extension


    };

} // Doge
