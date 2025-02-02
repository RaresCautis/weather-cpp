#ifndef WINDOW_H
#define WINDOW_H
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

class WindowComponent {
  public:
    WindowComponent(std::string newTitle);
    ftxui::Component getComponent();
    void setChild(ftxui::Component comp);

  private:
    ftxui::Element getChild();
    ftxui::Component child;
    std::string windowTitle;
};

#endif
