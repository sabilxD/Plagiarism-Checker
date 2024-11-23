#include "structures.hpp"
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
#include <chrono>
#include <atomic>
#include <mutex>
#include <thread>
#include <memory>
#include <math.h>
#include <unordered_map>
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here


class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    void exact_match(std::vector<int> tokens, const int match_length,
                     std::atomic<int>& count_matches, std::atomic<bool>& plagged,
                     std::shared_ptr<submission_t> sub1, std::chrono::time_point<std::chrono::steady_clock> timestamp1,
                     std::shared_ptr<submission_t> sub2, std::chrono::time_point<std::chrono::steady_clock> timestamp2, bool pre_plagged);

    bool is_submission_flagged(std::shared_ptr<submission_t>& submission);

    void set_submission_flagged(std::shared_ptr<submission_t>& submission);


    std::vector<std::pair<std::pair<std::chrono::time_point<std::chrono::steady_clock>,bool>,
                        std::shared_ptr<submission_t>>> submissions;
    std::mutex mtx;
    auto timestamp ;
    // End TODO
};
