#include <window.hpp>

using namespace ftxui;

WindowComponent::WindowComponent(std ::string newTitle)
    : windowTitle(std::move(newTitle)) {};

Element WindowComponent::getChild(ComponentPosition position) {
    if (auto it = children.find(position); it != children.end())
        return children[position]->Render() | flex;
    return filler();
};

Component WindowComponent::getCenterChild() {
    if (auto it = children.find(ComponentPosition::center);
        it != children.end())
        return children[ComponentPosition::center];
    return Renderer([] { return filler(); });
}

Component WindowComponent::getComponent() {
    std::string wrappedTitle = "|" + windowTitle + "|";

    // TODO: Move the inbetween fillers to achieve top/bottom alignment
    // + think about positioning
    return Renderer(getCenterChild(), [this, wrappedTitle] {
        return window(text(wrappedTitle),
                      vbox({
                          filler(), // THIS
                          hbox({
                              filler(),
                              getChild(ComponentPosition::top),
                              filler(),
                          }) | center,
                          hbox({

                              getChild(ComponentPosition::left),
                              getChild(ComponentPosition::center),
                              getChild(ComponentPosition::right),
                          }) | center,
                          filler(), // THIS
                          hbox({
                              filler(),
                              getChild(ComponentPosition::bottom),
                              filler(),
                          }) | center,
                      }) | flex_grow) |
               flex_grow;
    });
}

void WindowComponent::setChild(ComponentPosition position, Component comp) {
    children[position] = comp;
}

void WindowComponent::deleteChild(ComponentPosition position) {
    children.erase(position);
}
