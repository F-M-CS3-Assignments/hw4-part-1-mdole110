#include "bdc.h"

#include <vector>
#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <climits>
#include <algorithm>

#define NPOS UINT32_MAX

using namespace std;

//JUST FIX LIST FORMAT and write tests

string vec_to_string(vector<int> v){
//just for printing the output/input?
    string vec_as_string = "{";
    for(int i=0; i<v.size(); i++){
        string char_to_append = to_string(v[i]); //to_string: convert int to string https://www.geeksforgeeks.org/converting-number-to-string-in-cpp/
        vec_as_string = vec_as_string + char_to_append;
        //vec_as_string = vec_as_string + ",";
        //set up output to look like the output on the hmwk directions
        //space between each number, [ before and after ]
        if(i!= v.size()-1){
            vec_as_string += ", ";
        }
        
    }   
    vec_as_string += "}";

    return vec_as_string;
}




vector<int> longest_vector(vector<vector<int>>& candidates){
    vector<int> longest_vector;
    for(int i=0; i<candidates.size(); i++){
        if(candidates[i].size() > longest_vector.size()){
            longest_vector = candidates[i];
        }

    }
    return longest_vector;
}
int find_next_dividend_position(const vector<int>& input, int start_index){
    //cout << "find_next_dividend_position function ENTERED" << endl;
    for(int i=start_index; i<input.size(); ++i){
        //cout<< input[i] << "%" << input[start_index-1] << " = " << input[i]%input[start_index-1]<< endl;
        if(input[i]%input[start_index-1]==0){
            return i;
        }
        
    }
    return -1; //aka else (if not divisible number is found)
    
}
vector<int> sub_vec(vector<int> input, int next_dividend_position){
    //extract the vector from the position j to the end
    vector<int> new_sub_vector;
    if(next_dividend_position >0) {
        for(int i=next_dividend_position; i<input.size(); i++){
            new_sub_vector.push_back(input[i]);
        }
        //cout << "new sub vec: " << vec_to_string(new_sub_vector);
        return new_sub_vector;
    }
    else{
        return new_sub_vector;
    }
}


//combine function 
vector<int> combine(vector<int>& L, vector<int>& R){

    if(R.size() >0 && R.at(0) % L.at(0) !=0){
        return L;
    }
    vector<int> output;
    for (size_t i=0; i< L.size(); i++){
        output.push_back(L.at(i));
    }
    for(size_t i=0; i< R.size(); i++){
        output.push_back(R.at(i));
    }
    return output;
    
    /*
    vector<int> combined_vec = L;
    //start with L then for loop to add R (push back will add to end)
    for(int i=0; i<R.size(); i++){
        combined_vec.push_back(R[i]);
    }
    //return combined vec
    return combined_vec;
    */
}

vector<int> bdc_helper(vector<int>& input){
    
    //int next_dividend_position;
    if(input.empty() == true || input.size() == 1){
        //cout << "based case reached" << endl;
        return input;
    }

    vector<vector<int>> candidates;//starting a vector with a 2D format
    
    for(size_t i = 0; i<input.size(); i++){ //go thru alll elements in the input
        //cout << "running bdc_helper from index " << i << endl;
        //cout << "lead value: " << input[i] << endl;
        vector<int> L = {input[i]};
        
        //L.push_back(input[i]);//append the current input to L
        //now find the next number in the list (input) that is divisible by input[i] (currently only int in L)
        
        int next_dividend_position = find_next_dividend_position(input, i+1);
        /*
        The division and recurison is happening but the problem I am facing right now is that once I get the 
        number on the left to be [2] for example and the next subset to be [8....56], I recurse the function with
        left = [2]
        right = [8 ...56]
        but then when it finds 24 and recurses the function, it does so with 
        left = [8] (for i = input[i].... and L= {input[i]}) //...whatever the for loop is starting with becomes L and everything before is lost
        right = [24...56]
        HOW do i prevent everything before the current index of the for loop from being lost
        R is not keeping the values that are found to be viable dividends 
        ...
        also, by this principle when the final number is run and no dividend, the number after that final number becomes left and there is no right
        so then the answer/candidate is just the next number (lead value that was LOOKING for its next dividend)
        */

        
        // handle the case when no divisible number is found 
        if(next_dividend_position == -1){
            candidates.push_back(L);
            continue;
        }
            

        //THIS FIXES THE SECOND PROBLEM I THINK
        vector<int> Rin = sub_vec(input, next_dividend_position);
        //call the recursive function with the new subvector
        //cout << "left: " << vec_to_string(L)<< endl;
        //cout << "subset: " << vec_to_string(Rin) << endl;
        vector<int> R = bdc_helper(Rin); //R as in Right-hand side
        
        //cout << "right: " << vec_to_string(R) << endl;
    
            
            vector<int> cand_v = combine(L,R);
            //append this new candidate vector to the candidates vector
            candidates.push_back(cand_v);
            //cout << "found candidate: " << vec_to_string(cand_v) << endl;
            //cout << "candidates list: " << vec_to_string(candidates) << endl;
       

       }
    //cout << "returning longest vector: " << vec_to_string(longest_vector(candidates)) << endl;
    return longest_vector(candidates);
}

