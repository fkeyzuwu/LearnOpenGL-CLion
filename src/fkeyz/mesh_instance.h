#pragma once
#include <array>
#include <vector>
#include <span>


namespace fkeyz
{
    class MeshInstance
    {
    private:
        unsigned int vao;
        unsigned int vbo;
    public:
        MeshInstance(std::span<float> vertices, int stride, std::initializer_list<int> attribute_sizes);
        void bind();
    };
}
