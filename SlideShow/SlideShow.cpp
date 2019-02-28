class SlideShow{

    public:
    int num_photos;
    std::vector<Photo> photos;

    SlideShow(char* filename){

        std::fstream input_data(filename, std::ios_base::in);

        input_data >> num_photos;

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
}
