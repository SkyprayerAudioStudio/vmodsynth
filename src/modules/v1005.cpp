/*
    Copyright (C) 2012, 2013 Rafał Cieślak

    This file is part of vModSynth.

    vModSynth is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    vModSynth is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with vModSynth.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "v1005.h"
#include "../AlsaDriver.h"
#include <iostream>
#include <tuple>

v1005::v1005()
{
    name = "MIDI input";
    caption = "MIDI INPUT";
    type_id = 1005;
    panel_width = 136;
    add_outlet(33,90);   //pitch
    add_outlet(103,90);  //pitch2
    add_outlet(33,165);  //velocity
    add_outlet(103,165); //gate
    add_selector(68,267,17,0,false); //channel
    last_gate_pitch = 0;
}

v1005::~v1005()
{
    //dtor
}

void v1005::dsp_full(AudioContext ctx){
    int channel = knobs[0]->get_value();
    int p,v;
    std::tie(p, v) = ctx.midi_driver->GetLastNote(channel);
    double pitch = p/12.0;
    double velocity = v*5.0/128.0;
    outlets[0]->push_sample(pitch);
    outlets[1]->push_sample(pitch);
    outlets[2]->push_sample(velocity);
    double gate;
    std::cout << velocity << std::endl;
    if(v == 0){
        gate = 0;
        last_gate_pitch = 0;
    }else
        if(p == last_gate_pitch || 0 == last_gate_pitch){
            gate = 1;
            last_gate_pitch = p;
        }else{ //new pitch is different than previous, therefore a single 0-sample shall be emitted
            gate = 0;
            last_gate_pitch = 0; //this will cause to proceed to normal gate on the next sample
        }
    outlets[3]->push_sample(5.0*gate);
}

void v1005::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.3);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(3.0);
    cr->move_to(33,90);
    cr->line_to(103,90);
    cr->stroke();

    draw_text(cr,"PITCH (1V/OCTAVE)"   ,10.0, 68,  56);
    draw_text(cr,"VELOCITY",10.0, 33,  131);
    draw_text(cr,"GATE"    ,10.0, 103, 131);
    draw_text(cr,"EXCLUSIVE CHANNEL"    ,10.0, 68, 220);

    cr->restore();
}
