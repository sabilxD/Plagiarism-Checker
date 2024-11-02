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
void update(int &index_i , int&index_j , int&curr_match , int&longest_match , int&total_length_match, int&min_length , int&i , int &j) {

    if ( curr_match >= min_length) { 
        total_length_match += curr_match ;
        // std::cout << " match found at " << i << " " << j << " length: " << curr_match << std::endl ;
    }
    
    if ( curr_match > longest_match ) { 
        longest_match = curr_match ; 
        index_i = i ; 
        index_j = j ; 
    }
    
    curr_match = 0 ;
    
}

int total_length(std::vector<int>& submission1, std::vector<int>& submission2) {
    int n = submission1.size();
    int m = submission2.size();
    int total_length_match = 0; // Total length of non-overlapping accurate matches of length 10-20
    int last_end_index1 = -1; // End index of the last counted non-overlapping match in submission1
    int last_end_index2 = -1; // End index of the last counted non-overlapping match in submission2

    // Iterate through both vectors
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int curr_match_length = 0;

            // Check for matches starting from indices i in submission1 and j in submission2
            while (i + curr_match_length < n && j + curr_match_length < m &&
                   submission1[i + curr_match_length] == submission2[j + curr_match_length]) {
                curr_match_length++;
            }

            // Check if the current match is of length between 10 and 20 and non-overlapping
            if (curr_match_length >= 10 && curr_match_length <= 20 && 
                i > last_end_index1 ) {
                
                total_length_match += curr_match_length;

                // Update last_end_index to prevent overlaps
                last_end_index1 = i + curr_match_length - 1; // End of current match in submission1
            }
            
        }
    }

    return total_length_match;
}

//This is brute force for accurate matching
void accurate(std::array<int,5>& result_accurate,std::vector<int> &submission1, 
        std::vector<int> &submission2){
    int min_length = 10 ; 

    int total_length_match = 0 ; 
    int longest_match = 0 ; 
    int index_i = -1 ;
    int index_j = -1 ;
    

    //stores whether the a match has occured at from index i in submission1 and index j in submission2
    std::vector<bool> match_at_index(submission1.size()*submission2.size() , false) ;



    //this loop checks for a match starting at index i and index j in submission1 and submission2 respectively
    for ( int i  = 0 ; i< submission1.size() ; i++) { 
        for ( int j = 0 ; j < submission2.size() ; j++ ) { 


            //if a match has already been found at this index, skip this iteration
            if ( match_at_index[i*submission2.size() + j] ) continue ;
            

            //current running match's length
            int curr_match = 0 ; 


            for ( int l = 0 ; l < std::min(submission1.size() - i , submission2.size() - j) ; l++ ) { 
                
                if ( submission1[i+l] == submission2[j+l] ) { 

                    match_at_index[(i+l)*submission2.size() + j+l ] = true ;
                    curr_match++ ;

                    /*if the match is of length min_length, then we cover it here as it will not be covered as
                    we skip to the next j and set curr_match to 0 and the this current matching gets lost
                    */
                    int last_index = std::min(submission1.size() - i , submission2.size() - j) - 1 ;
                    bool last = ( l == last_index) ;
                    if (last) {
                        update(index_i , index_j , curr_match , longest_match , total_length_match, min_length , i , j ) ;
                    }

                }
                
                else { 
                    update(index_i , index_j , curr_match , longest_match , total_length_match, min_length , i , j ) ;
                    break ;  
                
                }

            }


        }

    }
    result_accurate[1] = total_length_match ;
    result_accurate[2] = longest_match ;
    result_accurate[3] = index_i ;
    result_accurate[4] = index_j ;

    if ( double(total_length_match)/submission2.size() > 0.5 ) { 
        result_accurate[0] = 1 ; 
    }
}


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

