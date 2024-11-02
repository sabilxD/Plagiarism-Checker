#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <math.h> 
#include <unordered_set>
#include <unordered_map>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

const int BASE = 257; 
const int MOD = 1e9 + 7;

//Helper function for updating



// This is optimized algorithm for accurate matches using rolling hash
int calculate_hash(const std::vector<int>& vec, int start, int len) {
    int hash = 0;
    int power = 1;
    for (int i = start; i < start + len; ++i) {
        hash = (hash + 1LL * vec[i] * power % MOD) % MOD;
        power = (1LL * power * BASE) % MOD;
    }
    return hash;
}


//Fuzzy algorithm using lavenshtein
void fuzzy_approximate_match(std::array<int, 5>& result_accurate, std::vector<int>& submission1, std::vector<int>& submission2, int max_mismatches) {
    int n = submission1.size();
    int m = submission2.size();
    int longest_match = 0;
    int total_length_match = 0;
    int index_i = -1;
    int index_j = -1;
    bool in_match = false; // Tracks if we're in an ongoing match sequence
    
    // 2D DP table to store lengths of fuzzy matches
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // Iterate through both vectors
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int curr_match = 0;
            int mismatches = 0;
            
            // If elements match, extend match length
            if (submission1[i - 1] == submission2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // Allow mismatch up to max_mismatches
                dp[i][j] = dp[i - 1][j - 1] + 1; // Consider mismatch as continuation of match
                mismatches++;
            }
            

            // Check if current match satisfies mismatch tolerance
            if (mismatches <= max_mismatches) {
                curr_match = dp[i][j];
                // Update total match length for patterns of length 10-20
                if (curr_match >= 10 && curr_match <= 20) {
                    total_length_match += curr_match;
                }
                // For matches longer than 20, count in segments of 20
                else if (curr_match > 20) {
                    total_length_match += 20 * (curr_match / 20);
                    int remaining = curr_match % 20;
                    if (remaining >= 10) {
                        total_length_match += remaining;
                    }
                }
                // Update longest match information if current match is longer
                if (curr_match > longest_match) {
                    longest_match = curr_match;
                    index_i = i - curr_match;
                    index_j = j - curr_match;
                }
                // total_length_match += curr_match;
            } else {
                dp[i][j] = 0;  // Reset match length if mismatches exceed tolerance
            }
        }
    }

    // Store results
    result_accurate[2] = longest_match;
    result_accurate[3] = index_i;
    result_accurate[4] = index_j;

}

//Tried for approximate matches using winnowing and rolling hash
int hashWindow(const std::vector<int>& vec, int start, int windowSize) {
    int hash = 0;
    for (int i = start; i < start + windowSize; ++i) {
        hash = (1LL * hash * 31 + vec[i]) % MOD;
    }
    return hash;
}

std::unordered_set<int> winnowing(const std::vector<int>& vec, int windowSize) {
    std::unordered_set<int> fingerprints;
    int n = vec.size();
    
    // Loop over the vector with a sliding window
    for (int i = 0; i <= n - windowSize; ++i) {
        int hashValue = hashWindow(vec, i, windowSize);
        fingerprints.insert(hashValue); // Store the minimum hash in each window
    }
    return fingerprints;
}

bool longestApproximateMatch(const std::vector<int>& vec1, const std::vector<int>& vec2, int windowSize, int& matchLength) {
    std::unordered_set<int> fingerprints1 = winnowing(vec1, windowSize);
    std::unordered_set<int> fingerprints2 = winnowing(vec2, windowSize);

    // Find the common fingerprints
    std::vector<int> commonFingerprints;
    for (const int& fp : fingerprints1) {
        if (fingerprints2.find(fp) != fingerprints2.end()) {
            commonFingerprints.push_back(fp);
        }
    }
    
    matchLength = commonFingerprints.size();
    return matchLength;
}





