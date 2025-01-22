#ifndef WINDOW_H
#define WINDOW_H
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

enum class ComponentPosition {
    topLeft,
    topRight,
    center,
    bottomLeft,
    bottomRight,
};

class WindowComponent {
  public:
    WindowComponent(std::string newTitle);
    ftxui::Component getWindow();
    void setComponent(ComponentPosition position, ftxui::Component comp);
    void deleteComponent(ComponentPosition position);

  private:
    ftxui::Element getComponent(ComponentPosition position);
    ftxui::Component getCenterComponent(ComponentPosition position);
    std::unordered_map<ComponentPosition, ftxui::Component> components;
    std::string windowTitle;
};

#endif
