#ifndef WINDOW_H
#define WINDOW_H
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

enum class ComponentPosition {
    left,
    top,
    center,
    bottom,
    right,
};

class WindowComponent {
  public:
    WindowComponent(std::string newTitle);
    ftxui::Component getComponent();
    void setChild(ComponentPosition position, ftxui::Component comp);
    void deleteChild(ComponentPosition position);

  private:
    ftxui::Element getChild(ComponentPosition position);
    ftxui::Component getCenterChild();
    std::unordered_map<ComponentPosition, ftxui::Component> children;
    std::string windowTitle;
};

#endif
