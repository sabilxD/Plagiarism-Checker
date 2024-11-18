#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
void plagiarism_checker_t::exact_match(std::vector<int>& tokens, const int match_length,
                 std::atomic<int>& count_matches, std::atomic<bool>& plagged,
                 std::shared_ptr<submission_t>& sub1, std::shared_ptr<submission_t>& sub2){
                 count_matches+=21;
                // count_matches.fetch_add(21, std::memory_order_relaxed);


}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission) {

    auto timestamp = std::chrono::steady_clock::now();
    std::vector<int> tokens = tokenizer_t(__submission->codefile).get_tokens();
    const int matchlength=75;
    std::atomic<int> count_matches(0);
    std::atomic<bool> plagged(false);
    std::vector<std::thread> threads;

    {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto &prev_submission : submissions) {
            // Launch a thread for each previous submission comparison
            // threads.emplace_back(&plagiarism_checker_t::exact_match, this,
            //                      tokens, matchlength, count_matches, plagged,
            //                      __submission, prev_submission.second);
            threads.emplace_back([this, &tokens, matchlength, &count_matches, &plagged, 
                                  &__submission, &prev_submission]() {
                exact_match(tokens, matchlength, count_matches, plagged, __submission, prev_submission.second);
            });
        }

        // Add the new submission to the list
        submissions.push_back({timestamp,__submission});
    }

    // Join all threads to ensure all comparisons are complete
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    // TO detect patched plagiarism
    if (!plagged){
        if (count_matches>=20){
            //declare patched plag
            __submission->student->flag_student(__submission);
            __submission->professor->flag_professor(__submission);
        }
    }
}
// End TODO