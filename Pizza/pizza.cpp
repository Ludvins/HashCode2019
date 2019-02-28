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

    for (int i = 1; i < square_root; i++) {
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

    int get_area(){
        return (last_row - first_row +1) * (last_column - first_column +1);
    }
};

enum constraints_code {
    slice_out_of_pizza_range,
    size_bigger_than_accepted,
    not_enought_ingredients,
    slice_over_another_slice,
    slice_is_ok
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

        for (next_row; next_row < total_rows; ++next_row){
            for(next_col; next_col < total_columns; ++next_col){
                if(!slice_pointer_matrix[next_row][next_col]) //nullptr means position isn't in a slice
                {
                    row = next_row;
                    col = next_col;

                    /* std::cout << next_row << next_col << std::endl; */
                    next_row = next_row + (next_col+1)/total_columns;
                    next_col = (next_col + 1) % total_columns;
                    /* std::cout << next_row << next_col << std::endl; */

                    /* std::cout << "Next pos: r:" << row << " c:" << col << std::endl; */
                    return true;
                }
            }

        }
        /* std::cout << "!Next pos: r:" << row << " c:" << col << std::endl; */
        return false;
    }

    constraints_code satisfies_constraints(Slice s)
    {

        if (s.first_row < 0 || s.first_column < 0 || s.last_row >= total_rows || s.last_column >= total_columns)
            return constraints_code::slice_out_of_pizza_range;

        // Check if slice is bigger than accepted;
        if (s.get_area() > cells_in_slice)
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
        if (num_m < ingredients_in_slice || num_t < ingredients_in_slice)
            return constraints_code::not_enought_ingredients;

        // Everything is correct, return 2.
        return constraints_code::slice_is_ok;

    }

    // Updates positions in the slice
    void update_pizza_matrix(Slice s){
        for ( int i = s.first_row; i <= s.last_row; ++i)
            for (int j = s.first_column; j <= s.last_column; ++j)
                slice_pointer_matrix[i][j] = &s;
    }

    bool make_largest_slice(Slice &s){

        // Try every possible size in descendant order
        for (int i = cells_in_slice; i > 0; --i) {
            std::vector<int> first_half_divs = first_half_divisors(i);
            for (auto div : first_half_divs ){
                for (int j = 0; j < 2; ++j){
                    s.last_row      = s.first_row + (j ? i/div : div) -1;
                    s.last_column   = s.first_column + (j ? div : i/div) -1;
                    std::cout << std::endl;

                    std::cout << s.first_row << s.last_row << std::endl;
                    std::cout << s.first_column << s.last_column << std::endl;

                    constraints_code s_c_ret = satisfies_constraints(s);
                    std::cout << (int) s_c_ret << std::endl;
                    switch (s_c_ret) {

                        case slice_over_another_slice:
                            //Slice is over another slice already selected.

                            // TODO Check

                            // for (int k = s.first_row; k <= s.last_row; ++k)
                            //   for (int l = s.first_column; l <= s.last_column; ++l)
                            //     slice_pointer_matrix[k][l] = &s;

                            break;

                        case slice_is_ok:
                            // Slice is correct
                            return true;
                            break;

                        default:

                            break;
                    }
                }

            }
        }

        return false;
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

    // Show slices and return the percentage area of pizza sliced
    float show_slices(){
        int  area_of_pizza_sliced = 0;

        for (auto slice : slices){
            std::cout <<  slice.first_row << " " << slice.last_row << " " << slice.first_column << " " << slice.last_column << " " << std::endl;
            area_of_pizza_sliced += slice.get_area();
        }

        return (float) area_of_pizza_sliced / (total_columns * total_rows);
    }
};

int main (int argc, char** argv) {

    Pizza pizza (argv[1]);

    pizza.printf();

    pizza.calc_slices();
    std::cout << pizza.show_slices() << std::endl;
    return 0;
}
