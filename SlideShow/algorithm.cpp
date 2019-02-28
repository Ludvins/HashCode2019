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
