#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <math.h> 
#include <unordered_set>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here


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

const int MOD = 1000000007;
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
    accurate(result_accurate,submission1,submission2);

    // now we have the longest match and the total length of matches



    /* Aditi and aakash please write the case for longer strings with 80% matchings here ie fuzzy matching and stuff ;)*/
    
    
    int windowSize = 10; 
    int longestMatch;
    longestApproximateMatch(submission1, submission2, windowSize,longestMatch);
    int threshold = std::fmax(submission1.size(), submission2.size()) * 0.8;
    

    
    /*----------------------------------------------------------------*/


    if(longestMatch>=threshold) result_accurate[0]=1;
    if(longestMatch>=result_accurate[2]) result_accurate[2]=longestMatch;
    // else result_accurate[2]=longest_match;
    // result_accurate[2]=longestMatch;
    // std::cout << result_accurate[2] << std::endl ;
    // std::cout << longestMatch << std::endl ;
    // std::cout << submission1.size() << std::endl ;
    // std::cout << submission2.size() << std::endl ;
    return result_accurate;
    // End TODO

}      


