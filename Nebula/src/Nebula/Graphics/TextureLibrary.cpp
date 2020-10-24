#include "TextureLibrary.h"

namespace Nebula
{
    TextureLibrary::~TextureLibrary()
    {
        for (auto kvp : Library)
        {
            delete kvp.second;
        }
    }

    void TextureLibrary::AddTexture(const std::string name, uint32_t width, uint32_t height)
    {
        Texture2D* newTex = Texture2D::Create(width, height);
        if (newTex != nullptr && newTex->IsValid())
        {
            Library[name] = newTex;
            return;
        }
        
        LOG_ERR("Lib: Could not make texture <%s> with given paramters!\n", name.c_str());
    }

    void TextureLibrary::AddTexture(const std::string name, const std::string path)
    {
        Texture2D* newTex = Texture2D::Create(path);
        if (newTex != nullptr && newTex->IsValid())
        {
            Library[name] = newTex;
            return;
        }

        LOG_ERR("Lib: Could not make texture <%s> with given paramters!\n", name.c_str());
    }
    
    void TextureLibrary::AddTexture(const std::string name, Texture2D* texture)
    {
        Library[name] = texture;
    }

    
    std::string TextureLibrary::GetName(Texture2D* Texture)
    {
        for (auto kv : Library)
        {
            if (Texture == kv.second)
            {
                return kv.first;
            }
        }
        return std::string();
    }

    void TextureLibrary::RemoveTexture(std::string name)
    {
        if (Library.find(name) == Library.end())
        {
            LOG_ERR("Texture did not exist! could not remove\n");
            return;
        }
        Library.erase(name);
    }

    Texture2D* TextureLibrary::GetTexture(std::string name)
    {
        
        if (Library.find(name) == Library.end())
        {
            LOG_ERR("Texture did not exist! could not remove\n");
            return nullptr;
        }
        return Library[name];
    }

    std::vector<std::string> TextureLibrary::GetAllNames()
    {
        std::vector<std::string> out;
        out.reserve(Library.size());

        for (auto kv : Library)
        {
            out.push_back(kv.first);
        }

        return out;
    }

}