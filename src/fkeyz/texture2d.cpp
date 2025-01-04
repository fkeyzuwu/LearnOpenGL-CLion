#include "texture2d.h"

#include <iostream>
#include <stb_image/stb_image.h>

namespace fkeyz
{
    Texture2D::Texture2D(char const* path)
    {
        generate(path);
    }

    // file_name starts after "../assets/"
    void Texture2D::generate(char const *file_name)
    {
        std::string full_path = assets_path + file_name;

        glGenTextures(1, &id);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(full_path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << full_path << std::endl;
            stbi_image_free(data);
        }
    }

    void Texture2D::setActive(int texture_index)
    {
        glActiveTexture(GL_TEXTURE0 + texture_index);
    }

    void Texture2D::bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }
} // fkeyz