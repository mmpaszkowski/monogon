//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_MODELRENDERER_H
#define MONOGON_MODELRENDERER_H

#include <indicators/progress_bar.hpp>

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
        }
    {

    }

public:
    template<typename T>
    void render(size_t iteration, size_t total, T loss)
    {
        bar.set_option(option::PrefixText{std::to_string(iteration) + "/" + std::to_string(total) + " "});
        bar.set_option(option::PostfixText{" - ETA: 0s - loss: " + std::to_string(loss)});
        bar.set_progress(100*iteration/total);
    }

private:
    ProgressBar bar;
};

#endif //MONOGON_MODELRENDERER_H
