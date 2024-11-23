#include "structures.hpp"
#include <vector>
#include <queue>
#include <memory>
#include <chrono>
#include <thread>
#include <condition_variable> 
#include <mutex>
#include <math.h>
#include <unordered_map>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
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
    std::vector<std::pair<std::pair<double,int>,std::shared_ptr<submission_t>>> submissions;
    // double is for time and int is for type of file. Original->0, Not-Plagged->1, Plagged->2
    std::queue<std::pair<double,std::shared_ptr<submission_t>>> unchecked;
    std::thread processing_thread;
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;

    void queue_processing();
    void check_plagiarism(std::pair<double,std::shared_ptr<submission_t>> sub1);

    
    // End TODO
};
