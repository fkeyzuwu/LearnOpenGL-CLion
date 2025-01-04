#pragma once
#include <string>
#include <vector>

namespace fkeyz
{
    class TextureCubemap
    {
    private:
        const std::string assets_path = "../assets/";
        unsigned int id;
        void generate(std::vector<std::string> faces);
    public:
        TextureCubemap(std::vector<std::string> faces);
        void setActive(int texture_index = 0);
        void bind();
    };
}
