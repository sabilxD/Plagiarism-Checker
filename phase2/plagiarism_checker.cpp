#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class

// Mutex for synchronizing flagging
std::mutex flagging_mutex;

void plagiarism_checker_t::exact_match(std::vector<int> tokens, const int match_length,
                 std::atomic<int>& count_matches, std::atomic<bool>& plagged,
                 std::shared_ptr<submission_t> sub1,std::chrono::time_point<std::chrono::steady_clock> timestamp1,
                 std::shared_ptr<submission_t> sub2, std::chrono::time_point<std::chrono::steady_clock> timestamp2, bool pre_plagged){

                 if(!sub1 || !sub2) return;

                 bool long_match=false;
                 int match_counts=0;

                 std::vector<int> pre_tokens=tokenizer_t(sub2->codefile).get_tokens();

                 //code for matches
                int found_k;
                 for (int i = 0; i + 15 <= tokens.size(); ++i) {
                    for (int j = 0; j + 15 <= pre_tokens.size(); ++j) {
                        int k = 0;
                        while (i + k < tokens.size() && j + k < pre_tokens.size() && tokens[i + k] == pre_tokens[j + k]) {
                            ++k;
                        }
                        if (k >= 15) {
                            ++match_counts;
                        }
                        if (k >= 75) {
                            long_match = true;
                            found_k=k;
                        }
                    }
                }

                count_matches+=match_counts;
                if(long_match || match_counts >= 10) {
                    plagged=true;
                    auto diff=timestamp1-timestamp2;
                    auto time_diff=std::chrono::duration_cast<std::chrono::seconds>(diff).count();
                    bool is_original=(timestamp2 != std::chrono::time_point<std::chrono::steady_clock>());

                    // Debugging
                    // std::cout<<time_diff<<"for " <<sub1->codefile<< " and " <<sub2->codefile<< "of max length" <<found_k <<std::endl;

                    // Lock the mutex to prevent race conditions
                    std::lock_guard<std::mutex> lock(flagging_mutex);

                    if (abs(time_diff) >= 1) {
                        // Flag only the later submission if not already flagged
                        if (time_diff > 0) {
                            if (sub1 && !is_submission_flagged(sub1)) {
                                sub1->student->flag_student(sub1);
                                sub1->professor->flag_professor(sub1);
                                set_submission_flagged(sub1);  // Mark as flagged
                            }
                        } else {
                            if (sub2 && !is_submission_flagged(sub2) && !is_original) {
                                sub2->student->flag_student(sub2);
                                sub2->professor->flag_professor(sub2);
                                set_submission_flagged(sub2);  // Mark as flagged
                            }
                        }
                    } else {
                        // Flag both submissions if not already flagged
                        if (sub1 && !is_submission_flagged(sub1)) {
                            sub1->student->flag_student(sub1);
                            sub1->professor->flag_professor(sub1);
                            set_submission_flagged(sub1);  // Mark as flagged
                        }
                        if (sub2 && !is_submission_flagged(sub2) && !is_original) {
                            sub2->student->flag_student(sub2);
                            sub2->professor->flag_professor(sub2);
                            set_submission_flagged(sub2);  // Mark as flagged
                        }
                    }
                }
}

bool plagiarism_checker_t::is_submission_flagged(std::shared_ptr<submission_t>& submission) {
    // Iterate over submissions and check if it is flagged
    for (auto& sub : submissions) {
        if (sub.second == submission && sub.first.second) {
            return true;  // Already flagged
        }
    }
    return false;
}

void plagiarism_checker_t::set_submission_flagged(std::shared_ptr<submission_t>& submission) {
    // Mark the submission as flagged
    for (auto& sub : submissions) {
        if (sub.second == submission) {
            sub.first.second = true;  // Set flagged status
            break;
        }
    }
}

plagiarism_checker_t::plagiarism_checker_t(void){
}

plagiarism_checker_t::~plagiarism_checker_t(void) {
    submissions.clear();
}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions){
    for (auto& sub : __submissions) {
        submissions.push_back({{std::chrono::steady_clock::time_point(),false}, sub});
    }
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

            auto prev_sub = prev_submission.second;
            auto prev_timestamp = prev_submission.first.first;
            bool pre_plagged=prev_submission.first.second;

            threads.emplace_back([this, tokens, &count_matches, &plagged,
                                  __submission, timestamp, prev_sub, prev_timestamp,pre_plagged]() {
                exact_match(tokens, matchlength, count_matches, plagged, __submission,
                            timestamp, prev_sub, prev_timestamp, pre_plagged);
            });
        }

        // Add the new submission to the list
        submissions.push_back({{timestamp,plagged},__submission});
    }

    // Join all threads to ensure all comparisons are complete
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    // std::cout<<__submission->codefile <<" count " <<count_matches<<std::endl;
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