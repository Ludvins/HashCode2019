// basic file operations
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>

// **********************************************************************************
// Auxiliary functions

// Returns first half of the divisors
// Example: 12 -> 1 2 3
std::vector<int> first_half_divisors(int num){
    int square_root = (int) sqrt(num) + 1;
    std::vector<int> result;

    for (int i = square_root-1; i > 0 ; i++) {
        if (num % i == 0)
            result.push_back(i);
    }

    return result;
}

// **********************************************************************************

struct Slice {
    int first_row;
    int first_column;
    int last_row;
    int last_column;
};

enum constraints_code {
    slice_out_of_pizza_range,
    not_enought_ingredients,
    slice_over_another_slice,
    slyce_is_ok
};

class Pizza {

    std::vector<std::vector<char>> pizza_matrix;
    std::vector<std::vector<Slice*>> slice_pointer_matrix;
    int total_rows, total_columns, ingredients_in_slice, cells_in_slice;
    std::vector<Slice> slices;

    public:
    Pizza(char* filename){

        std::fstream input_data(filename, std::ios_base::in);

        input_data >> total_rows >> total_columns >> ingredients_in_slice >> cells_in_slice;

        std::vector<char> row;
        std::vector<Slice*> slice_row;
        char c;

        for ( int i = 0; i < total_rows; i++){
            for ( int j = 0 ; j < total_columns; j++){
                input_data >> c;
                row.push_back(c);
                slice_row.push_back(nullptr);
            }
            pizza_matrix.push_back(row);
            slice_pointer_matrix.push_back(slice_row);
            row.clear();
            slice_row.clear();
        }

        input_data.close();
    }

    void printf(){

        std::cout << total_rows << total_columns << ingredients_in_slice << cells_in_slice << std::endl;

        for ( int i = 0; i < total_rows; i++){
            for ( int j = 0 ; j < total_columns; j++){
                std::cout << pizza_matrix[i][j];
            }
            std::cout << std::endl;
        }

    }

    // If true is returned this stores the position in row and col,
    // false means there no more avaible positions.
    bool get_next_available_position(int& row, int& col){

        static int next_row = 0, next_col = 0;

        for (int i = next_row; i < total_rows; ++i){
            for(int j = next_col; j < total_columns; ++j){
                if(!slice_pointer_matrix[i][j]) //nullptr means position isn't in a slice
                {
                    next_row = row = i;
                    next_col = col = j;
                    return true;
                }
            }
        }
        return false;
    }

    constraints_code satisfies_constraints(Slice s)
    {

        if (s.first_row < 0 || s.first_column < 0 || s.last_row >= total_rows || s.last_column >= total_columns)
            return constraints_code::slice_out_of_pizza_range;

        // Check if slice is bigger than accepted;
        if (((s.last_row - s.first_row + 1) * (s.last_column - s.last_row + 1)) >= cells_in_slice)
            return constraints_code::size_bigger_than_accepted;

        int num_t = 0, num_m = 0; //initialice number of tomatoes and mush in the slice.

        for (int i = s.first_row ; i <= s.last_row; ++i){
            for(int j = s.first_column; j<= s.last_column; ++j){

                if (slice_pointer_matrix[i][j]) // if any of the cells is already in a slice, return false.
                    return constraints_code::slice_over_another_slice;

                if (pizza_matrix[i][j] == 'M')
                    num_m++;
                else
                    num_t++;

            }
        }

        // check the number of ingredients
        if (num_m >= ingredients_in_slice && num_t >= ingredients_in_slice)
            return constraints_code::not_enought_ingredients;

        // Everything is correct, return 2.
        return constraints_code::slyce_is_ok;

    }

    // Updates positions in the slice
    void update_pizza_matrix(Slice s){
        for ( int i = s.first_row; i <= s.last_row; ++i)
            for (int j = s.first_column; j <= s.last_column; ++j)
                slice_pointer_matrix[i][j] = &s;
    }

    bool make_largest_slice(Slice &s){

        // Try every possible size in descendant order
        for (i = cells_in_slice; i > 0; --i) {
            std::vector<int> first_half_divs = first_half_divisors(i);

            for (auto div : first_half_divs ){
                for (int j = 0; j < 2; ++j){
                    s.last_row      = (j : i/div ? div);
                    s.last_column   = (j : div ? i/div);

                    int s_c_ret = satisfies_constraints(s);

                    switch (s_c_ret) {

                        case slice_over_another_slice:
                            //Slice is over another slice already selected.

                            // Check if slice is worth?
                            // if it is, update the accepted slice, (we dont have a way to know which slice is it, so mmmm)

                            break;

                        case slice_is_ok:
                            // Slice is correct
                            return true;
                            break;

                        default:

                    }
                }

            }
        }
    }

    void calc_slices(){

        int row,col;

        while(get_next_available_position(row, col)){ //While there are available positions

            Slice s = {row, col, row, col}; //Construct trivial slice.

            if(make_largest_slice(s)){ //If the largest slice is valid.
                slices.push_back(s);
                update_pizza_matrix(s);
            }

        }
    }
};

int main (int argc, char** argv) {

    Pizza pizza (argv[1]);

    pizza.printf();

    return 0;
}
