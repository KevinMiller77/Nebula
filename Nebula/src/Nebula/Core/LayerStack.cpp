#include "LayerStack.h"

namespace Nebula {
    LayerStack::~LayerStack()
    {
    #ifdef NEB_PLATFORM_MACOS
    #else
        for (Layer* layer : layers)
        {
            layer->OnDetach();
            delete layer;
        }
    #endif
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        layers.emplace(layers.begin() + layerInsertIndex, layer);
        layerInsertIndex++;
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);
        if (it != layers.begin() + layerInsertIndex)
        {
            layer->OnDetach();
            layers.erase(it);
            layerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(layers.begin() + layerInsertIndex, layers.end(), overlay);
        if (it != layers.end())
        {
            overlay->OnDetach();
            layers.erase(it);
        }
    }
}