void accurate_optimized(std::array<int, 5>& result_accurate, std::vector<int>& submission1, std::vector<int>& submission2) {
    int min_length = 10;
    int total_length_match = 0;
    int longest_match = 0;
    int index_i = -1;
    int index_j = -1;

    std::unordered_map<int, std::vector<int>> hash_map;

    // Compute hashes for all starting positions in submission1 for `min_length` substrings
    for (int i = 0; i + min_length <= submission1.size(); ++i) {
        int hash = calculate_hash(submission1, i, min_length);
        hash_map[hash].push_back(i);
    }

    // Try matching each `min_length` substring in submission2 with entries in submission1
    for (int j = 0; j + min_length <= submission2.size(); ++j) {
        int hash = calculate_hash(submission2, j, min_length);

        if (hash_map.find(hash) != hash_map.end()) {
            for (int i : hash_map[hash]) {
                int curr_match =0;

                // Extend the match beyond `min_length` if possible
                while (i + curr_match < submission1.size() && j + curr_match < submission2.size()
                       && submission1[i + curr_match] == submission2[j + curr_match]) {
                    curr_match++;
                }

                // Update match statistics
                update(index_i, index_j, curr_match, longest_match, total_length_match, min_length, i, j);
            }
        }
    }

    result_accurate[1] = total_length(submission1,submission2);
    result_accurate[2] = longest_match;
    result_accurate[3] = index_i;
    result_accurate[4] = index_j;

    if (double(total_length_match) / submission2.size() > 0.5) {
        result_accurate[0] = 1;
    }
}

//Fuzzy algorithm using lavenshtein


void fuzzy_approximate_match(std::array<int, 5>& result_accurate, std::vector<int>& submission1, std::vector<int>& submission2, int max_mismatches) {
    int n = submission1.size();
    int m = submission2.size();
    int longest_fuzzy_match = 0; // Track the longest fuzzy match
    int index_i = -1;
    int index_j = -1;

    // Iterate through both vectors
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int curr_match_length = 0;
            int mismatches = 0;

            // Check for matches starting from indices i and j
            int x = i, y = j;
            while (x < n && y < m && mismatches <= max_mismatches) {
                if (submission1[x] != submission2[y]) {
                    mismatches++;
                }
                if (mismatches > max_mismatches) {
                    break;
                }
                curr_match_length++;
                x++;
                y++;
            }
            if (curr_match_length > longest_fuzzy_match) {
                    longest_fuzzy_match = curr_match_length;
                    index_i = i;
                    index_j = j;
                }
        }
    }

    // Store results
    result_accurate[1] = total_length(submission1,submission2);
    result_accurate[2] = longest_fuzzy_match;
    result_accurate[3] = index_i;
    result_accurate[4] = index_j;

    if (double(result_accurate[1]) / m > 0.5) {
        result_accurate[0] = 1;
    }
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

// Calling main function
std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result_accurate = {0, 0, 0, 0, 0};

    //----------------------------uncomment for debugging----------------------
    
    // for ( int i = 0 ; i < submission1.size() ; i++) { 
    //     std::cout << i << " "<< submission1[i]  << std::endl; 
    // }

    // for ( int i = 0 ; i < submission2.size() ; i++) { 
    //     std::cout << i << " "<< submission2[i]  << std::endl; 
    // }


    //------------------------------------------------------------------------*/
    //checking for short length matchings for accurate subsequence matchings first
    // accurate(result_accurate,submission1,submission2);
    // accurate_optimized(result_accurate,submission1,submission2);
    // now we have the longest match and the total length of matches



    /* Aditi and aakash please write the case for longer strings with 80% matchings here ie fuzzy matching and stuff ;)*/
    
    
    int windowSize = 10; 
    int longestMatch;
    // longestApproximateMatch(submission1, submission2, windowSize,longestMatch);
    // std::cout<<longestMatch<<std::endl;
    int threshold = std::fmax(submission1.size(), submission2.size()) * 0.2;
    fuzzy_approximate_match(result_accurate,submission1,submission2,threshold);
    
    /*----------------------------------------------------------------*/


    // if(longestMatch>=threshold) result_accurate[0]=1;
    // if(longestMatch>=result_accurate[2]) result_accurate[2]=longestMatch;
    // else result_accurate[2]=longest_match;
    // result_accurate[2]=longestMatch;
    // std::cout << result_accurate[2] << std::endl ;
    // std::cout << longestMatch << std::endl ;
    // std::cout << submission1.size() << std::endl ;
    // std::cout << submission2.size() << std::endl ;
    return result_accurate;
    // End TODO

}      
