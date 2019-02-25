// basic file operations
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

struct slice {
  int first_row;
  int first_column;
  int last_row;
  int last_column;
};

class Pizza {

  std::vector<std::vector<char> pizza_matrix;
  int total_rows, total_columns, ingredients_in_slice, cells_in_slice;
  std::vector<slice> slices;


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

    for (int i = 0, i < total_rows, ++i){
      for(int j = 0; j < total_columns; ++j){
        if(pizza_matrix[i][j] != '0') //Char 0 means the position is already in a slice
          {
          row = i;
          col=j;
          return true;
          }
      }
    }
    return false;
  }

  // 0 - Slice is bigger than accepted.
  // 1 - Slice has a position already in another slice.
  // 2 - The ammount of ingredients its ok.
  // 3 - Slice is ok
  int satisfies_constraints(slice s)
  {

    // If slice bigger than acepted, return false;
    if (((s.last_row - s.first_row + 1) * (s.last_column - s.last_row + 1)) >= cells_in_slice)
      return 0;

    int num_t = 0, num_m = 0;

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

    // If the number of ingredients isnt enought
    if (num_m >= ingredients_in_slice && num_t >= ingredients_in_slice) 
      return 3;

    return 2;

  }

  // Sets the positions in the slice as '0'
  void update_pizza_matrix(Slice s){
    for ( int i = s.first_row; i <= s.last_row; ++i)
      for (int j = s.first_column; j <= s.last_column; ++j)
        pizza_matrix[i][j] = '0';
  }

  void calc_slices(){

    int row,col;

    while(get_next_available_position(row, col)){

      Slice s = {row, col, row, col};

      if(make_largest_valid_slice(s)){
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
