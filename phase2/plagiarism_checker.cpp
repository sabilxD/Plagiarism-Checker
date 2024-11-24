#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
plagiarism_checker_t::plagiarism_checker_t(void) {

    finished = false;
    // Start the processing thread
    processing_thread = std::thread(&plagiarism_checker_t::queue_processing, this);
    startTime=std::chrono::steady_clock::now();
}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions) {

    finished = false;
    processing_thread=std::thread(&plagiarism_checker_t::queue_processing, this);
    for (auto sub : __submissions){
        std::vector<int> token = tokenizer_t(sub->codefile).get_tokens();
        submissions.push_back(std::make_tuple(0,0,sub,token));
    }
    startTime=std::chrono::steady_clock::now();

}

plagiarism_checker_t::~plagiarism_checker_t(void) {
    // {
    //     std::unique_lock<std::mutex> lock(mtx);
    //     // Ensure all items are processed before the destructor ends
    //     while (!unchecked.empty()) {
    //         cv.wait(lock);
    //     }
    //     finished = true;
    // }
    // cv.notify_one();
    {
        std::unique_lock<std::mutex> lock(mtx);
        finished = true; // Set finished flag early
        cv.notify_all(); // Notify the thread to exit immediately
    }
    if (processing_thread.joinable()) {
        processing_thread.join();
    }
    submissions.clear();
}

void update( int&curr_match  , std::vector<int>& matches, int&min_length , int&i , int &j ,
             std::unordered_map<int , int>&match_length , std::vector<bool>&match_at_index_i , std::vector<bool>&match_at_index_j)  {

    if ( curr_match >= min_length) { 

        if ( match_length[i] < curr_match ) { 

            int increment = curr_match - match_length[i] ;
            
            matches.push_back(increment);
            match_length[i] = curr_match ;
            // std::cout << " match found at " << i << " " << j << " of length: " << curr_match << " " << increment << std::endl ;

            for ( int idx = i ; idx < i + curr_match ; idx++ ) { 
                match_at_index_i[idx] = true ;
            }

            for ( int idx = j ; idx < j + curr_match ; idx++ ) { 
                match_at_index_j[idx] = true ;
            }
        }
    }

    curr_match = 0 ;

}

void match ( std::vector<int> &submission1 , std::vector<int> &submission2 , std::vector<bool> &match_at_index_i ,
             std::vector<bool> &match_at_index_j , std::unordered_map<int , int> &match_length_at_i ,
             int &min_length , std::vector<int>& matches ) { 

    for ( int i  = 0 ; i< submission1.size() ; i++) { 
        
        if ( match_at_index_i[i] ) continue ;

        for ( int j = 0 ; j < submission2.size() ; j++ ) { 
            
            if ( match_at_index_j[j] ) continue ;

            //current running match's length
            int curr_match = 0 ; 

            int M = std::min(submission1.size() - i , submission2.size() - j) ;
            for ( int l = 0 ; l < M ; l++ ) { 
                
                if ( submission1[i+l] == submission2[j+l] ) { 
                    curr_match++ ;

                    if ( l == M - 1 ) { 
                        update(curr_match , matches , min_length , i , j , match_length_at_i , match_at_index_i , match_at_index_j)  ;
                        j = j + l ;
                        break ;
                    }

                }
                
                else { 
                    update(curr_match , matches, min_length , i , j , match_length_at_i , match_at_index_i , match_at_index_j)  ;
                    j = j + l ;
                    break ;                  
                }

            }


        }

    }   
}

std::vector<int> find_matches(std::vector<int>& submission1, std::vector<int>& submission2,int min_length) {

    int total_length_match = 0 ; 
   
    std::vector<bool> match_at_index_i(submission1.size() , false) ;
    std::vector<bool> match_at_index_j(submission2.size() , false) ;

    std::unordered_map<int , int> match_length_at_i ;
    std::vector<int> matches;
    match( submission1 , submission2 , match_at_index_i , match_at_index_j , match_length_at_i , min_length , matches ) ;

    return matches;
}

void plagiarism_checker_t::check_plagiarism(std::pair<double,std::shared_ptr<submission_t>> sub1){
    std::vector<int> token1 = tokenizer_t(sub1.second->codefile).get_tokens();
    int total_matches=0;
    bool is_plagged=false;
    for (int i=0; i<submissions.size();i++){
        auto sub2=submissions[i];
        if(!sub1.second || !std::get<2>(sub2)) return;
        // std::vector<int> token2 = tokenizer_t(sub2.second->codefile).get_tokens();
        std::vector<int> matches=find_matches(token1,std::get<3>(sub2),15);
        // if (sub1.second->codefile=="ainur/destruction.cpp" 
        // // && sub2.second->codefile=="ainur/design_ea.cpp"
        // ){
        //     for (auto j:matches){
        //         std::cout<<j<<" ";
        //     }
        // }
        // std::cout<<std::endl;
        for(auto count:matches){
            if(count>=75 || matches.size()>=10){
                is_plagged=true;
                if(std::get<1>(sub2)==1 && sub1.first-std::get<0>(sub2)<=1000 ){
                    // std::cout<<"Is plagged by "<<sub1.second->codefile<<std::endl;
                    std::get<2>(sub2)->student->flag_student(std::get<2>(sub2));
                    std::get<2>(sub2)->professor->flag_professor(std::get<2>(sub2));
                    std::get<1>(submissions[i]) =2;
                    std::get<1>(sub2)=2;
                }             
            }
            total_matches++;
        }
    }
    if ((total_matches>=20 || is_plagged)){

        sub1.second->student->flag_student(sub1.second);
        sub1.second->professor->flag_professor(sub1.second);
        submissions.push_back(std::make_tuple(sub1.first,2,sub1.second,token1));
    }
    else{
        submissions.push_back(std::make_tuple(sub1.first,1,sub1.second,token1));
    }

}

void plagiarism_checker_t::queue_processing() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait until the queue is not empty or finished flag is true
        cv.wait(lock, [&]() { return !unchecked.empty() || finished; });

        if (finished && unchecked.empty()) break; // Exit if no more work to do

        // Process all elements in the queue
        while (!unchecked.empty()) {
            auto item = unchecked.front();
            unchecked.pop();

            lock.unlock();  
            check_plagiarism(item);  
            lock.lock();   
        }
    }
}


void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    auto timestamp = std::chrono::steady_clock::now();
    {
        // std::lock_guard<std::mutex> lock(mtx);
        std::unique_lock<std::mutex> lock(mtx);
        auto duration = timestamp.time_since_epoch();
        double timeDiffMillis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(timestamp - startTime).count();
        unchecked.push(std::make_pair(timeDiffMillis, __submission));
    }
    cv.notify_one();  // Notify the processing thread
}
// End TODO