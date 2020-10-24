#pragma once 
#include <Nebula.h>

namespace Nebula
{
    class TextureLibrary
    {
    public:
        ~TextureLibrary();

		void AddTexture(const std::string name, uint32_t width, uint32_t height);
		void AddTexture(const std::string name, const std::string path);
        void AddTexture(const std::string name, Texture2D* texture);

        void RemoveTexture(std::string name);

        std::string GetName(Texture2D* Texture);

        Texture2D* GetTexture(std::string name);
        std::vector<std::string> GetAllNames();

        inline int GetSize() { return Library.size(); }
    
    private:
        std::unordered_map<std::string, Texture2D*> Library;
    };
}
