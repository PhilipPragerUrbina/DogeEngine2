//
// Created by Philip on 4/17/2023.
//

#pragma once

#include "InFile.hpp"

namespace Doge {

    /**
     * Represents a directory on the filesystem
     */
    class Directory {
    private:
        std::filesystem::path path;

    public:
        /**
         * Specify a directory
         * @param path Where the directory is
         * @throws runtime_error Not valid directory
         */
        Directory(const std::filesystem::path& path) : path(path){
            if(!exists(path) || !is_directory(path)){
                throw std::runtime_error("Invalid directory: " + path.string());
            }
        }

        /**
         * Find a file anywhere in directory
         * @param ID Name of file without extension(stem)
         * @return The found file if found
         */
        std::optional<InFile> findFile(const std::string& ID){
            for (const std::filesystem::path & child : std::filesystem::directory_iterator(path)){
                if(!is_directory(child)){
                    if(child.stem() == ID)  return {InFile(child.string())};
                }else{
                    std::optional<InFile> search = Directory(child).findFile(ID);
                    if(search) return search;
                }
            }
            return {};
        };

        /**
         * Find a file anywhere in directory
         * @param ID Name of file without extension
         * @param extensions Allowed extensions in order of preferred to not preferred.
         * @return The best found file if found
         */
        std::optional<InFile> findFile(const std::string& ID, const std::vector<std::string>& extensions){
            for (const std::string& extension : extensions) {
                std::optional<InFile> search = searchForFile(ID + extension);
                if(search) return search;
            }
            return {};
        }


        /**
         * Search for a specific file recursively in directory
         * @param filename Name of file to search for with extension
         * @return Found file if found
         */
        std::optional<InFile> searchForFile(const std::string& filename){
            for (const std::filesystem::path & child : std::filesystem::directory_iterator(path)){
                if(!is_directory(child)){
                    if(child.filename() == filename)  return {InFile(child.string())};
                }else{
                    std::optional<InFile> search = Directory(child).searchForFile(filename);
                    if(search) return search;
                }
            }
            return {};
        }

    };

} // Doge