void update( int&curr_match  , int&total_length_match, int&min_length , int&i , int &j , std::unordered_map<int , int>&match_length , std::vector<bool>&match_at_index , std::vector<bool>&match_at_index2)  {

    if ( curr_match >= min_length) { 

        if ( match_length[i] < curr_match ) { 

            int increment = curr_match - match_length[i] ;
            
            total_length_match +=  increment ;
            
            match_length[i] = curr_match ;

            std::cout << " match found at " << i << " " << j << " of length: " << curr_match << " " << increment << std::endl ;

            for ( int idx = i ; idx < i + curr_match ; idx++ ) { 
                match_at_index[idx] = true ;
            }

            for ( int idx = j ; idx < j + curr_match ; idx++ ) { 
                match_at_index2[idx] = true ;
            }
        }
    }

    curr_match = 0 ;

}




void match ( std::vector<int> &submission1 , std::vector<int> &submission2 , std::vector<bool> &match_at_index_i , std::vector<bool> &match_at_index_j , std::unordered_map<int , int> &match_length_at_i , int &min_length , int &total_length_match ) { 


    for ( int i  = 0 ; i< submission1.size() ; i++) { 
        
        if ( match_at_index[i] ) continue ;

        for ( int j = 0 ; j < submission2.size() ; j++ ) { 
            
            if ( match_at_index2[j] ) continue ;

            //current running match's length
            int curr_match = 0 ; 

            int M = std::min(submission1.size() - i , submission2.size() - j) ;
            for ( int l = 0 ; l < std::min(M ,20) ; l++ ) { 
                
                if ( submission1[i+l] == submission2[j+l] ) { 
                    curr_match++ ;

                    if ( l == std::min(M ,20) - 1 ) { 
                        update(curr_match , total_length_match, min_length , i , j , match_length , match_at_index , match_at_index2)  ;
                        j = j + l ;
                        break ;
                    }

                }
                
                else { 
                    update(curr_match , total_length_match, min_length , i , j , match_length , match_at_index , match_at_index2)  ;
                    j = j + l ;
                    break ;                  
                }

            }


        }

    }   


}









void accurate_matching( std::array<int,5> result, std::vector<int> &submission1 , std::vector<int> &submission2  ) { 

    int min_length = 10 ; 
    int total_length_match = 0 ; 
   
    std::vector<bool> match_at_index_i(submission1.size() , false) ;
    std::vector<bool> match_at_index_j(submission2.size() , false) ;

    std::unordered_map<int , int> match_length_at_i ;

    match( submission1 , submission2 , match_at_index_i , match_at_index_j , match_length_at_i , min_length , total_length_match ) ;

    
    result[1] = total_length_match ;
    if ( double(total_length_match)/submission2.size() > 0.5 ) { 
        result[0] = 1 ; 
    }

    

    return ;
}










// Calling main function
std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};


    /*----------------------------uncomment for debugging----------------------
    
     for ( int i = 0 ; i < submission1.size() ; i++) { 
         std::cout << i << " "<< submission1[i]  << std::endl; 
     }

     for ( int i = 0 ; i < submission2.size() ; i++) { 
         std::cout << i << " "<< submission2[i]  << std::endl; 
     }


    ------------------------------------------------------------------------*/




    //Accurate matching
    accurate_matching( result , submission1 , submission2 ) ;




    /* Aditi and aakash please write the case for longer strings with 80% matchings here ie fuzzy matching and stuff ;)*/
    int windowSize = 10; 
    int threshold = std::fmax(submission1.size(), submission2.size()) * 0.2;
        
    fuzzy_approximate_match(result,submission1,submission2,threshold);

    
    /*----------------------------------------------------------------*/



    // if(longestMatch>=threshold) result_accurate[0]=1;
    // if(longestMatch>=result_accurate[2]) result_accurate[2]=longestMatch;
    // else result_accurate[2]=longest_match;
    // result_accurate[2]=longestMatch;
    // std::cout << result_accurate[2] << std::endl ;
    // std::cout << longestMatch << std::endl ;
    // std::cout << submission1.size() << std::endl ;
    // std::cout << submission2.size() << std::endl ;
    return result;
    // End TODO

}      
