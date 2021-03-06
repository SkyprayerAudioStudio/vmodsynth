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


#include "v200.h"

v200::v200()
{
    name = "Multiply";
    caption = "MULTIPLY";

    type_id = 200;
    panel_width = 136;
    add_inlet(33,90);
    add_outlet(103,90);
    add_outlet(33,165);
    add_outlet(103,165);
    add_inlet(33,270);
    add_outlet(103,270);
    add_outlet(33,345);
    add_outlet(103,345);
    add_inlet(33,450);
    add_outlet(103,450);
    add_outlet(33,525);
    add_outlet(103,525);

    add_switch(68,217);
    add_switch(68,397);
}

v200::~v200()
{
    //dtor
}

void v200::dsp(){
    double in1 = inlets[0]->pull_sample();
    double in2 = inlets[1]->pull_sample();
    double in3 = inlets[2]->pull_sample();
    double out1 = 0.0, out2 = 0.0, out3 = 0.0;
    bool s1 = switches[0]->get_value();
    bool s2 = switches[1]->get_value();
    if(!s1 && !s2){ //none on
        out1 = in1;
        out2 = in2;
        out3 = in3;
    }else if(s1 && !s2){ //first on
        out1 = out2 = in1+in2;
        out3 = in3;
    }else if(!s1 && s2){ //second on
        out1 = in1;
        out3 = out2 = in2+in3;
    }else if(s1 && s2){ //both on
        out1 = out2 = out3 = in1+in2+in3;
    }
    outlets[0]->push_sample(out1);
    outlets[1]->push_sample(out1);
    outlets[2]->push_sample(out1);
    outlets[3]->push_sample(out2);
    outlets[4]->push_sample(out2);
    outlets[5]->push_sample(out2);
    outlets[6]->push_sample(out3);
    outlets[7]->push_sample(out3);
    outlets[8]->push_sample(out3);
}

void v200::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.3);
    draw_caption_and_id(cr);
    cr->save();
    cr->translate(xoffset,0);
    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(2.5);

    //squares
    cr->move_to(33,90);
    cr->line_to(103,90);
    cr->line_to(103,165);
    cr->line_to(33,165);
    cr->close_path();
    cr->stroke();
    cr->move_to(33,270);
    cr->line_to(103,270);
    cr->line_to(103,345);
    cr->line_to(33,345);
    cr->close_path();
    cr->stroke();
    cr->move_to(33,450);
    cr->line_to(103,450);
    cr->line_to(103,525);
    cr->line_to(33,525);
    cr->close_path();
    cr->stroke();

    //switches
    cr->move_to(68,165);
    cr->line_to(68,270);
    cr->stroke();
    cr->move_to(68,345);
    cr->line_to(68,450);
    cr->stroke();


    cr->restore();
}
