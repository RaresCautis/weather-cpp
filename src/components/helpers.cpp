#include <helpers.hpp>

using namespace ftxui;

ftxui::Component makeClickableComponent(ftxui::Component comp) {

    auto clickableComponent = CatchEvent(comp, [&, comp](Event event) {
        // We only care about mouse left-release.
        if (event.is_mouse() && event.mouse().button == Mouse::Left &&
            event.mouse().motion == Mouse::Released) {
            // Let the Component process the event first
            // (which will update `selected_` if clicked on a valid item).
            bool handled = comp->OnEvent(event);
            if (handled) {
                // The click was inside the Component's bounds & changed
                // selection → now call "Enter" logic
                return comp->OnEvent(Event::Return);
            }
        }
        return false; // Not handled
    });

    return clickableComponent;
}
