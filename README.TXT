if you decided to try to play the game you should
add the png and the wav and the ttf files to your
program and change the loading stuff with your 
own path 

this is a linker  :  
g++ -c Stars_game.cpp -I"SFML-2.6.1\include" -DSFML_STATIC

and this is a compiler :  
g++ Stars_game.o -o Stars_game -L"SFML-2.6.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows 

and you should have the sfml dlls instaled in 
your computer  and edit the path of the linker 
and the compiler considering your own path 


and if you want to play the game directly
without all of this you can follow this link it
will take you to a set_up file where you can download
the game :
https://drive.google.com/drive/folders/1RmUmdvceAAIguDLeU0VP6NI7ScZ9Zf99?usp=sharing

