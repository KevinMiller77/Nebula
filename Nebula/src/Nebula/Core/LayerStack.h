#pragma once
#include <Core/Layer.h>

namespace Nebula
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return layers.begin(); }
        std::vector<Layer*>::iterator end() { return layers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }

        std::vector<Layer*>::const_iterator begin() const { return layers.begin(); }
        std::vector<Layer*>::const_iterator end()	const { return layers.end(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const { return layers.rend(); }

        void erase(int index) { layers.erase(layers.begin() + index); }

        Layer* operator[](int index) {return layers[index];}

        unsigned int GetSize() { return layers.size(); }
    private:
        std::vector<Layer*> layers;
        unsigned int layerInsertIndex = 0;
    };
}