// basic file operations
#include <iostream>
#include <fstream>
#include <vector>

struct slice {
  int first_row;
  int last_row;
  int first_column;
  int last_column;
};

class Pizza {


  std::vector<std::vector<char>> pizza_matrix;
  int total_rows, total_columns, ingredients_in_slice, cells_in_slice;
  vector<slices>;


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

  void calc_slices(){









  }


};


int main (int argc, char** argv) {


  Pizza pizza (argv[1]);

  pizza.printf();




  return 0;
}
