#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>

struct Photo {

  bool is_horizontal;
  int number_of_tags;
  std::vector<std::string> tags;
  int id;

  Photo(){
      id = -1;
  }

  Photo(int _id, bool _horizontal, int num_tags, std::vector<std::string>& _tags){
    id = _id;
    is_horizontal = _horizontal;
    number_of_tags = num_tags;
    tags = _tags;
  }

};

struct Slide {
    bool one_or_two_photos;

    int id1, id2; //If only one photo, id2 not used
    std::vector<std::string> tags;

    Slide(Photo p){
        one_or_two_photos = 0;
        id1 = Photo.id;
        tags = Photo.tags;
    }

    Slide(Photo p1, Photo p2){
        one_or_two_photos = 1;
        id1 = Photo1.id;
        id2 = Photo2.id;
        tags = Photo1.tags;
        tags.insert(tags.end(), Photo2.tags.begin(), Photo2.tags.end())
    }
};

class SlideShow{

    public:
    std::vector<Slide> slides;
    int num_photos;

    SlideShow(char* filename){
        std::fstream input_data(filename, std::ios_base::in);

        input_data >> num_photos;

        bool already_found_v_photo = false;
        Photo saved_v();
        int num_of_tags;
        std::vector<std::string> tags;
        char orientation;

        for (int i = 0; i < num_photos; i++){
            input_data >> orientation;
            input_data >> num_of_tags;
            for (int j = 0; j < num_of_tags; j++) {
                std::string tag;
                input_data >> tag;
                tags.push_back(tag);
            }

            if (orientation == 'H'){
                Photo aux(i, true, num_of_tags, tags);
                Slide s(aux);
                slides.push_back(s);
            }
            else{
                Photo aux(i, false, num_of_tags, tags);

                if(already_found_v_photo){
                    Slide s(saved_v, aux)
                    slides.push_back(s);
                }
                else{
                    saved_v = aux;
                }
                already_found_v_photo = !already_found_v_photo;
            }
        }

        input_data.close();
    }
};
