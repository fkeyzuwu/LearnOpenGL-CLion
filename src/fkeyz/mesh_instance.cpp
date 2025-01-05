#include "mesh_instance.h"

#include <initializer_list>
#include <vector>
#include <glad/glad.h>

namespace fkeyz
{
    MeshInstance::MeshInstance(std::span<float> vertices, int stride, std::initializer_list<int> attribute_sizes)
    {
        float stride_size = sizeof(float) * stride;
        size_t offset = 0;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

        size_t i = 0;

        for (int attribute_size : attribute_sizes)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attribute_size, GL_FLOAT, GL_FALSE, stride_size, (void*)(offset * sizeof(float)));
            offset += attribute_size;
            i++;
        }

        glBindVertexArray(0);
    }

    void MeshInstance::bind()
    {
        glBindVertexArray(vao);
    }
}
