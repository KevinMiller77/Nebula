#pragma once 
#include <Nebula.h>


//Probably phasing this out
namespace Nebula{
    class TextureLibrary
    {
    public:
        ~TextureLibrary();

		void AddTexture(const std::string name, uint32_t width, uint32_t height);
		void AddTexture(const std::string name, const std::string path);
        void AddTexture(const std::string name, Ref<Texture2D> texture);

        void RemoveTexture(std::string name);

        std::string GetName(Ref<Texture2D> Texture);

        Ref<Texture2D> GetTexture(std::string name);
        std::vector<std::string> GetAllNames();

        inline int GetSize() { return Library.size(); }
    
    private:
        std::unordered_map<std::string, Ref<Texture2D>> Library;
    };
}
