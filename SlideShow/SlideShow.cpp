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

    int id1, id2; // If only one photo, id2 not used
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
                tags.clear();
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

        // Greedy Algorithm with O(n^2) efficiency
        void calculate_slide_show(){
            Slide current_slide = slides[0];
            final_slideshow.push_back(current_slide);
            slides.erase(slides.begin());
            int a = 0;
            int total = slides.size();

            while( !slides.empty() )
            {
                std::cout << a++ << " de " << total << std::endl;
                if( slides.size() == 1 )
                {
                    final_slideshow.push_back( slides[0] );
                    slides.clear();
                }
                else
                {
                    auto best_slide = slides.begin();
                    int best_distance = metric( current_slide, slides[0] );
                    int aux_distance;

                    for(auto slide_i = ++slides.begin(); slide_i != slides.end(); ++slide_i) {
                        aux_distance = metric( current_slide, *slide_i);
                        if( aux_distance > best_distance)
                        {
                            best_distance = aux_distance;
                            best_slide = slide_i;
                        }
                    }

                    final_slideshow.push_back(*best_slide);
                    current_slide = *best_slide;
                    slides.erase(best_slide);
                }
            }
        }

        void show_slides_vector(){
            std::cout << slides.size() << std::endl;
            for (auto slide : slides){
                    if (slide.one_photo)
                        std::cout << slide.id1 << std::endl;
                    else
                        std::cout << slide.id1 << " " << slide.id2 << std::endl;
            }
        }

        void show_final_slides_vector(){
            std::cout << final_slideshow.size() << std::endl;
            for (auto slide : final_slideshow){
                    if (slide.one_photo)
                        std::cout << slide.id1 << std::endl;
                    else
                        std::cout << slide.id1 << " " << slide.id2 << std::endl;
            }
        }

        void write_output_file(){
            std::fstream output_data("results.txt", std::ios_base::out);
            if (output_data.is_open()){

                output_data << final_slideshow.size() << std::endl;
                for (auto slide : final_slideshow){
                    if (slide.one_photo)
                        output_data << slide.id1 << std::endl;
                    else
                        output_data << slide.id1 << " " << slide.id2 << std::endl;
                }
            }
            else {
                std::cout << "Error opening file" << std::endl;
            }
        }
};

int main (int argc, char** argv){
    SlideShow slideshow(argv[1]);

    slideshow.calculate_slide_show();
    slideshow.write_output_file();
    return 0;
}