vector<int> biggest_divisible_conglomerate(vector<int> input){
// base cases 
//make the output vector 
//cout << "pre-sort: " << vec_to_string(input); //SORTING WORKS
//vector<int>output;
vector<int> curr_subset;
 //sort in decreasing order to make sure the L%S (L>S) requirement is met and not messing with calculations
    for(int i=0; i<input.size()-1; i++){
    //find the smallest unsorted number, move to back
    int smallestIndex = i;
    for(int j=i+1; j<input.size(); j++){
        if(input[j] < input[smallestIndex]){ //want the list to start w/ largest and decrease 
            smallestIndex = j;
        }
    }
    //use a third vaiable to swap
    if(smallestIndex!=i){//aka if not alr sorted
        int temporaryIndex = input[i];
        input[i] = input[smallestIndex];
        input[smallestIndex] = temporaryIndex;
    }
    }

    //NOW find the largest conglomerate 
    //add the first element to the subset because it can be divided by itself 
    //call bdc helper
   
    //cout << "output: " << vec_to_string(output);

    //CALL HELPER FUNCTION
    //cout << "running 2 " << endl;
    vector<int> output = bdc_helper(input);
    return output;
}



int main() {
    //vector<int> values = {28, 22, 7, 2, 8, 14, 24, 56};
    //vec_to_string(values);
    //biggest_divisible_conglomerate(values);

	// Test 1 (example from assignment description)
	vector<int> values = {28, 22, 7, 2, 8, 14, 24, 56};
	vector<int> ans = biggest_divisible_conglomerate(values);
	cout << "input: " << vec_to_string(values) << endl;
	cout << "output: " << vec_to_string(ans) << endl << endl;
	set<int> answerSet(ans.begin(), ans.end());
	set<int> soln = {56, 14, 7, 28};
	bool first = (answerSet == soln);
	soln = {56, 28, 14, 2};
	bool second = (answerSet == soln);
	assert(first || second);


	// Test 2
	values = {10, 5, 3, 15, 20};
	ans = biggest_divisible_conglomerate(values);
	cout << "input: " << vec_to_string(values) << endl;
	cout << "output: " << vec_to_string(ans) << endl << endl;
	soln = {20, 5, 10};
	set<int> answerSet2(ans.begin(), ans.end());
	assert(answerSet2 == soln);

	// write your own tests here!
    values = {10, 20, 30, 40, 50};
	ans = biggest_divisible_conglomerate(values);
	cout << "input: " << vec_to_string(values) << endl;
	cout << "output: " << vec_to_string(ans) << endl << endl;
	soln = {10, 20, 40};
	set<int> answerSet3(ans.begin(), ans.end());
	assert(answerSet3 == soln);

    //testing empty list - takes long, but works
    values = {};
	ans = biggest_divisible_conglomerate(values);
	cout << "input: " << vec_to_string(values) << endl;
	cout << "output: " << vec_to_string(ans) << endl << endl;
	soln = {};
	set<int> answerSet4(ans.begin(), ans.end());
	assert(answerSet4 == soln);
    
    //testing just one value
    values = {1};
    ans = biggest_divisible_conglomerate(values);
	cout << "input: " << vec_to_string(values) << endl;
	cout << "output: " << vec_to_string(ans) << endl << endl;
	soln = {1};
	set<int> answerSet5(ans.begin(), ans.end());
	assert(answerSet5 == soln);

    //no divisible number possible 
    values = {11, 13, 19, 27};
    ans = biggest_divisible_conglomerate(values);
	cout << "input: " << vec_to_string(values) << endl;
	cout << "output: " << vec_to_string(ans) << endl << endl;
	soln = {11};
	set<int> answerSet6(ans.begin(), ans.end());
	assert(answerSet6 == soln);


	return 0;

}
