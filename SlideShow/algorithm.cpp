
// Seleccionamos la primera foto
vector<Photo> photos;
Photo photoActual = photos[0];
photos.remove( photoActual );


//Añadimos a la solucion mientras queden fotos.
while( ! photos.empty() )
  {
    if( photos.size() == 1 )
      {
      AddSlide( photos[0] );  //Añade la ultima slide si solo queda 1
      cout << "  -- Añadida útlima foto." << endl;
      }
    else
      {
        //Cogemos la primera opcion y la comparamos con las demas
        int mejor = 0,
          dMejor = metrica( photoActual, photo ),
          d;
        for( int i = 1; i < photos.size() ; ++i )
          {
            d = metrica( photoActual, photos[i]);
            if( d > dMejor )
              {
                dMejor = d;
                mejor = i;
              }
          }

        addSlide( photos[mejor] );
        Photo photoActual = photos[mejor];
        photos.remove( photos[mejor] );
      }
  }
