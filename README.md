# C-synth

C-synth is a basic wavetable synthesizer created in the C programming language making use of SDL2 and portaudio. Input and visuals are handled through a user interface as pictured below which provides methods to change frequency, adjust volume/lfo values, select the type of wave used for wavetable synthesis and display the partial waves and resulting wave from additive synthesis of partials.

<div align=center>
<video src= "https://github.com/noahhicks2001/C-Synth/assets/91241564/545e5305-c668-4192-a4a8-027ab9411684"/>
</div>
Audio is synthesized using wavetables for the wave types of sine, square, saw and triangle.  In addition, complex waves can be generated through use of additive synthesis of up to three partial waves.

Two lfos for wobble and tremolo are also included for the ability to modulate the synthesized output.

For use of controls and notes pertaining to user experience please refer to readme document provided.

Lastly, as C-synth makes use of [SDL2](https://www.libsdl.org/) and [Portaudio](https://www.portaudio.com/), some initial setup is required before compilation. For SDL2 the following links provide an excellent guide for setup of [SDL2](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php) and setup of [TTF_Fonts](https://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php).

Then for Portaudio, the following provides another great guide for setup of [Portaudio](https://cindybui.me/pages/blogs/visual_studio_0) for visual studio, (Also note that the TTF font used is 
arial.ttf)

For an explanation of how the wavetable increment works, check out this [resource](https://www.music.mcgill.ca/~gary/307/week4/wavetables.html)


<pre>
// IMPORTANT // 
    > KEEP VOLUME AT LOW LEVEL IN CASE OF AUDIO CLIPPING OCCURING 
    > CLIPPING CAN OCCUR WITH USE OF LFO FOR FREQUENCIES BELOW 150HZ
    > IF AUDIO DRIVER CORRUPTION OCCURS WHEN MODIFYING PROGRAM, RESTART COMPUTER TO RESET DRIVER
    > AN ADDITIONAL VOLUME SCALAR IS APPLIED TO SQUARE AND SAW WAVES DUE TO HARSH INCREASE IN VOLUME
    
// CONTROLS // 

  KEYBOARD: 
    Volume: 
      q->down 
      w->up 
    wobble: 
      e->down 
      r->up 
    tremolo: 
      r->down 
      t->up 
    additive-zoom:  // controls zoom for additive wave 
      a->zoom out 
      s->zoom in 
    partial-zoom:  // controls zoom for selected partial wave 
      d->zoom out 
      f->zoom in 
    text: 
      = -> enter text  // text for frequency can only be entered when max text length is reached 
                       // use decimal to set tens placement 
      backspace -> delete character 
    partial-waves: 
        up-arrow -> add partial wave 
        down-arrow -> remove partial wave 

  MOUSE: 
    partials:
      click on desired partial with rmb to select 
    textboxes: 
      click on desired textbox with rmb to enter text, right click to deselect 
    wavetables: 
      click on desired wavetable with rmb to select and apply to selected partial 
      click with lmb to change wave type used for lfos 
    
TODO:
    -> fix struct namespace pollution
</pre>




    
    
    
    

