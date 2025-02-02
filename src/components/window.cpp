#include <window.hpp>

using namespace ftxui;

WindowComponent::WindowComponent(std ::string newTitle)
    : windowTitle(std::move(newTitle)) {};

Element WindowComponent::getChild() {
    if (child)
        return child->Render();
    return filler();
};

Component WindowComponent::getComponent() {
    std::string wrappedTitle = "|" + windowTitle + "|";

    // TODO: Move the inbetween fillers to achieve top/bottom alignment
    // positioning
    return Renderer(child, [this, wrappedTitle] {
        return window(text(wrappedTitle), getChild()) | flex_grow;
    });
}

void WindowComponent::setChild(Component comp) { child = comp; }
