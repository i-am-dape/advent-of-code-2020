#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

ABSL_FLAG(uint32_t, preamble, 25, "XMAS preamble count");

std::vector<std::string> ReadInput() {   
    std::vector<std::string> lines;
    std::string line;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        //std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
        lines.push_back(line);
    }
    return lines;
}


std::pair<int, int> FindTerms(int sum, std::deque<int> &state) {
    for (size_t i = 0; i < state.size(); i++) {
        int t0 = state[i];
        for (uint32_t j = i + 1; j < state.size(); j++) {            
            int t1 = state[j];

            if (t0 + t1 == sum) {
                return std::make_pair(t0, t1);
            }
        }
    }

    return std::make_pair(-1, -1);
}

std::vector<int> FindContTerms(int sum, std::deque<int> &state) {
    for (uint32_t i = 0; i < state.size(); i++) {
        std::vector<int> terms;
        terms.push_back(state[i]);

        for (uint32_t j = i + 1; j < state.size(); j++) {
            terms.push_back(state[j]);

            int tmp_sum = 0;
            for (auto n : terms) {
                //std::cout << n << " ";
                tmp_sum += n;
            }

            //std::cout << "=" << tmp_sum << std::endl;

            if (tmp_sum == sum) {
                return terms;
            } else if (tmp_sum > sum) {                
                break;
            }
        }
    }

    return std::vector<int>();
}

int main(int argc, char *argv[]) {
    absl::ParseCommandLine(argc, argv);
    auto preamble = absl::GetFlag(FLAGS_preamble);
    std::cout << "9" << " XMAS preamble: " << preamble << std::endl;
    auto in = ReadInput();
    std::deque<int> state;
    std::deque<int> all;

    for (size_t i = 0; i < in.size(); i++) {
        int cur = 0;
        absl::SimpleAtoi(in[i], &cur);

        if (i < preamble) {
            //std::cout << "-" << cur << std::endl;
            state.push_back(cur);
            all.push_back(cur);
            continue;
        }

        //std::cout << "+" << cur << std::endl;
        auto terms = FindTerms(cur, state);
        if (terms.first == -1) {
            std::cout << "No terms for: " << cur << std::endl;
            std::vector<int> cont_terms = FindContTerms(cur, all);
            std::cout << cont_terms.size() << std::endl;
            if (!cont_terms.empty()) {
                for (auto i : cont_terms) {
                    std::cout << i << " ";
                }
                std::cout << "=" << cur << std::endl;
                auto min = std::min_element(cont_terms.begin(), cont_terms.end());
                auto max = std::max_element(cont_terms.begin(), cont_terms.end());
                auto sum = *min + *max;
                std::cout << *min << "+" << *max << "=" << sum << std::endl;
            }

            return 1;
        }

        state.push_back(cur);
        all.push_back(cur);
        state.pop_front();
    }

    return 0;
}
