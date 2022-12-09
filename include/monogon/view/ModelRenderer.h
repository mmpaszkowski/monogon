//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_MODELRENDERER_H
#define MONOGON_MODELRENDERER_H

#include <indicators/progress_bar.hpp>
#include <indicators/cursor_control.hpp>

using namespace indicators;

class ModelRenderer
{
public:
    ModelRenderer():
        bar{
                option::BarWidth{30},
                option::Start{"["},
                option::Fill{"="},
                option::Lead{">"},
                option::Remainder{"."},
                option::End{"]"},
                option::ShowRemainingTime{true}
        }
    {

    }

public:
    template<typename T>
    void render_progress_bar(size_t iteration, size_t total, T loss)
    {
        show_console_cursor(false);
        bar.set_option(option::PrefixText{std::to_string(iteration) + "/" + std::to_string(total) + " "});
        bar.set_option(option::PostfixText{"- loss: " + std::to_string(loss)});
        bar.set_progress(100*iteration/total);
        show_console_cursor(true);
    }

    void render_epoch(size_t iteration, size_t total)
    {
        std::cout << "Epoch " << iteration << "/" << total << std::endl;
    }

private:
    ProgressBar bar;
};

#endif //MONOGON_MODELRENDERER_H
