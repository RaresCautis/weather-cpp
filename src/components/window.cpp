#include "window.hpp"

using namespace ftxui;

WindowComponent::WindowComponent(std ::string newTitle)
    : windowTitle(std::move(newTitle)) {};

Element WindowComponent::getComponent(ComponentPosition position) {
    if (auto it = components.find(position); it != components.end())
        return components[position]->Render() | flex;
    return filler();
};

Component WindowComponent::getCenterComponent(ComponentPosition position) {
    if (auto it = components.find(position); it != components.end())
        return components[position];
    return Renderer([] { return filler(); });
}

Component WindowComponent::getWindow() {
    std::string wrappedTitle = "|" + windowTitle + "|";

    // auto centerComp = components[ComponentPosition::center];
    // centerComp = Renderer([] { return filler(); });

    return Renderer(
        getCenterComponent(ComponentPosition::center), [this, wrappedTitle] {
            return window(
                       text(wrappedTitle),
                       gridbox({
                           {
                               getComponent(ComponentPosition::topLeft),
                               filler(),
                               getComponent(ComponentPosition::topRight),
                           },
                           {
                               filler(),
                               getComponent(ComponentPosition::center) | center,
                               filler(),
                           },
                           {
                               getComponent(ComponentPosition::bottomLeft),
                               filler(),
                               getComponent(ComponentPosition::bottomRight),
                           },
                       }) | flex_grow) |
                   flex_grow;
        });
}

void WindowComponent::setComponent(ComponentPosition position, Component comp) {
    components[position] = comp;
}

void WindowComponent::deleteComponent(ComponentPosition position) {
    components.erase(position);
}
