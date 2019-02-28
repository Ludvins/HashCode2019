#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <set>

struct Photo {

    int id;
    bool is_horizontal;
    int number_of_tags;
    std::set<std::string> tags;

    Photo(){
        id = -1;
    }

    Photo(int _id, bool _horizontal, int num_tags, std::set<std::string>& _tags){
        id = _id;
        is_horizontal = _horizontal;
        number_of_tags = num_tags;
        tags = _tags;
    }

};

struct Slide {
    bool one_photo;

    int id1, id2; //If only one photo, id2 not used
    std::set<std::string> tags;

    Slide(Photo p){
        one_photo = true;
        id1 = p.id;
        tags = p.tags;
    }

    Slide(Photo p1, Photo p2){
        one_photo = false;
        id1 = p1.id;
        id2 = p2.id;
        tags = p1.tags;
        tags.insert(p2.tags.begin(), p2.tags.end());
    }
};

class SlideShow{

    public:
        std::vector<Slide> slides;
        std::vector<Slide> final_slideshow;
        int num_photos;

        SlideShow(char* filename){
            std::fstream input_data(filename, std::ios_base::in);

            input_data >> num_photos;

            bool already_found_v_photo = false;
            Photo saved_v;
            int num_of_tags;
            std::set<std::string> tags;
            char orientation;

            for (int i = 0; i < num_photos; i++){
                input_data >> orientation;
                input_data >> num_of_tags;
                for (int j = 0; j < num_of_tags; j++) {
                    std::string tag;
                    input_data >> tag;
                    tags.insert(tag);
                }

                if (orientation == 'H'){
                    Photo aux(i, true, num_of_tags, tags);
                    Slide s(aux);
                    slides.push_back(s);
                }
                else{
                    Photo aux(i, false, num_of_tags, tags);

                    if(already_found_v_photo){
                        Slide s(saved_v, aux);

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

        void calculate_slide_show(){

            Slide current_slide = slides[0];
            final_slide_show.push_back(current_slide);
            slides.erase(slides.begin());

            //Añadimos a la solucion mientras queden fotos.
            while( !slides.empty() )
            {
                if( slides.size() == 1 )
                {
                    slides.push_back( slides[0] );  //Añade la ultima slide si solo queda 1
                    std::cout << "  -- Añadida útlima foto." << std::endl;
                }
                else
                {
                    //Cogemos la primera opcion y la comparamos con las demas
                    auto best_slide = slides.begin();
                    int best_distance = metrica( current_slide, photos[0] );
                    int aux_distance;

                    for(auto slide_i = ++slides.begin(); slide_i != slides.end(); ++slide_i) {
                        aux_distance = metrica( current_slide, *slide_i);
                        if( aux_distance > best_distance )
                        {
                            best_distance = aux_distance;
                            best_slide = slide_i;
                        }
                    }

                    final_slide_show.push_back(*best_slide);
                    current_slide = *best_slide;
                    slides.erase(best_slide);
                }
            }
        }
};

int metric(Slide s1, Slide s2){

    int tags_only_in_s1 = 0, tags_only_in_s2 = 0, common_tags = 0;

    for (auto tag : s1.tags){
        if (s2.tags.find(tag) == s2.tags.end())
            tags_only_in_s1++;
        else
            common_tags++;
    }
    for ( auto tag : s2.tags){
        if(s1.tags.find(tag) == s1.tags.end())
            tags_only_in_s2++;
    }

    return std::min(std::min(tags_only_in_s2, tags_only_in_s1), common_tags);

}

int main (int argc, char** argv){
    SlideShow slideshow(argv[1]);

    return 0;
}
