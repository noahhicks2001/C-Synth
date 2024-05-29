<pre>
# C-synth
A basic wavetable synthesizer created in C 
// IMPORTANT // 
    > If audio driver corruption occurs when modifying program, restart computer to reset driver. 
    > An additional volume scalar is applied to square and saw waves due to harsh increase in volume 
    > clipping can occur with use of lfo for frequencies below 150hz 
    
// INSTALL // 
  refer to release page for required external dependencies 

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
</pre>


    
    
    
    

