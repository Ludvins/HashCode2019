// basic file operations
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

struct Slice {
  int first_row;
  int first_column;
  int last_row;
  int last_column;
};

class Pizza {

  std::vector<std::vector<char>> pizza_matrix;
  int total_rows, total_columns, ingredients_in_slice, cells_in_slice;
  std::vector<Slice> slices;


public:
  Pizza(char* filename){

    std::fstream input_data(filename, std::ios_base::in);

    input_data >> total_rows >> total_columns >> ingredients_in_slice >> cells_in_slice;

    std::vector<char> row;
    char c;

    for ( int i = 0; i < total_rows; i++){
      for ( int j = 0 ; j < total_columns; j++){
        input_data >> c;
        row.push_back(c);
      }
      pizza_matrix.push_back(row);
      row.clear();
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

    for (int i = 0; i < total_rows; ++i){
      for(int j = 0; j < total_columns; ++j){
        if(pizza_matrix[i][j] != '0') //Char 0 means the position is already in a slice
          {
          row = i;
          col = j;
          return true;
          }
      }
    }
    return false;
  }

  // 0 - Slice is bigger than accepted.
  // 1 - Slice has a position already in another slice.
  // 2 - Slice is ok
  // 3 - The ammount of ingredients isn't ok.
  // 4 - Slice is over the margins of the pizza
  int satisfies_constraints(Slice s)
  {

    if (s.first_row < 0 || s.first_column < 0 || s.last_row >= total_rows || s.last_column >= total_columns)
      return 4;

    // Check if slice is bigger than accepted;
    if (((s.last_row - s.first_row + 1) * (s.last_column - s.last_row + 1)) >= cells_in_slice)
      return 0;

    int num_t = 0, num_m = 0; //initialice number of tomatoes and mush in the slice.

    for (int i = s.first_row ; i <= s.last_row; ++i){
      for(int j = s.first_column; j<= s.last_column; ++j){

        if (pizza_matrix[i][j] == '0') // if any of the cells is already in a slice, return false.
          return 1;

        if (pizza_matrix[i][j] == 'M')
          num_m++;
        else
          num_t++;

      }
    }

    // check the number of ingredients
    if (num_m >= ingredients_in_slice && num_t >= ingredients_in_slice) 
      return 3;

    // Everything is correct, return 2.
    return 2;

  }

  // Sets the positions in the slice as '0'
  void update_pizza_matrix(Slice s){
    for ( int i = s.first_row; i <= s.last_row; ++i)
      for (int j = s.first_column; j <= s.last_column; ++j)
        pizza_matrix[i][j] = '0';
  }

  bool get_largest_slice(Slice s){

    int s_c_ret = satisfies_constraints(s);


    // Get this switch in a loop (?)

    switch (s_c_ret) {

    case 1:
      //Slice is over another slice already selected.

      // Check if slice is worth?
      // if it is, update the accepted slice, (we dont have a way to know which slice is it, so mmmm)

      break;

    case 2:
      // Slice is correct
      return true;
      break;

    case 3:

      // the number of ingredients isnt enought so we must make it larger ??
      break;

    case 0:
    case 4:
    default:

      // Slice is so bad it cant be helped
      return false;
      break;
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
