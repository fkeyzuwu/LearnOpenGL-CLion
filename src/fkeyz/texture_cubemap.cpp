#include "texture_cubemap.h"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

namespace fkeyz
{
    TextureCubemap::TextureCubemap(std::vector<std::string> faces)
    {
        generate(faces);
    }

    void TextureCubemap::generate(std::vector<std::string> faces)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            std::string full_path = assets_path + faces[i];
            unsigned char *data = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << full_path << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void TextureCubemap::setActive(int texture_index)
    {
        glActiveTexture(GL_TEXTURE0 + texture_index);
    }

    void TextureCubemap::bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }
}
