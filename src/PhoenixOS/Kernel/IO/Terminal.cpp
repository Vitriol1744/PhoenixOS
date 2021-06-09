#include "Terminal.hpp"

#include "Utility/Logger.hpp"

print_func Terminal::print = 0;

bool Terminal::Initialize(stivale2_struct* bootloader_data)
{
    stivale2_struct_tag_terminal* term_str_tag;
    term_str_tag = reinterpret_cast<stivale2_struct_tag_terminal*>(stivale2_get_tag(bootloader_data, STIVALE2_STRUCT_TAG_TERMINAL_ID));
    
    if (!term_str_tag) while (true)
    {
        asm ("hlt");
    }

    void* term_write_ptr = reinterpret_cast<void*>(term_str_tag->term_write);
    reinterpret_cast<void*&>(print) = term_write_ptr;

    if (!print)
    {
        LOG_ERROR("Failed to Initialize Terminal!");
        return false;
    }

    LOG_TRACE("Initialized Terminal!");
    return true;
}

void Terminal::Print(const char* msg, size_t length)
{
    print(msg, length);
}

void Terminal::SetColor(Terminal::Color foreground_color, Terminal::Color background_color)
{
    switch (foreground_color)
    {
        case Color::BLACK:
            Print(ANSI::foreground_black);
            break;
        case Color::RED:
            Print(ANSI::foreground_red);
            break;
        case Color::GREEN:
            Print(ANSI::foreground_green);
            break;
        case Color::YELLOW:
            Print(ANSI::foreground_yellow);
            break;
        case Color::BLUE:
            Print(ANSI::foreground_blue);
            break;
        case Color::MAGENTA:
            Print(ANSI::foreground_magenta);
            break;
        case Color::CYAN:
            Print(ANSI::foreground_cyan);
            break;
        case Color::WHITE:
            Print(ANSI::foreground_white);
            break;
        case Color::NONE:
            break;
    }
    switch (background_color)
    {
        case Color::BLACK:
            Print(ANSI::background_black);
            break;
        case Color::RED:
            Print(ANSI::background_red);
            break;
        case Color::GREEN:
            Print(ANSI::background_green);
            break;
        case Color::YELLOW:
            Print(ANSI::background_yellow);
            break;
        case Color::BLUE:
            Print(ANSI::background_blue);
            break;
        case Color::MAGENTA:
            Print(ANSI::background_magenta);
            break;
        case Color::CYAN:
            Print(ANSI::background_cyan);
            break;
        case Color::WHITE:
            Print(ANSI::background_white);
            break;
        case Color::NONE:
            break;
    }
}