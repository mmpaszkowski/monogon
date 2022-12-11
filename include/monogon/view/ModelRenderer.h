//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_MODELRENDERER_H
#define MONOGON_MODELRENDERER_H

#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>

using std::to_string;
using indicators::ProgressBar;
using indicators::show_console_cursor;
using namespace indicators::option;

class ModelRenderer
{
public:
    ModelRenderer()
        : bar{BarWidth{30}, Start{"["}, Fill{"="}, Lead{">"}, Remainder{"."}, End{"]"}, ShowRemainingTime{true}}
    {
    }

public:
    template <typename T>
    void render_progress_bar(size_t iteration, size_t total, T loss, T accuracy)
    {
        show_console_cursor(false);
        if (iteration == total)
        {
            bar.set_option(ShowRemainingTime{false});
            bar.set_option(ShowElapsedTime{true});
        }
        bar.set_option(PrefixText{to_string(iteration) + "/" + to_string(total) + " "});
        bar.set_option(PostfixText{"- loss: " + to_string(loss) + " - categorical_accuracy " + to_string(accuracy)});
        bar.set_progress(100 * iteration / total);
        show_console_cursor(true);
    }

    void render_epoch(size_t iteration, size_t total)
    {
        std::cout << "Epoch " << iteration << "/" << total << std::endl;
    }

    void finish()
    {
        std::cout << std::endl;
    }

private:
    ProgressBar bar;
};

#endif //MONOGON_MODELRENDERER_H
