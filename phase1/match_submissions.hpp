#include <array>
#include <iostream>
#include <span>
#include <vector>
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


std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};

    
    
    
    /*----------------------------uncomment for debugging----------------------
    
    // for ( int i = 0 ; i < submission1.size() ; i++) { 
    //     std::cout << i << " "<< submission1[i]  << std::endl; 
    // }

    // for ( int i = 0 ; i < submission2.size() ; i++) { 
    //     std::cout << i << " "<< submission2[i]  << std::endl; 
    // }


    ------------------------------------------------------------------------*/





    //checking for short length matchings for accurate subsequence matchings first
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

    // now we have the longest match and the total length of matches



    /* Aditi and aakash please write the case for longer strings with 80% matchings here ie fuzzy matching and stuff ;)*/
    
    
        
    
    
    
    

    
    /*----------------------------------------------------------------*/




    result[1] = total_length_match ;
    result[2] = longest_match ;
    result[3] = index_i ;
    result[4] = index_j ;

    if ( double(total_length_match)/submission2.size() > 0.5 ) { 
        result[0] = 1 ; 
    }
    // std::cout << submission1.size() << std::endl ;
    // std::cout << submission2.size() << std::endl ;
    return result; // dummy return
    // End TODO

}      


