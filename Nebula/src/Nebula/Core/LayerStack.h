#pragma once
#include <Core/Layer.h>
#include <Core/Ref.h>

namespace Nebula
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Ref<Layer> layer);
        void PushOverlay(Ref<Layer> overlay);
        void PopLayer(Ref<Layer> layer);
        void PopOverlay(Ref<Layer> overlay);

        std::vector<Ref<Layer>>::iterator begin() { return layers.begin(); }
        std::vector<Ref<Layer>>::iterator end() { return layers.end(); }
        std::vector<Ref<Layer>>::reverse_iterator rbegin() { return layers.rbegin(); }
        std::vector<Ref<Layer>>::reverse_iterator rend() { return layers.rend(); }

        std::vector<Ref<Layer>>::const_iterator begin() const { return layers.begin(); }
        std::vector<Ref<Layer>>::const_iterator end()	const { return layers.end(); }
        std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
        std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return layers.rend(); }

        void erase(int index) { layers.erase(layers.begin() + index); }

        Ref<Layer> operator[](int index) {return layers[index];}

        unsigned int GetSize() { return layers.size(); }
    private:
        std::vector<Ref<Layer>> layers;
        unsigned int layerInsertIndex = 0;
    };
}