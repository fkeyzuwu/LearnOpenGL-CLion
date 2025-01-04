#pragma once
#include <string>
#include <glad/glad.h>

namespace fkeyz
{
    class Texture2D
    {
    private:
        const std::string assets_path = "../assets/";
        unsigned int id;
        void generate(char const *path);
    public:
        Texture2D(char const *path);
        void setActive(int texture_index = 0);
        void bind();
    };
};
